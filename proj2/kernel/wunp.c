#include "wunp.h"
unsigned long timer_interval_ns = 1e6;

enum hrtimer_restart timer_callback(struct hrtimer *timer_for_restart) {
    ktime_t currtime, interval;
    currtime = ktime_get();
    interval = ktime_set(0, timer_interval_ns);
    hrtimer_forward(timer_for_restart, currtime, interval);
    return HRTIMER_RESTART;
}

asmlinkage long wait_until_next_period(void) {

    return 0;
}