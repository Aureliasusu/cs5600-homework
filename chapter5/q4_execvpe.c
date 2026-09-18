#define _GNU_SOURCE
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
        printf("Child is about to run ls using execvpe\n");

        char *args[] = {"ls", "-l", NULL};

        char *env[] = {
            "MYVAR=hello",
            "PATH=/bin:/usr/bin",
            NULL
        };

        execvpe("ls", args, env);

        printf("execvpe failed\n");
    } else {
        printf("Parent continues running\n");
    }

    return 0;
}
