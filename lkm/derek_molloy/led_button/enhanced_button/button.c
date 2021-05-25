/**
 * @file	button.c
 * @author 	Derek Molloy
 * @date	19 April 2015
 * @brief A kernel module for controlling a button (or any signal) that is
 * connected to a GPIO. It has full support for interrupts and for sysfs entries
 * so that an interface can be created to the button or the button can be configure
 * from Linux user-space.
 * The sysfs entry appears at /sys/ebb/gpio115
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>		// Required for the GPIO functions
#include <linux/interrupt.h>	// Required for IRQ code
#include <linux/kobject.h>	// Using kojects for the sysfs bindings
#include <linux/time.h> // Using the clock to measure time between button presses

#define DEBOUNCE_TIME 200	// bounce time - 200ms

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Derek Molloy");
MODULE_DESCRIPTION("A simple Linux GPIO Button LKM for the BBB");
MODULE_VERSION("0.1");

static bool isRising = 1; // Rising edge is the degault IRQ property
module_param(isRising, bool, S_IRUGO);	// S_IRUGO can be read/not changed
MODULE_PARM_DESC(isRising, "Rising edge = 1 (default), Falling edge = 0");

static unsigned int gpioButton = 115;
module_param(gpioButton, uint, S_IRUGO);
MODULE_PARM_DESC(gpioButton, "GPIO Button number (115)");

static unsigned int gpioLED = 49;
module_param(gpioLED, uint, S_IRUGO);
MODULE_PARM_DESC(gpioLED, "GPIO LED number (49)");

static char gpioName[8] = "gpioXXX"; // NULL terminate default string
static int irqNumber;
static int numberPresses = 0;
static bool ledOn = 0;
static bool isDebounce = 1;	// Used to store the debounce state (default ON)
static struct timespec ts_last, ts_current, ts_diff; // nano precision

// Function prototype for IRQ handler function
static irq_handler_t ebbgpio_irq_handler(unsigned int irq, void *dev_id,
						struct pt_regs *regs);
/** @brief A callback function to output the numberPresses variable
 * @param buf the buffer to which to write the number of presses
 * @param kobj represents a kernel object device that appears in sysfs filesystem
 * @param attr the pointer to the kobj_attribute struct
 * @return return the total number of characters written to the buffer (excluding
 * NULL)
 */
static ssize_t numberPresses_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", numberPresses);
}

/** @brief A callback function to read numberPresses variable
 * @param kobj represents a kernel object device that appears in sysfs filesystem
 * @param attr the pointer to the kobj_attribute struct
 * @param buf The buffer to read the number of presses
 * @param count The number characters in the buffer
 * @return Return total number of characters used from the buffer
 */
static ssize_t numberPresses_store(struct kobject *kobj, 
			struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%du", &numberPresses);

	return count;
}

/** @brief Display if LED is on/off */
static ssize_t ledOn_show(struct kobject *kobj,
					struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", ledOn);
}

/** brief Display the last time the button was pressed
 * manually output the date
 */
static ssize_t lastTime_show(struct kobject *kobj,
					struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%.2lu:%.2lu:%.2lu:%.9lu\n", (ts_last.tv_sec/3600)%24,
		(ts_last.tv_sec/60)%60, (ts_last.tv_sec)%60, ts_last.tv_nsec);
}

/** @brief Display the time difference in form secs.nanosecs to 9 places */
static ssize_t diffTime_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%lu.%.9lu\n", ts_diff.tv_sec, ts_diff.tv_nsec);
}

/** @brief Display if button debouncing is on or off */
static ssize_t isDebounce_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", isDebounce);
}

/** @brief Store and set the debounce state */
static ssize_t isDebounce_store(struct kobject *kobj, struct kobj_attribute *attr,
						const char *buf, size_t count)
{
	unsigned int temp;

	sscanf(buf, "%du", &temp);// use a temp variable for correct int->bool
	gpio_set_debounce(gpioButton, 0);
	isDebounce = temp;
	if (isDebounce) {
		gpio_set_debounce(gpioButton, DEBOUNCE_TIME);
		printk(KERN_INFO "EBB Button: Debounce on\n");
	}
	else {
		gpio_set_debounce(gpioButton, 0); // set the debounce time to 0
		printk(KERN_INFO "EBB Button: Debounce off\n");
	}

	return count;
}

/** Use these helper macros to define the name and access levels of the 
 * kobj_attributes. The kobj_attribue has an attribute attr (name and mode), show
 * and store function pointers
 * The count variable is associated with the numberPresses variable and it is to
 * be exposed with mode 0666 using the numberPresses_show and numberPresses_store
 * functions above.
 */
static struct kobj_attribute count_attr = __ATTR(numberPresses, 0660, numberPresses_show, numberPresses_store);
static struct kobj_attribute debounce_attr = __ATTR(isDebounce, 0660, isDebounce_show, isDebounce_store);

/** The __ATTR_RO macro define a read-only attribute. There is no need to identify
 * that the function is called _show, but it must be present. __ATTR_WO can be used
 * for a write-only attribute but only Linux 3.11.x on.
 */
static struct kobj_attribute ledon_attr = __ATTR_RO(ledOn); // ledon kobject attr
static struct kobj_attribute time_attr = __ATTR_RO(lastTime); // last time pressed
static struct kobj_attribute diff_attr = __ATTR_RO(diffTime); // different time

/** The ebb_attrs[] is an array of attributes that used to create the attribute
 * group. The attr property of the kobj_attribute is used to extract the attribute
 * struct.
 */
