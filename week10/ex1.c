#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>
#include <limits.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))
#define MAX_PATH 1024

char *path;

void find_all_hlinks(const char *source) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char fullpath[MAX_PATH];
    char realpathbuf[MAX_PATH];
    ino_t inode = -1;
    int count = 0;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(1);
    }

    stat(strcat(strcat(strcpy(fullpath, path), "/"), source), &statbuf);
    inode = statbuf.st_ino;


    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        strcpy(fullpath, path);
        strcat(fullpath, "/");
        strcat(fullpath, entry->d_name);

        if (stat(fullpath, &statbuf) == -1) {
            perror("stat");
            exit(1);
        }

        if (S_ISREG(statbuf.st_mode)) {
            if (statbuf.st_ino == inode && strcmp(entry->d_name, source) != 0) {
                count++;
                realpath(fullpath, realpathbuf);
                printf("inode: %ld\t%s\n", statbuf.st_ino, realpathbuf);
            }
        }
    }

    if (count == 0) {
        printf("No hard links found.\n");
        return;
    }

    printf("Total %d hard links found.\n", count);
}

void unlink_all(const char* source) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char fullpath[MAX_PATH];
    ino_t inode = -1;
    int count = 0;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(1);
    }

    stat(strcat(strcat(strcpy(fullpath, path), "/"), source), &statbuf);
    inode = statbuf.st_ino;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        strcpy(fullpath, path);
        strcat(fullpath, "/");
        strcat(fullpath, entry->d_name);

        if (stat(fullpath, &statbuf) == -1) {
            perror("stat");
            exit(1);
        }

        if (S_ISREG(statbuf.st_mode)) {
            if (statbuf.st_ino == inode && strcmp(entry->d_name, source) != 0) {
                count++;
                if (unlink(fullpath) == -1) {
                    perror("unlink");
                    exit(1);
                }
            }
        }
    }

    if (count == 0) {
        printf("No hard links found.\n");
        return;
    }

    printf("Total %d hard links removed.\n", count);
}

void create_sym_link(const char* source, const char* link) {
    if (symlink(source, link) == -1) {
        perror("symlink");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <dir>\n", argv[0]);
        exit(1);
    }

    path = argv[1];

    char *file = malloc(MAX_PATH);
    strcpy(file, path);

    FILE *fp = fopen(strcat(file, "/myfile1.txt"), "w");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    fprintf(fp, "Hello world.\n");
    fclose(fp);
    
    printf("File created.\n");
    printf("%s\n", path);

    char *link11 = malloc(MAX_PATH);
    strcpy(link11, path);
    strcat(link11, "/myfile11.txt");

    if (link(file, link11) == -1) {
        perror("link");
        exit(1);
    }

    printf("Hard link created.\n");

    char *link12  = malloc(MAX_PATH);
    strcpy(link12, path);
    strcat(link12, "/myfile12.txt");

    if (link(file, link12) == -1) {
        perror("link");
        exit(1);
    }

    find_all_hlinks("myfile1.txt");

    char *file_moved = malloc(MAX_PATH);
    strcpy(file_moved, "/tmp/myfile1.txt");

    if (rename(file, file_moved) == -1) {
        perror("rename");
        exit(1);
    }

    printf("File moved.\n");

    FILE *fp2 = fopen(link11, "a");
    if (fp2 == NULL) {
        perror("fopen");
        exit(1);
    }

    fprintf(fp2, "Modified.\n");
    fclose(fp2);

    printf("File modified.\n");

    char *link13 = malloc(MAX_PATH);
    strcpy(link13, path);
    strcat(link13, "/myfile13.txt");

    create_sym_link(file_moved, link13);

    printf("Symbolic link created.\n");

    FILE *fp3 = fopen(file_moved, "a");
    if (fp3 == NULL) {
        perror("fopen");
        exit(1);
    }

    fprintf(fp3, "Modified.\n");
    fclose(fp3);

    printf("File modified.\n");
    
    unlink_all("myfile11.txt");

    printf("Hard links removed.\n");


    find_all_hlinks("myfile11.txt");

    free(file);
    free(link11);
    free(link12);
    free(file_moved);
    free(link13);

    return 0;  
}
