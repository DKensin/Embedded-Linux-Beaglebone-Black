/* viet virtual memory de su dung duoc device tree */

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

struct bbb_wdt_dev {
	struct watchdog_device wdog;
	void __iomem    *base;          /* virtual */
	struct device   *dev;
	bool            is_active_wdt;
	int             wdt_trigg_val;
	struct mutex    lock;
};

static const struct watchdog_info omap_wdt_info = {
	.options = WDIOF_SETTIMEOUT | WDIOF_MAGICCLOSE | WDIOF_KEEPALIVEPING,
	.identity = "BBB Watchdog",
};

static int bbb_wdt_start(struct watchdog_device *wdog)
{
	printk(KERN_INFO "PhuLA %s %d\n", __func__, __LINE__);

	return 0;
}

static int bbb_wdt_stop(struct watchdog_device *wdog)
{
	printk(KERN_INFO "PhuLA %s %d\n", __func__, __LINE__);
	
	return 0;
}

static int bbb_wdt_ping(struct watchdog_device *wdog)
{
	printk(KERN_INFO "PhuLA %s %d\n", __func__, __LINE__);

	return 0;
}

static int bbb_wdt_set_timeout(struct watchdog_device *wdog, unsigned int timeout)
{
	printk(KERN_INFO "PhuLA %s %d\n", __func__, __LINE__);

	return 0;
}

static const struct watchdog_ops bbb_wdt_ops = {
	.owner          = THIS_MODULE,
	.start          = bbb_wdt_start,
	.stop           = bbb_wdt_stop,
	.ping           = bbb_wdt_ping,
	.set_timeout    = bbb_wdt_set_timeout,
};

static int bbb_wdt_probe(struct platform_device *pdev)
{
	struct bbb_wdt_dev *wdev = NULL;

	printk(KERN_INFO "PhuLA %s %d\n", __func__, __LINE__);

	wdev = devm_kzalloc(&pdev->dev, sizeof(*wdev), GFP_KERNEL);
	if (wdev == NULL)
		return -ENOMEM;

	wdev->wdog.parent = &pdev->dev;
	wdev->wdog.ops = &bbb_wdt_ops;
	wdev->wdog.info = &omap_wdt_info;
	wdev->wdog.min_timeout = 1;
	wdev->wdog.max_timeout = 3600;
	wdev->wdog.status = WATCHDOG_NOWAYOUT_INIT_STATUS;

	watchdog_register_device(&wdev->wdog);

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
				.of_match_table = bbb_wdt_of_match,
			  },
};

module_platform_driver(bbb_wdt_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Phu Luu An");
MODULE_DESCRIPTION("hello world kernel module");
