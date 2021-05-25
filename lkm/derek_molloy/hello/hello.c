#include <linux/init.h>	// Used to mark up functions e.g., __init __exit
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kernel.h> // Contains types, macros, functions for the kernel

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Derek Molloy");
MODULE_DESCRIPTION("A simple Linux driver for the BBB.");
MODULE_VERSION("0.1");

static char *name = "world";  // example LKM argument
module_param(name, charp, S_IRUGO); //charp = char ptr, S_IRUGO only can be read
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

static int __init helloBBB_init(void)
{
	printk(KERN_INFO "EBB: Hello %s from the BBB LKM!\n", name);

	return 0;
}

static void __exit helloBBB_exit(void)
{
	printk(KERN_INFO "EBB: Goodbye %s from the BB LKM\n", name);
}


module_init(helloBBB_init);
module_exit(helloBBB_exit);
