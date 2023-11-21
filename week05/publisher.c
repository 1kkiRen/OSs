#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_MSG_SIZE 1024

int main(int argc, char *argv[]){
    int n = atoi(argv[1]);

    char pipe_name[250] = "";
    sprintf(pipe_name, "/tmp/ex1/s%d", n);
    FILE *fd = fopen(pipe_name, "w");
    fclose(fd);

    while(1){
        char message[MAX_MSG_SIZE] = "";
        printf("Enter a message: ");
        fgets(message, MAX_MSG_SIZE, stdin);

        for (int i = 0; i < n; i++) {
            pid_t pid = fork();
            if(pid == 0){
                char pipe_name[250] = "";
                sprintf(pipe_name, "/tmp/ex1/s%d", i + 1);
                FILE *fd = fopen(pipe_name, "w");
                fprintf(fd, "%s", message);
                fflush(fd);
                fclose(fd);
                exit(0);
            }
        }

        for(int i = 0; i < n; i++){
            wait(NULL);
        }
    }

    return 0;
}
