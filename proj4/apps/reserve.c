#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define set_rsv 397
#define cancel_rsv 398

int main(int argc, char* argv[]) {
    struct timespec C;
    struct timespec T;
    long res;
    pid_t pid;
    C.tv_nsec = atoi(argv[2])*1000;
    T.tv_nsec = atoi(argv[3])*1000;
    if (atoi(argv[1]) == 0) {
        pid = getpid();
    }
    else {
        pid = atoi(argv[1]);
    }
    res = syscall(set_rsv, pid, &C, &T, atoi(argv[4]));
    if (res == -1) {
        printf("Set RSV Failed\n");
    }
    else {
        printf("Set RSV Succeeded\n");
    }
    return 0;
}