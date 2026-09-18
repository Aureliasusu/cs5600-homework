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
        printf("Child: my PID is %d\n", getpid());

        pid_t result = wait(NULL);

        printf("Child: wait() returned %d\n", result);
    } else {
        printf("Parent: waiting for child %d\n", pid);

        pid_t result = wait(NULL);

        printf("Parent: wait() returned %d\n", result);
        printf("Parent: child has finished\n");
    }

    return 0;
}
