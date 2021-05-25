/**
 * @file	led.c
 * @author	Derek Molloy
 * @date	19 April 2015
 * @brief A kernel module for controll a simple LED (or any signal) that is
 * connected to a GPIO. It is thread in order that it can flash the LED.
 * The sysfs entry appears at /sys/ebb/led49
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>		// Required for the GPIO functions
#include <linux/kobject.h>	// Using kobjects for the sysfs bindings
#include <linux/kthread.h>	// Using kthreads for the flash functionality
#include <linux/delay.h>	// Using this header for the msleep() function

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Derek Molloy");
MODULE_DESCRIPTION("A simple Linux LED driver LKM for the BBB");
MODULE_VERSION("0.1");

static unsigned int gpioLED = 49;	// Default GPIO for the LED is 49
module_param(gpioLED, uint, S_IRUGO);	// S_IRUGO can be read/not changed
MODULE_PARM_DESC(gpioLED, "GPIO LED number (default=49)");

static unsigned int blinkPeriod = 1000;	// The blink period in ms
module_param(blinkPeriod, uint, S_IRUGO);
MODULE_PARM_DESC(blinkPeriod, "LED blink period in ms (min=1, default=1000), max=10000");
static char ledName[7] = "ledXXX";	// NULL terminate default string
static bool ledOn = 0;			// Is LED ON or OFF. Used for flash
enum modes{OFF, ON, FLASH}; // The available LED modes -- static not useful here
static enum modes mode = FLASH; 	// Default mode is flash

/** @brief A callback function to display the LED mode
 * @param kobj Represent a kernel object device that appears in sysfs filesystem
 * @param attr Pointer to kobj_attribute struct
 * @param buf Buffer which to write the number of presses
 * @return Return number of characters of the mode string successful displayed
 */
static ssize_t mode_show(struct kobject *kobj, struct kobj_attribute *attr,
								char *buf)
{
	switch (mode) {
		case OFF: return sprintf(buf, "off\n"); // Display the state
		case ON: return sprintf(buf, "on\n");
		case FLASH: return sprintf(buf, "flash\n");
		default: return sprintf(buf, "LKM Error\n");
	}
}

/** @brief A callback function to store the LED mode using enum above */
static ssize_t mode_store(struct kobject *kobj, struct kobj_attribute *attr,
						const char *buf, size_t count)
{
	// count-1 is important as otherwise the \n is used in the comparison
	if (strncmp(buf, "on", count-1) == 0)
		mode = ON;
	if (strncmp(buf, "off", count-1) == 0)
		mode = OFF;
	if (strncmp(buf, "flash", count-1) == 0)
		mode = FLASH;
	return count;
}

/** @brief A callback function to display the LED period */
static ssize_t period_show(struct kobject *kobj, struct kobj_attribute *attr,
								char *buf)
{
	return sprintf(buf, "%d\n", blinkPeriod);
}

/** @brief A callback function to store the LED period value */
static ssize_t period_store(struct kobject *kobj, struct kobj_attribute *attr, 
						const char *buf, size_t count)
{
	unsigned int period; // Using a variable to validate the data sent

	sscanf(buf, "%du", &period); // Read in the period
	if ((period > 1) && (period <= 10000)) // Must be >= 2ms, <= 10s
		blinkPeriod = period;

	return period;
}

/** Use these helper macros to define the name and access levels of 
 * kobj_attributes. The kobj_attribute has an attribute attr (name and mode), show
 * and store function pointers. The period variable is associated with the
 * blinkPeriod variable and it is to be exposed with mode 0660 using period_show
 * and period_store functions above.
 */
static struct kobj_attribute period_attr = __ATTR(blinkPeriod, 0660, period_show, period_store);
static struct kobj_attribute mode_attr = __ATTR(mode, 0660, mode_show, mode_store);
/** the ebb_attrs[] is an array of attributes that is used to create the attribute
 * group below. The attr property of kobj_attribute is used to extract the
 * attribute struct.
 */
static struct attribute *ebb_attrs[] = {
	&period_attr.attr,	// Period which the LED flash
	&mode_attr.attr,	// mode of the LED
	NULL,
};

/** The attribute group uses the attribute array and a name, which is exposed on
 * sysfs -- in this case it is gpio49, which is automatic defined in ebbLED_init()
 * functions using the custom kernel parameter that can be passed when the module
 * is loaded.
 */
static struct attribute_group attr_group = {
	.name = ledName,	// the name is generated in ebbLED_init()
	.attrs = ebb_attrs,
};

static struct kobject *ebb_kobj;	// The pointer to the kobject
static struct task_struct *task;	// The pointer to the thread task

/** @brief The LED flash main kthread loop
 * @param arg A void pointer used in order to pass data to the thread
 * @return Return 0 if success
 */
static int flash(void *arg)
{
	printk(KERN_INFO "EBB LED: Thread has started running\n");

	while (!kthread_should_stop()) { // Return true when kthread_stop() called
		set_current_state(TASK_RUNNING);

		if (mode == FLASH)
			ledOn = !ledOn; // Invert the LED state
		else if (mode == ON)
			ledOn = true;
		else
			ledOn = false;
		gpio_set_value(gpioLED, ledOn); // use LED state to on/off LED

		set_current_state(TASK_INTERRUPTIBLE);
		msleep(blinkPeriod/2); // ms sleep for half of the period
	}

	printk(KERN_INFO "EBB LED: Thread has run to completion\n");

	return 0;
}

/** @brief The LKM init fucntion
 * In this example, this function set up the GPIOs and the IRQ
 */
static int __init ebbLED_init(void)
{
	int result = 0;

	printk(KERN_INFO "EBB LED: Initializing the EBB LED LKM\n");
	// create the gpio49 name for /sys/ebb/led49
	sprintf(ledName, "led%d", gpioLED);
	
	// kernel_kobj points to /sys/kernel
	ebb_kobj = kobject_create_and_add("ebb", kernel_kobj->parent);
	if (!ebb_kobj) {
		printk(KERN_ALERT "EBB LED: Failed to create kobjects\n");
		
		return -ENOMEM;
	}

	// add the attributes to /sys/ebb
	result = sysfs_create_group(ebb_kobj, &attr_group);
	if (result) {
		printk(KERN_ALERT "EBB LED: Failed to create sysfs group\n");
		kobject_put(ebb_kobj); // cleanup -- remove kobject sysfs entry

		return result;
	}

	ledOn = true;
	gpio_request(gpioLED, "sysfs");
	gpio_direction_output(gpioLED, ledOn); // set GPIO output, turn on
	/* this function will cause gpio49 to appear in /sys/class/gpio
	 * the second argument prevents the direction from being changed
	 */
	gpio_export(gpioLED, false);

	// start LED flash thread
	task = kthread_run(flash, NULL, "LED_flash_thread");
	if (IS_ERR(task)) {
		printk(KERN_ALERT "EBB LED: Failed to create task\n");

		return PTR_ERR(task);
	}

	return result;
}

/** @brief The LKM cleanup function */
static void __exit ebbLED_exit(void)
{
	kthread_stop(task);	// Stop LED flash thread
	kobject_put(ebb_kobj);	// cleanup -- remove kobject sysfs entry
	gpio_set_value(gpioLED, 0); // Turn LED OFF
	gpio_unexport(gpioLED); // Unexport LED GPIO
	gpio_free(gpioLED);	// Free LED GPIO

	printk(KERN_INFO "EBB LED: Goodbye from the EBB LKM!\n");
}

module_init(ebbLED_init);
module_exit(ebbLED_exit);
