// include the necessary header files for the Linux kernel
#include <linux/module.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("student");
MODULE_DESCRIPTION("Current Timer - Linux Kernel Module");

// used to set up a new timer
static struct hrtimer mytimer;
u64 start_t;

// function calculates the time difference between the start time and the current time, which is done by using the jiffies function that returns the number of clock ticks since the system started.
static enum hrtimer_restart test_hrtimer_handler(struct hrtimer *timer) {
  u64 now_t = jiffies;
  printk("start_t - now_t = %u\n", jiffies_to_msecs(now_t - start_t));
  return HRTIMER_NORESTART;
}

// sets up the HRTimer
static int __init ModuleInit(void) {
  printk("Hello, LKM!\n");
  hrtimer_init(&mytimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
  mytimer.function = &test_hrtimer_handler;
  start_t = jiffies;
  hrtimer_start(&mytimer, ms_to_ktime(100), HRTIMER_MODE_REL);
  return 0;
}

// function is called when the module is removed, and it cancels the timer by calling hrtimer_cancel and prints a goodbye message.
static void __exit ModuleExit(void) {
  hrtimer_cancel(&mytimer);
  printk("Goodbye, LKM\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
