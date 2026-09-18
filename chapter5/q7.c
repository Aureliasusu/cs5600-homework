#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("fork failed\n");
        return 1;
    }

    if (pid == 0) {
        printf("Child: before closing stdout\n");
        fflush(stdout);

        close(STDOUT_FILENO);

        printf("Child: after closing stdout\n");
        fflush(stdout);
    } else {
        wait(NULL);
        printf("Parent: child has finished\n");
    }

    return 0;
}
