#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

asmlinkage long wait_until_next_period(void);