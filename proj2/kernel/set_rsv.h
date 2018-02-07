#include <linux/kernel.h>
#include <linux/unistd.h>
asmlinkage int set_rsv(pid_t pid, struct timespec *C, struct timespec *T);