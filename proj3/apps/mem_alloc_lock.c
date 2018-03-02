#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[]) {
    struct timespec t1, t2;
    long i;
    long long t1_nsec, t2_nsec;
    long n = atoi(argv[1]);
    char *buf = malloc(n * sizeof(char));
    if (mlock(buf, n)) {
        free(buf);
        printf("Memory lock failed\n");
    }
    clock_gettime(CLOCK_REALTIME, &t1);
    for (i = 0; i < n; i += 4096) {
        buf[i] = 1;
    }
    clock_gettime(CLOCK_REALTIME, &t2);
    t1_nsec = (long long)t1.tv_sec * 1000000000LL + (long long)t1.tv_nsec;
    t2_nsec = (long long)t2.tv_sec * 1000000000LL + (long long)t2.tv_nsec;

    printf("Total memory access time: %lld nsec\n", t2_nsec - t1_nsec);
    munlock(buf, n);
    free(buf);
    return 0;
}