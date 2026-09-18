#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdlib.h>

#define ITERATIONS 100000

double time_diff(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1000000.0
           + (end.tv_usec - start.tv_usec);
}

void pin_to_cpu0() {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    if (sched_setaffinity(0, sizeof(set), &set) == -1) {
        perror("sched_setaffinity");
        exit(1);
    }
}

int main() {
    int pipe1[2];
    int pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    char byte = 'x';

    if (pid == 0) {
        // Child
        pin_to_cpu0();

        close(pipe1[1]);
        close(pipe2[0]);

        for (int i = 0; i < ITERATIONS; i++) {
            read(pipe1[0], &byte, 1);
            write(pipe2[1], &byte, 1);
        }

        close(pipe1[0]);
        close(pipe2[1]);

        return 0;

    } else {
        // Parent
        pin_to_cpu0();

        close(pipe1[0]);
        close(pipe2[1]);

        struct timeval start, end;

        gettimeofday(&start, NULL);

        for (int i = 0; i < ITERATIONS; i++) {
            write(pipe1[1], &byte, 1);
            read(pipe2[0], &byte, 1);
        }

        gettimeofday(&end, NULL);

        wait(NULL);

        double total = time_diff(start, end);

        double round_trip = total / ITERATIONS;

        // One round trip contains approximately two context switches
        double context_switch = round_trip / 2.0;

        printf("Total time: %.2f us\n", total);
        printf("Average round-trip time: %.6f us\n", round_trip);
        printf("Estimated context switch cost: %.6f us\n",
               context_switch);

        close(pipe1[1]);
        close(pipe2[0]);
    }

    return 0;
}
