#include <stdlib.h>
#include <stdio.h>

#define set_rsv 397
#define cancel_rsv 398

int main(int argc, char* argv[]) {
    struct timespec C;
    struct timespec T;
    long res;
    res = syscall(cancel_rsv, getpid());
    if (res == -1) {
        printf("Cancel RSV Failed\n");
    }
    else {
        printf("Cancel RSV Succeed\n");
    }
    C.tv_nsec = 1;
    T.tv_nsec = 10;
    res = syscall(set_rsv, getpid(), &C, &T);
    if (res == -1) {
        printf("Set RSV Failed\n");
    }
    else {
        printf("Set RSV Succeed\n");
    }
    C.tv_nsec = 2;
    T.tv_nsec = 30;
    res = syscall(set_rsv, getpid(), &C, &T);
    if (res == -1) {
        printf("Set RSV Failed\n");
    }
    else {
        printf("Set RSV Succeed\n");
    }
    res = syscall(cancel_rsv, getpid());
    if (res == -1) {
        printf("Cancel RSV Failed\n");
    }
    else {
        printf("Cancel RSV Succeed\n");
    }
    C.tv_nsec = 2;
    T.tv_nsec = 30;
    res = syscall(set_rsv, getpid(), &C, &T);
    if (res == -1) {
        printf("Set RSV Failed\n");
    }
    else {
        printf("Set RSV Succeed\n");
    }
    C.tv_nsec = 2;
    T.tv_nsec = 30;
    res = syscall(set_rsv, getpid(), &C, &T);
    if (res == -1) {
        printf("Set RSV Failed\n");
    }
    else {
        printf("Set RSV Succeed\n");
    }
    return 0;
}