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
        sleep(2);
        printf("Child: finished\n");
    } else {
        printf("Parent: waiting specifically for child %d\n", pid);

        pid_t result = waitpid(pid, NULL, 0);

        printf("Parent: waitpid() returned %d\n", result);
        printf("Parent: child has finished\n");
    }

    return 0;
}
