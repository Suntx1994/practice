#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define set_rsv 397
#define cancel_rsv 398
#define wait_until_next_period 399

int main(int argc, char* argv[]) {
    struct timespec C;
    struct timespec T;
    long res;
    int i, j;
    long long exe_1, exe_2, diff, budget;
    struct timespec t_1, t_2;

    printf("Testing the reservation...\n");
    C.tv_sec = atoi(argv[1]);
    C.tv_nsec = atoi(argv[2]);
    T.tv_sec = atoi(argv[3]);
    T.tv_nsec = atoi(argv[4]);
    // res = syscall(set_rsv, 2, &C, &T);
    // res = syscall(set_rsv, 6, &C, &T);
    // res = syscall(set_rsv, 23, &C, &T);
    res = syscall(set_rsv, getpid(), &C, &T);
    if (res == -1) {
        printf("Set RSV Failed\n");
    }
    else {
        printf("Set RSV Succeed\n");
    }
    printf("Testing the budget\n");
    budget = ((long long)C.tv_sec + 2LL) * 1000000000LL + (long long)C.tv_nsec;
    for (i = 0; i < 3; i++) {
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t_1);
        exe_1 = (long long)t_1.tv_sec * 1000000000LL + (long long)t_1.tv_nsec;
        printf("Running...\n");
        while (1) {
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t_2);
            exe_2 = (long long)t_2.tv_sec * 1000000000LL + (long long)t_2.tv_nsec;
            diff = exe_2 - exe_1;
            if (diff > budget) {
                printf("Running out of budget...\n");
                break;
            }
        }
        printf("before suspend..\n");
        res = syscall(wait_until_next_period);
        printf("woke up\n");
    }
    
    res = syscall(cancel_rsv, getpid());
    if (res == -1) {
        printf("Cancel RSV Failed\n");
    }
    else {
        printf("Cancel RSV Succeed\n");
    }
    return 0;
}