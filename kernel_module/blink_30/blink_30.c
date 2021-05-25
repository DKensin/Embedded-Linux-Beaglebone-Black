#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/io.h>
#include <linux/delay.h>

#define GPIO_ADDR_BASE			0X44E07000
#define ADDR_SIZE			0X1000
#define GPIO_SETDATAOUT_OFFSET	0X194
#define GPIO_CLEARDATAOUT_OFFSET	0X190
#define GPIO_OE_OFFSET			0X134
#define VALUE_0				~(1 << 30)
#define VALUE_1				(1 << 30)

void __iomem *base_addr;

int init_module(void)
{
	printk(KERN_EMERG "Hello");

	base_addr = ioremap(GPIO_ADDR_BASE, ADDR_SIZE);

	/* write 0 to GPIO Pin 30 mean config this pin is output */
	writel_relaxed(VALUE_0, base_addr + GPIO_OE_OFFSET);
	
	while (1) {
		/* set high level for GPIO Pin 30 mean ON LED */
		writel_relaxed(VALUE_1, base_addr + GPIO_SETDATAOUT_OFFSET);
		msleep(500);
		writel_relaxed(VALUE_1, base_addr + GPIO_CLEARDATAOUT_OFFSET);
		msleep(500);
	}

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_EMERG "Goodbye");

	/* off led */
	writel_relaxed(VALUE_1, base_addr + GPIO_CLEARDATAOUT_OFFSET);

	/* config default GPIO Pin is input
	 * set Pin 30 to  default
	 */
	writel_relaxed(VALUE_1, base_addr + GPIO_CLEARDATAOUT_OFFSET);

}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Phu Luu An");
MODULE_DESCRIPTION("Hello world kernel module");
