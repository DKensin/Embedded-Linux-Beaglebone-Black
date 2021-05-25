#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/moduleparam.h>
#include <linux/pm_runtime.h>
#include <linux/i2c.h>

static int i2c_driver_probe_new(struct i2c_client *client)
{
	printk(KERN_INFO "Phu LA %s %d\n", __func__, __LINE__);

	return 0;
}

static int i2c_driver_remove(struct i2c_client *client)
{
	printk(KERN_INFO "Phu LA %s %d\n", __func__, __LINE__);

	return 0;
}

static struct i2c_device_id i2c_driver_device_id[] = {
	{.name = "i2c_driver_test",},
	{ }
};

static struct of_device_id i2c_driver_of_id[] = {
	{.compatible = "lcd_16x2",},
	{ }
};

static struct i2c_driver i2c_driver = {
	.probe_new	= i2c_driver_probe_new,
	.remove		= i2c_driver_remove,
	.id_table	= i2c_driver_device_id,
	.driver		= {
		.name		= "bbb_i2c_driver",
		.of_match_table	= i2c_driver_of_id,
	},
};

module_i2c_driver(i2c_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Phu Luu An");
MODULE_DESCRIPTION("I2C Kernel Module");
