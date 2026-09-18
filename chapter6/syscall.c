#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

#define ITERATIONS 1000000

double time_diff(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1000000.0
           + (end.tv_usec - start.tv_usec);
}

int main() {
    struct timeval start, end;

    // 1. Measure back-to-back gettimeofday()
    gettimeofday(&start, NULL);

    for (int i = 0; i < ITERATIONS; i++) {
        struct timeval temp;
        gettimeofday(&temp, NULL);
    }

    gettimeofday(&end, NULL);

    double timer_total = time_diff(start, end);
    printf("gettimeofday total time: %.2f us\n", timer_total);
    printf("Average gettimeofday cost: %.6f us\n",
           timer_total / ITERATIONS);

    // 2. Measure a 0-byte read system call
    int fd = open("/dev/null", O_RDONLY);

    if (fd < 0) {
        perror("open");
        return 1;
    }

    char buffer;

    gettimeofday(&start, NULL);

    for (int i = 0; i < ITERATIONS; i++) {
        read(fd, &buffer, 0);
    }

    gettimeofday(&end, NULL);

    double syscall_total = time_diff(start, end);

    printf("\n0-byte read total time: %.2f us\n", syscall_total);
    printf("Average 0-byte read cost: %.6f us\n",
           syscall_total / ITERATIONS);

    close(fd);

    return 0;
}
