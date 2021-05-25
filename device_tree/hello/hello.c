#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>

static const struct of_device_id hello_table[] = {
	{ .compatible = "hello_world", },
	{ /* end of list */ },
};
MODULE_DEVICE_TABLE(of, hello_table);

static int my_probe(struct platform_device *dev)
{
	printk(KERN_EMERG "Hello world\n");

	return 0;
}

static struct platform_driver my_platform_driver = {
        .probe = my_probe,
        .driver = {
                .name = "my_driver",
                .owner = THIS_MODULE,
                .of_match_table = hello_table,
        },
};

int init_module(void)
{
	printk(KERN_EMERG "%s, %d\n", __func__, __LINE__);

	platform_driver_register(&my_platform_driver);

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_EMERG "%s, %d\n", __func__, __LINE__);

	platform_driver_unregister(&my_platform_driver);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Phu Luu An");
MODULE_DESCRIPTION("hello world kernel module");
