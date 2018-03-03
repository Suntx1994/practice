#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define show_segment_info 400

int main(int argc, char* argv[]) {
    int res;
    int pid;
    if (argc < 2) {
        pid = (int)getpid();
    }
    else {
        pid = atoi(argv[1]);
    }
    res = syscall(show_segment_info, pid);
    if (res == -1) {
        printf("process %d not found.\n", pid);
    }
    else {
        printf("Succeed.\n");
    }
    return 0;
}