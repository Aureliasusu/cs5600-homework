#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("fork failed\n");
        return 1;
    }

    if (pid == 0) {
        printf("Child is about to run ls using execlp\n");

        execlp("ls", "ls", "-l", NULL);

        printf("execlp failed\n");
    } else {
        printf("Parent continues running\n");
    }

    return 0;
}
