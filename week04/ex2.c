#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


int main(int argc, char *argv[]){
    srand(time(NULL));
    int u[120], v[120];
    int n, sum = 0;
    FILE *fp;
    fp = fopen("temp.txt", "w");

    if(fp == NULL){
        printf("Error opening file!\n");
        exit(1);
    }

    for(int i = 0; i < 120; i++){
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }

    printf("Enter the number of processes: ");
    
    scanf("%d", &n);
    int step = 120 / n;

    pid_t pid[n];

    for(int i = 0; i < n; i++){
        pid[i] = fork();

        if(pid[i] == 0){
            int temp = 0;
            for(int j = i * step; j < (i + 1) * step; j++){
                temp += u[j] * v[j];
            }
            fprintf(fp, "%d\n", temp);
            fclose(fp);
            exit(0);
        }
    }

    for(int i = 0; i < n; i++){
        waitpid(pid[i], NULL, 0);
    }

    fp = fopen("temp.txt", "r");
    for(int i = 0; i < n; i++){
        int temp;
        fscanf(fp, "%d", &temp);
        sum += temp;
    }

    fclose(fp);
    
    printf("Dot product: %d\n", sum);

    return 0;
}
