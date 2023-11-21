#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>


int main(void){
    struct timeval start, end;
    gettimeofday(&start, NULL);

    pid_t pid1 = fork();
    pid_t pid2 = fork();

    gettimeofday(&end, NULL);

    if(pid1 == 0 && pid2 == 0){
        printf("Child 1: %d\n", getpid());
        printf("Parent 1: %d\n", getppid());
        printf("Child 2: %d\n", getpid());
        printf("Parent 2: %d\n", getppid());
    }else if(pid1 == 0){
        printf("Child 1: %d\n", getpid());
        printf("Parent 1: %d\n", getppid());
    }else if(pid2 == 0){
        printf("Child 2: %d\n", getpid());
        printf("Parent 2: %d\n", getppid());
    }else{
        printf("Main: %d\n", getpid());
        printf("Parent: %d\n", getppid());
    }

    printf("Time: %ld\n", (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec);   

    return 0;
}
