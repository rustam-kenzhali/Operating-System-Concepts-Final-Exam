#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("student");
MODULE_DESCRIPTION("Hello World - Linux Kernel Module");

/*
module has two functions defined as __init and __exit which 
respectively are called when the module is loaded and unloaded from the kernel
*/

// function prints "Hello World!" to the kernel log using the printk() function and returns 0 to indicate that the module loaded successfully
static int __init hello_init(void)
{
  printk(KERN_INFO "Hello World!\n");
  return 0;
}


// function prints "Goodbye World!" to the kernel log
static void __exit hello_exit(void)
{
  print(KERN_INFO "Goodbye World!\n");
}

module_init(hello_init);
module_exit(hello_exit);
