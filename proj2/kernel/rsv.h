#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

asmlinkage long set_rsv(pid_t pid, struct timespec *C, struct timespec *T);
asmlinkage long cancel_rsv(pid_t pid);
asmlinkage long wait_until_next_period(void);
