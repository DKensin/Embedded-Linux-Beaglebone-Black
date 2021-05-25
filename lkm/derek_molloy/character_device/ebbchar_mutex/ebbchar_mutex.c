/**
 * @file   ebbchar.c
 * @author Derek Molloy
 * @date   7 April 2015
 * @version 0.1
 * @brief An introductory character driver to support the second article of 
 * my series on Linux loadable kernel module (LKM) development. This module
 * maps to /dev/ebbchar and comes with a helper C program that can be run in 
 * Linux user-space to communicate with this the LKM.
 */

#include <linux/init.h>	// use to mark up functions: __init __exit
#include <linux/module.h> // Core header for loading LKM into the kernel
#include <linux/device.h> // Header to support the kernel Driver
#include <linux/kernel.h> // Contains types, macros, functions for the kernel
#include <linux/fs.h> // Header for the Linux file system support
#include <linux/uaccess.h> // Required for the copy to user function

#define DEVICE_NAME "ebbchar" // The device will appear at /dev/ebbchar
#define CLASS_NAME "ebb" // The device class will appear /sys/class/ebb

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Derek Molloy");
MODULE_DESCRIPTION("A simple Linux char driver for the BBB");
MODULE_VERSION("0.1");

static int majorNumber; // store the device number
static char message[256]; // string is passed from user-space
static short size_of_message;
static int numberOpens = 0; // Count the number of times the device is opened
static struct class *ebbcharClass = NULL; // Device drvier class struct pointer
static struct device *ebbcharDevice = NULL; // Device driver device struct pointer

static DEFINE_MUTEX(ebbchar_mutex); // Macro to declare a new mutex

// The prototype functios for the character driver
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/** @brief Devices are represented as file structure in the kernel. 
 * The file_operations structure from /linux/fs.h lists the callback functions 
 * that you wish to associated with your file operations  using a C99 syntax 
 * structure. char devices usually implement open, read, write and 
 * release calls
 */
static struct file_operations fops = {
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release,
};

static int __init ebbchar_init(void)
{
	printk(KERN_INFO "EBBChar: Initializing the EBBChar LKM\n");

	// Try to dynamic allocate a major number for the device
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber < 0) {
		printk(KERN_ALERT "EBBChar failed to register a major number\n");
		return majorNumber;
	}
	printk(KERN_INFO "EBBChar: registered correctly with major number %d\n",
							majorNumber);
	// Register the device class
	ebbcharClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(ebbcharClass)) { // Check error and cleanup if there is
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class\n");
		
		// correct way to return an error on a pointer
		return PTR_ERR(ebbcharClass);
	}
	printk(KERN_INFO "EBBChar: device class registered correctly\n");

	// Register the device driver
	ebbcharDevice = device_create(ebbcharClass, NULL, MKDEV(majorNumber,0),
					NULL, DEVICE_NAME);
	if (IS_ERR(ebbcharDevice)) { // clean up if there is an error
		class_destroy(ebbcharClass);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create the device\n");

		return PTR_ERR(ebbcharDevice);
	}
	printk(KERN_INFO "EBBChar: device class created correctly\n");

	mutex_init(&ebbchar_mutex);

	return 0;
}

static void __exit ebbchar_exit(void)
{
	mutex_destroy(&ebbchar_mutex);
	device_destroy(ebbcharClass, MKDEV(majorNumber, 0)); // remove the device
	class_unregister(ebbcharClass); // unregister the device class
	class_destroy(ebbcharClass); // remove the device class
	unregister_chrdev(majorNumber, DEVICE_NAME); // unregister the major number
	printk(KERN_INFO "EBBChar: Goodbye from the LKM\n");
}

/** @brief The device open function that is called each time the device 
 * is opened
 * This will only increment the numberOpens counter in this case.
 * @param inodep A pointer to an inode object (defined in linux/fs.h)
 * @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_open(struct inode *inodep, struct file *filep)
{
	if (!mutex_trylock(&ebbchar_mutex)) { // Try to accquire the mutex
		printk(KERN_ALERT "EBBChar: Device in use by another process");
		
		return -EBUSY;
	}

	numberOpens++;
	printk(KERN_INFO "EBBChar: Device has ben opened %d times\n", 
						numberOpens);

	return 0;
}

/** @brief This function is called whenever device is being read from 
 * user-space i.e. data is being sent from the device to the user. 
 * In this case is uses the copy_to_user() function to send the buffer string 
 * to the user and captures any errors.
 * @param filep A pointer to a file object (defined in linux/fs.h)
 * @param buffer The pointer to the buffer to which this function writes 
 * the data
 * @param len The length of the b
 * @param offset The offset if required
 */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, 
				loff_t *offset)
{
	int error_count = 0;
	
	// copy_to_user has the format ( * to, *from, size), returns 0 if success
   	error_count = copy_to_user(buffer, message, size_of_message);
 
   	if (error_count==0) {            // if true then have success
      		printk(KERN_INFO "EBBChar: Sent %d characters to the user\n", 
					size_of_message);
		// clear position to start and return 0
      		return (size_of_message=0);  
  	 }
   	else {
      		printk(KERN_INFO "EBBChar: Failed to send %d characters to the user\n", error_count);
      		return -EFAULT; // return bad address message
   	}
}

/** @brief This function is called whenever the device is being written to 
 * from user-space i.e. data is sent to the device from the user. The data 
 * is copied to the message[] array in this LKM using the sprintf() function 
 * along with the length of the string.
 *  @param filep A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param len The length of the array of data that is being passed in 
 *  the const char buffer
 *  @param offset The offset if required
 */
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len,
						loff_t *offset)
{
	// appending received string with its length
	sprintf(message, "%s(%zu letters)", buffer, len);
	//copy_from_user(message, buffer, len);
	size_of_message = strlen(message);
	printk(KERN_INFO "EBBchar: Received %zu characters from the user\n", len);

	return len;
}

/** @brief The device release function that is called whenever the device is 
 * closed/released by  the userspace program
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_release(struct inode *inodep, struct file *filep)
{
	mutex_unlock(&ebbchar_mutex); // release the mutex

	printk(KERN_INFO "EBBChar: Device successfully closed\n");

	return 0;
}

module_init(ebbchar_init);
module_exit(ebbchar_exit);
