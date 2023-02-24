// import various kernel headers that are required for the module to function correctly.
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

// macros provide metadata about the module, including its license and description
MODULE_LICENSE("GPL");
MODULE_AUTHOR("student");
MODULE_DESCRIPTION("Imitation Device Callbacks - Linux Kernel Module");

// create a character buffer and a pointer to the current position in the buffer.
static char buffer[255];
static int buffer_pointer = 0;

// create the device number, class, and structure that are used to register the device driver.
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

// create constants for the driver name and class.
#define DRIVER_NAME "dummydriver"
#define DRIVER_CLASS "MyModuleClass"


//  implement the basic functionality of reading from and writing to the character buffer, and logging when the device is opened and closed
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta;

	to_copy = min(count, buffer_pointer);

	not_copied = copy_to_user(user_buffer, buffer, to_copy);

	delta = to_copy - not_copied;

	return delta;
}

static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta;

	to_copy = min(count, sizeof(buffer));

	not_copied = copy_from_user(buffer, user_buffer, to_copy);
	buffer_pointer = to_copy;

	delta = to_copy - not_copied;

	return delta;
}

static int driver_open(struct inode *device_file, struct file *instance) {
	printk("dev_nr - open was called!\n");
	return 0;
}

static int driver_close(struct inode *device_file, struct file *instance) {
	printk("dev_nr - close was called!\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.read = driver_read,
	.write = driver_write
};

// function initializes the device driver by allocating a device number, creating a class, creating a device file, and registering the device driver with the kernel.
static int __init ModuleInit(void) {
	int retval;
	printk("Hello, Kernel!\n");

	if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
		printk("Device Nr. could not be allocated!\n");
		return -1;
	}
	printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);

	if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
		printk("Device class can not be created!\n");
		goto ClassError;
	}

	if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) {
		printk("Can not create device file!\n");
		goto FileError;
	}

	cdev_init(&my_device, &fops);

	if(cdev_add(&my_device, my_device_nr, 1) == -1) {
		printk("Registering of device to kernel failed!\n");
		goto AddError;
	}

	return 0;
AddError:
	device_destroy(my_class, my_device_nr);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_nr, 1);
	return -1;
}

// function unregisters the device driver and cleans up any resources that were allocated during initialization.
static void __exit ModuleExit(void) {
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_nr, 1);
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
