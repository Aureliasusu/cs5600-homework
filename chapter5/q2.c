#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main() {
    int fd = open("q2_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd < 0) {
        printf("open failed\n");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        printf("fork failed\n");
        return 1;
    }

    if (pid == 0) {
        write(fd, "Child writes this line.\n", 24);
    } else {
        write(fd, "Parent writes this line.\n", 25);
    }

    close(fd);
    return 0;
}
