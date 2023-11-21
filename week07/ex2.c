#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILE_SIZE 500 * 1024 * 1024

int main() {
    int fd = open("text.txt", O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    double time = 0;

    ftruncate(fd, FILE_SIZE);

    char *map = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        printf("Error mapping file!\n");
        exit(EXIT_FAILURE);
    }

    char *random = "/dev/random";
    int random_fd = open(random, O_RDONLY);
    if (random_fd < 0) {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE); 
    }

    char c;
    int i = 0;
    while (i < FILE_SIZE) {
        read(random_fd, &c, 1);
        if (isprint(c)) {
            map[i] = c;
            i++;
        }
    }

    long cs = sysconf(_SC_PAGESIZE);
    int count = 0;
    for (int i = 0; i < FILE_SIZE; i += cs) {
        for (int j = i; j < i + cs; j++) {
            if (isupper(map[j])) {
                count++;
                map[j] = tolower(map[j]);
            }
        }
    }

    printf("Number of capital letters: %d\n", count);

    munmap(map, FILE_SIZE);
    close(fd);
    close(random_fd);
   
    return 0;
}
