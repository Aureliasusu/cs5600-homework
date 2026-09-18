#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int pipefd[2];

    pipe(pipefd);

    pid_t pid = fork();

    if (pid < 0) {
        printf("fork failed\n");
        return 1;
    }

    if (pid == 0) {
        close(pipefd[0]);

        printf("hello\n");

        write(pipefd[1], "x", 1);
        close(pipefd[1]);
    } else {
        char buffer;

        close(pipefd[1]);

        read(pipefd[0], &buffer, 1);

        printf("goodbye\n");

        close(pipefd[0]);
    }

    return 0;
}
