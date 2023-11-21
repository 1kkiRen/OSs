#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define MAX_MSG_SIZE 1024

int main(int argc, char *argv[]) {
    int id = atoi(argv[1]);
    if(id <= 0){
        fprintf(stderr, "id must be a positive integer\n");
        exit(1);
    }

    char pipe_name[250] = "";
    while(1){
        sprintf(pipe_name, "/tmp/ex1/s%d", id);
        FILE *fd = fopen(pipe_name, "r");

        char message[MAX_MSG_SIZE] = "";
        fgets(message, MAX_MSG_SIZE, fd);

        fclose(fd);

        printf("\nSubscriber %d\n", id);
        printf("%s\n", message);
        sleep(3);
    }

    return 0;
}