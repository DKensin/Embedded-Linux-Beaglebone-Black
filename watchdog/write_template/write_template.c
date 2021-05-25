/* viet template de su dung duoc device tree */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/reboot.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/moduleparam.h>
#include <linux/pm_runtime.h>
#include <linux/watchdog.h>

static int bbb_wdt_probe(struct platform_device *pdev)
{
	printk(KERN_INFO "PhuLA %s %d\n", __func__, __LINE__);

	return 0;
}

static const struct of_device_id bbb_wdt_of_match[] = {
	{.compatible = "my_ti,omap3-wdt",},
	{},
};

MODULE_DEVICE_TABLE(of, bbb_wdt_of_match);

static struct platform_driver bbb_wdt_driver = {
	.probe		= bbb_wdt_probe,
	.driver		= {
				.name = "bbb_wdt",
				.owner = THIS_MODULE,
				.of_match_table = bbb_wdt_of_match,
			  },
};
module_platform_driver(bbb_wdt_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Phu Luu An");
MODULE_DESCRIPTION("hello world kernel module");
