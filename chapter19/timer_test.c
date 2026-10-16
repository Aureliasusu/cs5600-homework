#include <stdio.h>
#include <sys/time.h>

int main(void) {
    struct timeval start, end;

    gettimeofday(&start, NULL);

    do {
        gettimeofday(&end, NULL);
    } while (end.tv_sec == start.tv_sec &&
             end.tv_usec == start.tv_usec);

    long elapsed =
        (end.tv_sec - start.tv_sec) * 1000000L +
        (end.tv_usec - start.tv_usec);

    printf("Smallest observed timer change: %ld microseconds\n", elapsed);

    return 0;
}
