/**
 * @file   gpio_test.c
 * @author Derek Molloy
 * @date   19 April 2015
 * @brief  A kernel module for controlling a GPIO LED/button pair. The device 
 * mounts devices via sysfs /sys/class/gpio/gpio115 and gpio49. Therefore, this 
 * test LKM circuit assumes that an LED is attached to GPIO 49 which is on P9_23 
 * and the button is attached to GPIO 115 on P9_27. There is no requirement for 
 * a custom overlay, as the pins are in their default mux mode states.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>		// Required for the GPIO functions
#include <linux/interrupt.h>	// Required for the IRQ code

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Derek Molloy");
MODULE_DESCRIPTION("A Button/LED test driver for the BBB");
MODULE_VERSION("0.1");

static unsigned int gpioLED = 49; // GPIO P1_17
static unsigned int gpioButton = 115; // GPIO P3_19
static unsigned int irqNumber;
static unsigned int numberPresses = 0; // Number of button presses
static bool ledOn = 0; // Is LED ON/OFF? Used to invert its state, default OFF

// Function prototype for custom IRQ handler function
static irq_handler_t ebbgpio_irq_handler(unsigned int irq, void *dev_id,
						struct pt_regs *regs);

/** @brief LKM init function
 * This function sets up the GPIOs and the IRQ
 */
static int __init ebbgpio_init(void)
{
	int result = 0;

	printk(KERN_INFO "GPIO_TEST: Initializing the GPIO_TEST LKM\n");

	// Check the GPIO number is valid?
	if (!gpio_is_valid(gpioLED)) {
		printk(KERN_INFO "GPIO_TEST: invalid LED GPIO\n");

		return -ENODEV;
	}

	// Going to set up the LED: GPIO output mode, default: ON
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
	gpio_set_debounce(gpioButton, 200);
	gpio_export(gpioButton, false); // gpio115 will appear /sys/class/gpio

	// Test to see that the button is working as expected on LKM load
	printk(KERN_INFO "GPIO_TEST: the button state is currently: %d\n",
					gpio_get_value(gpioButton));
	/* GPIO numbers and IRQ numbers are not the same!
	 * This function performs the mapping for us
	 */
	irqNumber = gpio_to_irq(gpioButton);
	printk(KERN_INFO "The button is mapped to IRQ: %d\n", irqNumber);

	// This next call requests an interrupt line
	result = request_irq(irqNumber, // The intterupt number requested
				// The pointer to handler function
				(irq_handler_t) ebbgpio_irq_handler,
			     	IRQF_TRIGGER_RISING, // rising edge
			     	"ebb_gpio_handler", // used in /proc/interrupts
			     	NULL);
	printk(KERN_INFO "GPIO_TEST: The interrupt request resut is: %d\n",result);
	
	return result;
}

/** @brief The LKM cleanup function
 * Used to release the GPIOs and display cleanup messages
 */
static void __exit ebbgpio_exit(void)
{
	printk(KERN_INFO "GPIO_TEST: The button state is currently: %d\n", 
					gpio_get_value(gpioButton));
	printk(KERN_INFO "GPIO_TEST: The button was pressed %d times\n",
							numberPresses);
	gpio_set_value(gpioLED, 0); // Turn off LED

	gpio_unexport(gpioLED);
	free_irq(irqNumber, NULL); // no *dev_id required in this case
	gpio_unexport(gpioButton);

	gpio_free(gpioLED);
	gpio_free(gpioButton);

	printk(KERN_INFO "GPIO_TEST: Goodbye from the LKM\n");
}

/** @brief The GPIO IRQ handler function
 * @param irq The irq number is associated with the GPIO
 * @param dev_id the *dev_id that is provided -- can be used to identify which
 * device caused the interrupt -- not used in this example as NULL is passed
 * @param regs h/w specific register values -- only really ever used for debugging
 * return IRQ_HANDLED if success
 */
static irq_handler_t ebbgpio_irq_handler(unsigned int irq, void *dev_id,
					 struct pt_regs *regs)
{
	ledOn = !ledOn; // Invert the LED state on each button press
	gpio_set_value(gpioLED, ledOn);
	printk(KERN_INFO "GPIO_TEST: Interrupt! (button state is %d)\n",
						gpio_get_value(gpioButton));
	numberPresses++;

	return (irq_handler_t) IRQ_HANDLED; // Announce IRQ handled correctly
}

module_init(ebbgpio_init);
module_exit(ebbgpio_exit);
