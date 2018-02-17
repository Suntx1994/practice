#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/hrtimer.h>

asmlinkage int set_rsv(pid_t pid, struct timespec *C, struct timespec *T);
asmlinkage int cancel_rsv(pid_t pid);