static struct attribute *ebb_attrs[] = {
	&count_attr.attr, // number of button presses
	&ledon_attr.attr, // LED ON or OFF
	&time_attr.attr, // last time button press
	&diff_attr.attr, // difference time betwwen last two presses button
	&debounce_attr.attr, // debounce state true or false
	NULL,
};

/** The attribute group use the attribute array and a name, which is exposed on
 * sysfs -- in this case it is gpio115, which is automatic defined in the 
 * ebbButton_init() using the custom kernel parameter that can be passed when
 * module is loaded.
 */
static struct attribute_group attr_group = {
		.name = gpioName, // The name is generated in ebbButton_init()
		.attrs = ebb_attrs, // The attributes array defined above
};

static struct kobject *ebb_kobj;

/** @brief The LKM init function.
 * This function sets up the GPIOs and the IRQ
 */
static int __init ebbButton_init(void)
{
	int result = 0;
	unsigned long IRQflags = IRQF_TRIGGER_RISING; // default is rising-edge

	printk(KERN_INFO "EBB Button: Initializing the EBB Button LKM\n");

	// create the gpio115 name for /sys/ebb/gpio115
	sprintf(gpioName, "gpio%d", gpioButton);

	// create the kobject sysfs entry at /sys/ebb -- not an ideal location!
	// kernel_kobj point to /sys/kernel
	ebb_kobj = kobject_create_and_add("ebb", kernel_kobj->parent);
	if (!ebb_kobj) {
		printk(KERN_ALERT "EBB Button: failed create kobject mapping\n");
		return -ENOMEM;
	}

	/* add the attributes to /sys/ebb/ 
	 * for example, /sys/ebb/gpio115/numberPresses
	 */
	result = sysfs_create_group(ebb_kobj, &attr_group);
	if(result) {
		printk(KERN_ALERT "EBB Button: Failed to create sysfs group\n");
		kobject_put(ebb_kobj); // clean up -- remove kobject sysfs entry

		return result;
	}
	getnstimeofday(&ts_last); // set the last time to be the current time
	ts_diff = timespec_sub(ts_last, ts_last); // set init time difference to 0

	// Going to set up LED: GPIO output mode, on by default
	ledOn = true;
	gpio_request(gpioLED, "sysfs");
	gpio_direction_output(gpioLED, ledOn); // set output mode, led on
	//gpio_set_value(gpioLED, ledOn); // not neccessary
	/* export gpio
	 * cause gpio49 to appear in /sys/class/gpio
	 * the bool argument prevents the direction being changed
	 */
	gpio_export(gpioLED, false);

	gpio_request(gpioButton, "sysfs");
	gpio_direction_input(gpioButton);
	// Debounce the button with delay 200ms
	gpio_set_debounce(gpioButton, DEBOUNCE_TIME);
	gpio_export(gpioButton, false); // gpio115 will appear /sys/class/gpio

	// Test to see that the button is working as expected on LKM load
	printk(KERN_INFO "EBB Button: the button state is currently: %d\n",
					gpio_get_value(gpioButton));
	/* GPIO numbers and IRQ numbers are not the same!
	 * This function performs the mapping for us
	 */
	irqNumber = gpio_to_irq(gpioButton);
	printk(KERN_INFO "The button is mapped to IRQ: %d\n", irqNumber);

	if (!isRising) {
		IRQflags = IRQF_TRIGGER_FALLING;
	}

	// This next call requests an interrupt line
	result = request_irq(irqNumber, // The intterupt number requested
				// The pointer to handler function
				(irq_handler_t) ebbgpio_irq_handler,
			     	IRQflags, // rising edge
			     	"ebb_gpio_handler", // used in /proc/interrupts
			     	NULL);
	printk(KERN_INFO "GPIO_TEST: The interrupt request resut is: %d\n",result);
	
	return result;
}

/** @brief The LKM cleanup function */
static void __exit ebbButton_exit(void)
{
	printk(KERN_INFO "EBB Button: The button was pressed %d times\n", 
								numberPresses);
	kobject_put(ebb_kobj); // clean up -- remove the kobject sysfs entry
	gpio_set_value(gpioLED, 0);	// Turn LED OFF
	gpio_unexport(gpioLED);
	// Free IRQ number, no *dev_id required in this case
   	free_irq(irqNumber, NULL);
   	gpio_unexport(gpioButton);	// Unexport the Button GPIO
   	gpio_free(gpioLED);		// Free the LED GPIO
   	gpio_free(gpioButton);		// Free the Button GPIO
   	printk(KERN_INFO "EBB Button: Goodbye from the EBB Button LKM!\n");
}

/** @brief The GPIO IRQ handler function */
static irq_handler_t ebbgpio_irq_handler(unsigned int irq, void *dev_id, 
						struct pt_regs *regs)
{
	ledOn = !ledOn;
	gpio_set_value(gpioLED, ledOn);
   	getnstimeofday(&ts_current);	// Get the current time as ts_current
	// Determine time different between last 2 presses
   	ts_diff = timespec_sub(ts_current, ts_last);
   	ts_last = ts_current; // Store the current time as the last time ts_last
   	printk(KERN_INFO "EBB Button: The button state is currently: %d\n", 
						gpio_get_value(gpioButton));
   	numberPresses++;

	// Announce that the IRQ has been handled correctly
   	return (irq_handler_t) IRQ_HANDLED;
}

module_init(ebbButton_init);
module_exit(ebbButton_exit);
