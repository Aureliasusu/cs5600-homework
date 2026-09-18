#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int x = 100;

    pid_t pid = fork();

    if (pid < 0) {
        printf("fork failed\n");
        return 1;
    }

    if (pid == 0) {
        printf("Child: x before change = %d\n", x);
        x = 200;
        printf("Child: x after change = %d\n", x);
    } else {
        printf("Parent: x before change = %d\n", x);
        x = 300;
        printf("Parent: x after change = %d\n", x);
    }

    return 0;
}
