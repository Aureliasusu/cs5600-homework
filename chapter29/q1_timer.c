#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int main() {
    int n = 10000;
    long min_us = 1000000, total_us = 0;
    struct timeval tv1, tv2;

    for (int i = 0; i < n; i++) {
        gettimeofday(&tv1, NULL);
        gettimeofday(&tv2, NULL);
        long diff = (tv2.tv_sec - tv1.tv_sec) * 1000000L
                  + (tv2.tv_usec - tv1.tv_usec);
        if (diff > 0 && diff < min_us) min_us = diff;
        total_us += diff;
    }
    printf("gettimeofday: min=%ld us, avg=%ld us\n", min_us, total_us / n);

    long min_ns = 1000000000L, total_ns = 0;
    struct timespec ts1, ts2;
    for (int i = 0; i < n; i++) {
        clock_gettime(CLOCK_MONOTONIC, &ts1);
        clock_gettime(CLOCK_MONOTONIC, &ts2);
        long diff = (ts2.tv_sec - ts1.tv_sec) * 1000000000L
                  + (ts2.tv_nsec - ts1.tv_nsec);
        if (diff > 0 && diff < min_ns) min_ns = diff;
        total_ns += diff;
    }
    printf("clock_gettime: min=%ld ns, avg=%ld ns\n", min_ns, total_ns / n);

    return 0;
}
