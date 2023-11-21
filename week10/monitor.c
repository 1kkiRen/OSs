#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

char *path;

void print_stat(char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    printf("File: %s\n", path);
    printf("Size: %ld\tBlocks: %ld\tIO Block: %ld\t", statbuf.st_size, statbuf.st_blocks, statbuf.st_blksize);
    printf("Device: %ld\tInode: %ld\tLinks: %ld\n", statbuf.st_dev, statbuf.st_ino, statbuf.st_nlink);
    printf("Access: (%o/", statbuf.st_mode);
    if (statbuf.st_mode & S_IRUSR)
        printf("r");
    else
        printf("-");
    if (statbuf.st_mode & S_IWUSR)
        printf("w");
    else
        printf("-");
    if (statbuf.st_mode & S_IXUSR)
        printf("x");
    else
        printf("-");
    if (statbuf.st_mode & S_IRGRP)
        printf("r");
    else
        printf("-");
    if (statbuf.st_mode & S_IWGRP)
        printf("w");
    else
        printf("-");
    if (statbuf.st_mode & S_IXGRP)
        printf("x");
    else
        printf("-");
    if (statbuf.st_mode & S_IROTH)
        printf("r");
    else
        printf("-");
    if (statbuf.st_mode & S_IWOTH)
        printf("w");
    else
        printf("-");
    if (statbuf.st_mode & S_IXOTH)
        printf("x");
    else
        printf("-");
    printf(")\tUid: %d\tGid: %d\n", statbuf.st_uid, statbuf.st_gid);
    printf("Access: %s", ctime(&statbuf.st_atime));
    printf("Modify: %s", ctime(&statbuf.st_mtime));
    printf("Change: %s", ctime(&statbuf.st_ctime));
}

void print_dir(char *path) {
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        char *new_path = malloc(strlen(path) + strlen(entry->d_name) + 2);
        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, entry->d_name);
        print_stat(new_path);
        free(new_path);
    }
    closedir(dir);
}

int print_event(struct inotify_event *event) {
    if (event->mask & IN_ACCESS) {
        printf("File was accessed\n");
        return 0;
    }
    if (event->mask & IN_CREATE) {
        printf("File/directory created in watched directory\n");
        return 0;
    }
    if (event->mask & IN_DELETE) {
        printf("File/directory deleted from watched directory\n");
        return 1;
    }
    if (event->mask & IN_MODIFY) {
        printf("File was modified\n");
        return 0;
    }
    if (event->mask & IN_OPEN) {
        printf("File or directory was opened\n");
        return 0;
    }
    if (event->mask & IN_ATTRIB) {
        printf("Metadata changed\n");
        return 0;
    }
    
    return 1;
}

void sigint_handler(int sig) {
    printf("SIGINT received\n");

    print_dir(path);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    path = argv[1];

    print_dir(path);

    signal(SIGINT, sigint_handler);

    int fd = inotify_init();

    if (fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int wd = inotify_add_watch(fd, path, IN_ALL_EVENTS);
    if (wd == -1) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    char buf[BUF_LEN];
    int len, i = 0;
    while ((len = read(fd, buf, BUF_LEN)) > 0) {
        i = 0;
        while (i < len) {
            struct inotify_event *event = (struct inotify_event *) &buf[i];
            int mode = print_event(event);
            char *new_path = malloc(strlen(path) + strlen(event->name) + 2);
            strcpy(new_path, path);
            strcat(new_path, "/");
            strcat(new_path, event->name);
            if(mode == 0)
                print_stat(new_path);
            free(new_path);
            i += EVENT_SIZE + event->len;
        }
        
    }
    return 0;
}