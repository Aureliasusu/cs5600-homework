#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }
    struct stat st;
    if (stat(argv[1], &st) != 0) {
        perror("stat");
        return 1;
    }
    printf("File: %s\n", argv[1]);
    printf("Size: %ld bytes\n", (long)st.st_size);
    printf("Blocks: %ld\n", (long)st.st_blocks);
    printf("Inode: %lu\n", (unsigned long)st.st_ino);
    printf("Links: %lu\n", (unsigned long)st.st_nlink);
    printf("Mode: %o\n", st.st_mode);
    printf("UID: %d\n", st.st_uid);
    printf("GID: %d\n", st.st_gid);
    return 0;
}
