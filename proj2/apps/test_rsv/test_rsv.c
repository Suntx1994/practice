#include <stdlib.h>
#include <stdio.h>

#define set_rsv 397
#define cancel_rsv 398
#define wait_until_next_period 399

int main(int argc, char* argv[]) {
    struct timespec C;
    struct timespec T;
    long res;
    int i;
    C.tv_nsec = 5;
    C.tv_sec = 6;
    T.tv_nsec = 7;
    T.tv_sec = 2;

    res = syscall(set_rsv, getpid(), &C, &T);
    if (res == -1) {
        printf("Set RSV Failed\n");
    }
    else {
        printf("Set RSV Succeed\n");
    }
    for (i = 0; i < 5; i++) {
        printf("Running...\n");
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