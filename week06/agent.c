#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void sig_handler(int signo){
    if (signo == SIGUSR1){
        FILE *fp = fopen("/mnt/c/Users/ikkiren/Documents/OSs/week06/text.txt", "r");
        char buf[1024];
        while(fgets(buf, 1024, fp) != NULL){
            printf("%s", buf);
        }
        fclose(fp);

    } else if(signo == SIGUSR2){
        printf("Process terminating...\n");
        exit(0);
    }
}

int main(){
    FILE *fp = fopen("/mnt/c/Users/ikkiren/Documents/OSs/week06/var/run/agent.pid", "w");
    fprintf(fp, "%d", getpid());
    fclose(fp);

    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);

    while(1){
        sleep(1);
    }
    return 0;
}