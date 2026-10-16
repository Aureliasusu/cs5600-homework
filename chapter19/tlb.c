#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <num_pages> <num_trials>\n", argv[0]);
        return 1;
    }

    int num_pages = atoi(argv[1]);
    long num_trials = atol(argv[2]);

    if (num_pages <= 0 || num_trials <= 0) {
        printf("Both arguments must be positive.\n");
        return 1;
    }

    int page_size = getpagesize();
    int jump = page_size / sizeof(int);

    /*
     * volatile prevents the compiler from optimizing away
     * the memory accesses in the main timing loop.
     */
    volatile int *a =
        calloc((size_t)num_pages * jump, sizeof(int));

    if (a == NULL) {
        perror("calloc");
        return 1;
    }

    /*
     * Touch each page once before timing.
     * This helps avoid measuring first-touch initialization costs.
     */
    for (int i = 0; i < num_pages * jump; i += jump) {
        a[i] = 0;
    }

    struct timeval start, end;

    gettimeofday(&start, NULL);

    for (long t = 0; t < num_trials; t++) {
        for (int i = 0; i < num_pages * jump; i += jump) {
            a[i] += 1;
        }
    }

    gettimeofday(&end, NULL);

    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;

    double elapsed_us =
        seconds * 1000000.0 + microseconds;

    long total_accesses =
        num_trials * (long)num_pages;

    double ns_per_access =
        (elapsed_us * 1000.0) / total_accesses;

    printf(
        "pages=%d trials=%ld time_per_access=%.3f ns\n",
        num_pages,
        num_trials,
        ns_per_access
    );

    free((void *)a);

    return 0;
}
