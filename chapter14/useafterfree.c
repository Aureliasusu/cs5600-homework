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

    free(data);

    printf("data[10] = %d\n", data[10]);

    return 0;
}
