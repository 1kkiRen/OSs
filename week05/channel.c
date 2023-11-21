#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define MAX_MSG_SIZE 1024

int main(void) {
    pid_t pid;
    int fd[2];
    char message[MAX_MSG_SIZE];

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(fd[0]);

        printf("Enter a message: ");
        fgets(message, MAX_MSG_SIZE, stdin);

        write(fd[1], message, strlen(message) + 1);

        close(fd[1]);

        exit(0);
    } else {
        close(fd[1]);

        read(fd[0], message, MAX_MSG_SIZE);
        printf("Message: %s", message);

        close(fd[0]);

        exit(0);
    }

    return 0;
}