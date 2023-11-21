#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

int pid;

void sigint_handler(int sig){
    kill(pid, SIGTERM);
    exit(0);
}

int main(){
    FILE *fp = fopen("/mnt/c/Users/ikkiren/Documents/OSs/week06/var/run/agent.pid", "r");
    fscanf(fp, "%d", &pid);
    fclose(fp);

    if(pid == 0){
        printf("Error: No agent found.\n");
        exit(0);
    } else {
        printf("Agent found.\n");
    }

    signal(SIGINT, sigint_handler);

    while(1){
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"} to send to the agent: ");
        char buf[100];
        scanf("%s", buf);

        if(strcmp(buf, "exit") == 0){
            kill(pid, SIGUSR2);
            exit(0);
        } else if(strcmp(buf, "read") == 0){
            kill(pid, SIGUSR1);
        } else if(strcmp(buf, "stop") == 0){
            kill(pid, SIGSTOP);
        } else if(strcmp(buf, "continue") == 0){
            kill(pid, SIGCONT);
        }
    }


    return 0;
}