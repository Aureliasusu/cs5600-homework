#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *data = malloc(100 * sizeof(int));

    if (data == NULL) {
        return 1;
    }

    for (int i = 0; i < 100; i++) {
        data[i] = i;
    }

    free(&data[50]);

    printf("Program finished.\n");

    return 0;
}
