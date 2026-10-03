#include <stdio.h>

int main(void) {
    int *ptr = NULL;

    printf("About to dereference a NULL pointer...\n");

    int value = *ptr;

    printf("Value: %d\n", value);

    return 0;
}
