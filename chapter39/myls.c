#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

void print_long(const char *dir, const char *name) {
    char path[4096];
    snprintf(path, sizeof(path), "%s/%s", dir, name);
    struct stat st;
    if (stat(path, &st) != 0) { perror(path); return; }

    char perms[11];
    perms[0] = S_ISDIR(st.st_mode) ? 'd' : S_ISLNK(st.st_mode) ? 'l' : '-';
    perms[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';
    perms[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';
    perms[7] = (st.st_mode & S_IROTH) ? 'r' : '-';
    perms[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (st.st_mode & S_IXOTH) ? 'x' : '-';
    perms[10] = '\0';

    struct passwd *pw = getpwuid(st.st_uid);
    struct group *gr = getgrgid(st.st_gid);
    printf("%s %2lu %-8s %-8s %8ld %s\n",
           perms, (unsigned long)st.st_nlink,
           pw ? pw->pw_name : "?",
           gr ? gr->gr_name : "?",
           (long)st.st_size, name);
}

int main(int argc, char *argv[]) {
    int long_format = 0;
    const char *dir = ".";
    int argi = 1;
    if (argi < argc && strcmp(argv[argi], "-l") == 0) { long_format = 1; argi++; }
    if (argi < argc) dir = argv[argi];

    DIR *dp = opendir(dir);
    if (!dp) { perror(dir); return 1; }

    struct dirent *d;
    while ((d = readdir(dp)) != NULL) {
        if (d->d_name[0] == '.') continue;
        if (long_format) print_long(dir, d->d_name);
        else printf("%s\n", d->d_name);
    }
    closedir(dp);
    return 0;
}
