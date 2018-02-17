#include "rsv.h"

// static struct hrtimer hr_timer;

asmlinkage int set_rsv(pid_t pid, struct timespec *C, struct timespec *T) {
    struct task_struct *cur_task = find_task_by_pid_ns(pid, &init_pid_ns);
    if (cur_task->C == NULL) {
        cur_task->C = C;
        cur_task->T = T;
        printk(KERN_INFO "rsv succeeded for process %d", (int)pid);
    }
    else {
        printk(KERN_ERR "resource for process %d has been reserved before", (int)pid);
        return -1;
    }
    return 0;
}

asmlinkage int cancel_rsv(pid_t pid) {
    struct task_struct *cur_task = find_task_by_pid_ns(pid, &init_pid_ns);
    if (cur_task->C == NULL) {
        return -1;
    }
    else {
        cur_task->C = NULL;
        cur_task->T = NULL;
    }
    return 0;
}