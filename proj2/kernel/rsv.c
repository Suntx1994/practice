#include "rsv.h"

ktime_t currtime, interval;
struct task_struct *cur_task;

enum hrtimer_restart timer_callback(struct hrtimer *timer_for_restart) {
    if (!cur_task->is_rsv_valid) {
        return HRTIMER_NORESTART;
    }
    wake_up_process(cur_task);
    currtime = ktime_get();
    interval = timespec_to_ktime(timer_for_restart->T);
    hrtimer_forward(timer_for_restart, currtime, interval);
    if (ktime_compare(cur_task->total_execution_time, timespec_to_ktime(cur_task->C)) > 0) {
        s64 total_ms, budget_ms;
        int util;
        total_ms = ktime_to_ms(cur_task->total_execution_time);
        budget_ms = ktime_to_ms(timespec_to_ktime(cur_task->C));
        util = ((int)total_ms / (int)budget_ms) * 100;
        printk("Task %d: budget overrun (util: %d %%)", (int)cur_task->pid, util);     
    }
    cur_task->total_execution_time = ktime_set(0, 0);
    return HRTIMER_RESTART;
}

asmlinkage long set_rsv(pid_t pid, struct timespec *C, struct timespec *T) {
    cur_task = find_task_by_pid_ns(pid, &init_pid_ns);
    if (!cur_task->is_rsv_valid) {
        cur_task->C = *C;
        cur_task->T = *T;
        cur_task->is_rsv_valid = true;
        hrtimer_init(&cur_task->rsv_hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_PINNED);
        cur_task->rsv_hr_timer.function = &timer_callback;
        cur_task->rsv_hr_timer.T = *T;
        hrtimer_start(&cur_task->rsv_hr_timer, interval, HRTIMER_MODE_PINNED);
        pr_info("rsv succeeded for p: %d", (int)pid);
    }
    else {
        pr_info("resource for p: %d has been reserved before", (int)pid);
        return -1;
    }
    return 0;
}

asmlinkage long cancel_rsv(pid_t pid) {
    int ret;
    struct task_struct *cur_task = find_task_by_pid_ns(pid, &init_pid_ns);
    if (cur_task->is_rsv_valid) {
        // cur_task->C = NULL;
        // cur_task->T = NULL;
        cur_task->is_rsv_valid = false;
        ret = hrtimer_cancel(&cur_task->rsv_hr_timer);
        if (ret) { 
            printk("rsv for p: %d has been canceled (was active)\n", (int)pid);
        }
        else {
            printk("rsv for p: %d has been canceled", (int)pid);
        }
    }
    else {
        pr_info("There's no reservation for p: %d", (int)pid);
        return -1;
    }
    return 0;
}

asmlinkage long wait_until_next_period(void) {
    __set_current_state(1);
    schedule();
    return 0;
}
