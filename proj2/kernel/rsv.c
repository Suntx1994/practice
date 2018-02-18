#include "rsv.h"

// static struct hrtimer hr_timer;

asmlinkage long set_rsv(pid_t pid, struct timespec *C, struct timespec *T) {
    struct task_struct *cur_task = find_task_by_pid_ns(pid, &init_pid_ns);
    if (!cur_task->is_rsv_valid) {
        cur_task->C = C;
        cur_task->T = T;
        cur_task->is_rsv_valid = true;
        pr_info("rsv succeeded for process %d", (int)pid);
    }
    else {
        pr_info("resource for process %d has been reserved before", (int)pid);
        return -1;
    }
    return 0;
}

asmlinkage long cancel_rsv(pid_t pid) {
    struct task_struct *cur_task = find_task_by_pid_ns(pid, &init_pid_ns);
    if (cur_task->is_rsv_valid) {
        cur_task->C = NULL;
        cur_task->T = NULL;
        cur_task->is_rsv_valid = false;
        pr_info("rsv for p: %d has been canceled", (int)pid);
    }
    else {
        pr_info("Invalid instruction");
        return -1;
    }
    return 0;
}