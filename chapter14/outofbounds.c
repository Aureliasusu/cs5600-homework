#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *data = malloc(100 * sizeof(int));

    if (data == NULL) {
        return 1;
    }

    data[100] = 0;

    printf("Program finished.\n");

    free(data);
    return 0;
}
