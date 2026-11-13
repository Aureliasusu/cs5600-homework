#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define CHUNK 4096

int main(int argc, char *argv[]) {
    int n = 10;
    const char *path = NULL;
    int argi = 1;
    if (argi < argc && strcmp(argv[argi], "-n") == 0) {
        n = atoi(argv[argi + 1]);
        argi += 2;
    }
    if (argi < argc) path = argv[argi];
    if (!path) { fprintf(stderr, "usage: %s [-n N] file\n", argv[0]); return 1; }

    int fd = open(path, O_RDONLY);
    if (fd < 0) { perror(path); return 1; }

    struct stat st;
    fstat(fd, &st);
    off_t size = st.st_size;
    if (size == 0) { close(fd); return 0; }

    char buf[CHUNK];
    int lines = 0;
    off_t pos = size;
    int found = 0;
    off_t print_start = 0;

    while (pos > 0 && !found) {
        off_t to_read = (pos >= CHUNK) ? CHUNK : pos;
        pos -= to_read;
        lseek(fd, pos, SEEK_SET);
        if (read(fd, buf, to_read) != to_read) { perror("read"); close(fd); return 1; }
        for (off_t i = to_read - 1; i >= 0; i--) {
            if (buf[i] == '\n') {
                lines++;
                if (lines > n) {
                    print_start = pos + i + 1;
                    found = 1;
                    break;
                }
            }
        }
    }

    lseek(fd, print_start, SEEK_SET);
    char c;
    while (read(fd, &c, 1) == 1) putchar(c);
    close(fd);
    return 0;
}
