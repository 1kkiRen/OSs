/*
The worker process (worker.c) in the main function will read its
idx as a command line argument. Then it registers the same
handler signal handler for all three signals (SIGTSTP,
SIGCONT, SIGTERM). The worker process generates a number n
using big n function. In an infinite loop, the worker checks the
candiadate for the next triangular number next n using
is triangular, when it finds the triangular number next n, it
needs to increments the counter tris and then checks again for the
next number (next n + 1) till termination by the scheduler.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>

// 6 digits for big triangular numbers like 113050
#define TRI_BASE 1000000

pid_t pid;

int process_idx;

long tris;

bool is_triangular(long n){
    for (long i = 1; i <= n; i++){
      if (i * (i + 1) == 2 * n){
         return true;
      }
   }
   return false;
}

void signal_handler(int signum){
	printf("Process %d (PID=<%d>): count of triangulars found so far is \e[0;31m%ld\e[0m\n", process_idx, pid, tris);

	switch(signum) {
    case SIGTSTP:
        printf("Process %d: stopping....\n", process_idx);
        pause();
        break;
    case SIGCONT:
        printf("Process %d: resuming....\n", process_idx);
        break;
    case SIGTERM:
        printf("Process %d: terminating....\n", process_idx);
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
	}
}

long big_n() {
    time_t t;
    long n = 0;
    srand((unsigned) time(&t));
    while(n < TRI_BASE)
        n += rand();
    return n % TRI_BASE;
}

int main(int argc, char *argv[]){
    process_idx = atoi(argv[1]);
    
    pid = getpid();

    signal(SIGTSTP, signal_handler);
    signal(SIGCONT, signal_handler);
    signal(SIGTERM, signal_handler);

    
    long next_n = big_n() + 1;

    printf("Process %d (PID=<%d>): has been started\n", process_idx, pid); 
    printf("Process %d (PID=<%d>): will find the next trianguar number from [%ld, inf)\n", process_idx, pid, next_n);

    tris = 0;


    while (true){
        if (is_triangular(next_n)){
            printf("Process %d (PID=<%d>): I found this triangular number \e[0;31m%ld\e[0m\n", process_idx, pid, next_n);
            tris++;
        }

      next_n++;
    }
  
}