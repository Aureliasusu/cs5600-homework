#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <megabytes>\n", argv[0]);
        return 1;
    }

    int megabytes = atoi(argv[1]);

    if (megabytes <= 0) {
        printf("Please enter a positive number of megabytes.\n");
        return 1;
    }

    long num_ints = ((long)megabytes * 1024 * 1024) / sizeof(int);

    int *array = malloc(num_ints * sizeof(int));

    if (array == NULL) {
        perror("malloc");
        return 1;
    }

    printf("PID: %d\n", getpid());
    printf("Using approximately %d MB of memory.\n", megabytes);

    while (1) {
        for (long i = 0; i < num_ints; i++) {
            array[i] = array[i] + 1;
        }
    }

    free(array);
    return 0;
}
