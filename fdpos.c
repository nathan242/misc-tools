#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

int dir_exists(const char filepath[])
{
    struct stat path_stat;
    stat(filepath, &path_stat);

    // Is it a dir?
    if (S_ISDIR(path_stat.st_mode)) {
        return 1;
    } else {
        return 0;
    }
}

long file_size(const char filepath[])
{
    long filesz;
    FILE *fp = fopen(filepath, "r");

    if (fp == NULL) {
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    filesz = ftell(fp);
    fclose(fp);

    return filesz;
}

long get_file_pos(const char fdinfo[]) {
    long filepos;
    char buf[512];
    FILE *fp = fopen(fdinfo, "r");

    if (fp == NULL) {
        return 0;
    }

    while (fgets(buf, 512, fp) != NULL) {
        if (strncmp(buf, "pos:", 4) == 0) {
            filepos = atoi(buf+5);
        }
    }

    fclose(fp);

    return filepos;
}

void show_bar(long size, long pos) {
    const int units = 50;
    float percentage;
    float sizef = size/1.0f;
    float posf = pos/1.0f;
    int count;

    int a;
    int b;

    percentage = posf/sizef;

    count = percentage*units;

    //printf("%.8f\n", percentage);
    //printf("%i\n", count);

    putchar('[');
    b = 0;
    for (a = 0; a < units; a++) {
        if (b++ < count) {
            putchar('#');
        } else {
            putchar('-');
        }
    }
    putchar(']');
}

int main(int argc, char *argv[])
{
    char pid[6];
    char path[12] = "/proc/";
    char fdpath[16];
    char fdinfo[20];
    char tpath[2048];
    char linkdest[2048];
    ssize_t linksz;
    long filesz;
    long filepos;

    struct dirent *dp;

    FILE *fp;

    if (argc < 2) {
        fputs("No PID specified.\n", stderr);
        return 1;
    }

    if (strlen(argv[1]) > 5) {
        fputs("PID is too large.\n", stderr);
        return 1;
    }

    strcpy(pid, argv[1]);

    if (!dir_exists(path)) {
        fputs("/proc not found.\n", stderr);
        return 2;
    }

    strcat(path, pid);
    strcpy(fdpath, path);
    strcat(fdpath, "/fd/");
    strcpy(fdinfo, path);
    strcat(fdinfo, "/fdinfo/");

    if (!dir_exists(path)) {
        fputs("PID not found.\n", stderr);
        return 3;
    }

    DIR *fddir = opendir(fdpath);
    while ((dp = readdir(fddir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            strcpy(tpath, fdpath);
            strcat(tpath, dp->d_name);

            memset(linkdest, 0, sizeof(linkdest));
            linksz = readlink(tpath, linkdest, sizeof(linkdest));

            filesz = file_size(linkdest);

            strcpy(tpath, fdinfo);
            strcat(tpath, dp->d_name);
            filepos = get_file_pos(tpath);

            show_bar(filesz, filepos);
            printf(" %s -> %s [%i:%i]\n", dp->d_name, linkdest, filesz, filepos);
        }
    }

    closedir(fddir);

    return 0;
}

