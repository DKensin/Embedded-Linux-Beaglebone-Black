#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Phu Luu An");
MODULE_DESCRIPTION("Example use timer in kernel linux");

struct timer_list my_timer;

static void my_func(unsigned long data)
{
	printk(KERN_INFO "Your timer expired and app has been called\n");
	mod_timer(&my_timer, jiffies + HZ);
}

int init_module(void)
{
	unsigned int delay = 0; // delay in s

	delay = 5 * HZ;		// convert second to number of ticks
	
	printk(KERN_INFO "Hello world\n");

	init_timer(&my_timer);
	my_timer.expires = jiffies + delay;
	my_timer.function = my_func;
	add_timer(&my_timer);

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world\n");

	del_timer(&my_timer);
}
