#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/hrtimer.h>

asmlinkage long set_rsv(pid_t pid, struct timespec *C, struct timespec *T);
asmlinkage long cancel_rsv(pid_t pid);
