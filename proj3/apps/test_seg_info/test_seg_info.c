#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define show_segment_info 400
#define show_vm_areas 401
#define BUFFER_SIZE 135168

int main(int argc, char* argv[]) {
    int res;
    int pid;
    int i, n;
    if (argc < 2) {
        pid = (int)getpid();
    }
    else {
        pid = atoi(argv[1]);
    }
    n = BUFFER_SIZE;
    char *buf = malloc(n * sizeof(char));
    char *buf2 = malloc(n * sizeof(char));
    if (mlock(buf, n)) {
        free(buf);
        printf("Memory lock failed\n");
    }
    for (i = 0; i < n; i += 4096) {
        buf[i] = 1;
        buf2[i] = 1;
    }
    // res = syscall(show_segment_info, pid);
    res = syscall(show_vm_areas, pid);
    if (res == -1) {
        printf("cannot access process %d\n", pid);
    }
    else {
        printf("Succeed.\n");
    }
    munlock(buf, n);
    free(buf);
    free(buf2);
    return 0;
}