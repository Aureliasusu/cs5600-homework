#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        printf("pipe failed\n");
        return 1;
    }

    pid_t child1 = fork();

    if (child1 < 0) {
        printf("fork failed\n");
        return 1;
    }

    if (child1 == 0) {
        // Child 1: send stdout into the pipe
        close(pipefd[0]);

        dup2(pipefd[1], STDOUT_FILENO);

        close(pipefd[1]);

        execlp("ls", "ls", NULL);

        return 1;
    }

    pid_t child2 = fork();

    if (child2 < 0) {
        printf("fork failed\n");
        return 1;
    }

    if (child2 == 0) {
        // Child 2: get stdin from the pipe
        close(pipefd[1]);

        dup2(pipefd[0], STDIN_FILENO);

        close(pipefd[0]);

        execlp("wc", "wc", "-l", NULL);

        return 1;
    }

    // Parent does not use the pipe
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);

    return 0;
}
