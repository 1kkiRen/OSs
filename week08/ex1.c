#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>

#define PASS_LEN 13

int main() {
    FILE *pid_file = fopen("/tmp/ex1.pid", "w");
    fprintf(pid_file, "%d", getpid());
    fclose(pid_file);

    char *pass = malloc(PASS_LEN);

    for (int i = 0; i < 5; i++) {
        pass[i] = "pass:"[i];
    }

    int random_fd = open("/dev/urandom", O_RDONLY);
    if (random_fd < 0) {
        perror("open");
        exit(1);
    }

    for (int i = 5; i < PASS_LEN; i++) {
        char c;
        do {
            read(random_fd, &c, 1);
        } while (!isprint(c));
        pass[i] = c;
    }

    close(random_fd);

    while (1) {
        sleep(1);
    }

    return 0;
}
