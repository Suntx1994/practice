#include "rsv.h"

static struct hrtimer hr_timer;
unsigned long timer_interval_ns = 1e6;

enum hrtimer_restart timer_callback(struct hrtimer *timer_for_restart) {
    ktime_t currtime, interval;
    currtime = ktime_get();
    interval = ktime_set(0, timer_interval_ns);
    hrtimer_forward(timer_for_restart, currtime, interval);
    return HRTIMER_RESTART;
}

asmlinkage long set_rsv(pid_t pid, struct timespec *C, struct timespec *T) {
    struct task_struct *cur_task = find_task_by_pid_ns(pid, &init_pid_ns);
    if (!cur_task->is_rsv_valid) {
        cur_task->C = C;
        cur_task->T = T;
        cur_task->is_rsv_valid = true;
        ktime = ktime_set(C->tv_sec, C->tv_nsec);
        hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_PINNED);
        hr_timer.function = &timer_callback;
        hrtimer_start(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_PINNED);
        pr_info("rsv succeeded for p: %d", (int)pid);
    }
    else {
        pr_info("resource for p: %d has been reserved before", (int)pid);
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
        int ret;
        ret = hrtimer_cancel(&hrtimer);
        if (ret) { 
            printk("The timer was still in use...\n");
        }
        else {
            pr_info("rsv for p: %d has been canceled", (int)pid);
        }
    }
    else {
        pr_info("There's no reservation for p: %d", (int)pid);
        return -1;
    }
    return 0;
}

asmlinkage long wait_until_next_period(void) {

    return 0;
}
