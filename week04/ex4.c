#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX 100

int main(void){
    char input[MAX];
    char *args[MAX];
    int i = 0;
    int status;

    while(1){
        printf(">> ");
        fgets(input, MAX, stdin);
        input[strlen(input) - 1] = '\0';

        if(strcmp(input, "exit") == 0){
            break;
        }

        char *token = strtok(input, " ");
        while(token != NULL){
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        pid_t pid = fork();

        if(pid == 0){
            execvp(args[0], args);
            exit(0);
        } else {
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}