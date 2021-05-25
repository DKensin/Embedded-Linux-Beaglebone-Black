#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_DESCRIPTION("Simple Misc Driver");
MODULE_AUTHOR("Nick Glynn <n.s.glynn@gmail.com>");
MODULE_LICENSE("GPL");

bool first_open = false;
bool on = true;

#define MAGIC_NO	500
#define ON_OFF_CMD	_IOW(MAGIC_NO, 1, char*)

static int sample_open(struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "PhuLA %s, %d\n", __func__, __LINE__);

	if (true != on)
		return -1;
	
	first_open = true;

	return 0;
}

static int sample_close(struct inode *inodep, struct file *filep)
{
	printk(KERN_EMERG "PhuLA %s, %d\n", __func__, __LINE__);

	return 0;
}

static ssize_t sample_read(struct file *file, char __user *buf, size_t len,
								loff_t *ppos)
{
	printk(KERN_EMERG "PhuLA %s, %d\n", __func__, __LINE__);

	if (true == first_open) {
		copy_to_user(buf, "Hello world\n", strlen("Hello world\n"));
		first_open = false;

		return strlen("Hello world\n");
	}

	return 0;
}

static ssize_t sample_write(struct file *file, const char __user *buf, size_t len,
								loff_t *ppos)
{
	printk(KERN_EMERG "PhuLA %s, %d\n", __func__, __LINE__);

	return 0;
}

static long sample_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	char config = 0;

	switch (cmd) {
		case ON_OFF_CMD:
			copy_from_user(&config, (char *)arg, 1);
			printk(KERN_EMERG "PhuLA config = %c\n", config);
			if ('1' == config)
				on = true;
			else
				on = false;
			break;
		default:
			return -EINVAL;
	}

	return 0;
}

static const struct file_operations sample_fops = {
	.owner			= THIS_MODULE,
	.write			= sample_write,
	.read			= sample_read,
	.open			= sample_open,
	.release		= sample_close,
	.unlocked_ioctl		= sample_ioctl,
};

struct miscdevice sample_device = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name = "simple_misc",
	.fops = &sample_fops,
};

static int __init misc_init(void)
{
	printk(KERN_EMERG "PhuLA %s, %d\n", __func__, __LINE__);
	misc_register(&sample_device);

	return 0;
}

static void __exit misc_exit(void)
{
	printk(KERN_EMERG "PhuLA %s, %d\n", __func__, __LINE__);
	misc_deregister(&sample_device);
}

module_init(misc_init);
module_exit(misc_exit);
