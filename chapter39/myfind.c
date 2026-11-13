#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void walk(const char *dir) {
    DIR *dp = opendir(dir);
    if (!dp) { perror(dir); return; }
    struct dirent *d;
    while ((d = readdir(dp)) != NULL) {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) continue;
        char path[4096];
        snprintf(path, sizeof(path), "%s/%s", dir, d->d_name);
        printf("%s\n", path);
        struct stat st;
        if (lstat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
            walk(path);
        }
    }
    closedir(dp);
}

int main(int argc, char *argv[]) {
    const char *root = (argc > 1) ? argv[1] : ".";
    printf("%s\n", root);
    walk(root);
    return 0;
}
