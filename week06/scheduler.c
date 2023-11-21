#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#define PS_MAX 10

typedef struct{
	int idx; 
	int at, bt, rt, wt, ct, tat;
	int burst;
} ProcessData;


int running_process = -1;


unsigned total_time; 

ProcessData data[PS_MAX];


pid_t ps[PS_MAX];

unsigned data_size;

void read_file(FILE* file){
    if (file == NULL) {
        printf("File is not found or cannot open it!\n"); 
        exit(EXIT_FAILURE);
    } else {
        int i = 0;

        char idx[4], at[3], bt[3];
        fscanf(file, "%s %s %s", idx, at, bt);
        while(fscanf(file, "%d %d %d", &data[i].idx, &data[i].at, &data[i].bt) != EOF) {
            data[i].burst = data[i].bt;
            ps[i] = 0;
            i++;
        }

        data_size = i;
    }
    fclose(file);

}

void resume(pid_t process) {
    if (process > 0 && ps[running_process] != 0) {
        kill(process, SIGCONT);
    } else {
        printf("Scheduler: No running process.\n");
    }
    
}

void suspend(pid_t process) {
    if (process > 0 && ps[running_process] != 0) {
        kill(process, SIGTSTP);
    } else {
        printf("Scheduler: No running process.\n");
    }
}

void terminate(pid_t process) {
    if (process > 0 && ps[running_process] != 0) {
        kill(process, SIGTERM);
    } else {
        printf("Scheduler: No running process.\n");
    }
}


void create_process(int new_process) {
    if (running_process != -1){
        printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
        suspend(ps[running_process]);
    }

    pid_t pid = fork();

    if (pid == 0) {
        char new_process_str[32];
        sprintf(new_process_str, "%d", new_process);
        char *args[] = {"./worker", new_process_str, NULL};
        execvp(args[0], args);
        exit(1);
    } else if (pid > 0) {
        running_process = new_process;
        ps[running_process] = pid;
        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", new_process, data[new_process].burst);
    } else{
        printf("Scheduler: Fork failed.\n");
        exit(1);
    }

}

ProcessData find_next_process() {
	int location = -1;

	for(int i=0; i < data_size; i++) {
        if (data[i].burst > 0) {
            if (location == -1) {
                location = i;
            } else {
                if (data[i].at < data[location].at) {
                    location = i;
                }
            }
        }
        
	}

	if(data[location].at > total_time || location == -1){
        
        printf("Scheduler: Runtime: %u seconds.\nProcess %d: has not arrived yet.\n", total_time, location);
        
        total_time++;
        return find_next_process(); 
	}

	return data[location];
}

void report(){
	printf("Simulation results.....\n");
	int sum_wt = 0;
	int sum_tat = 0;
	for (int i=0; i< data_size; i++){
		printf("process %d: \n", i);
		printf("	at=%d\n", data[i].at);
		printf("	bt=%d\n", data[i].bt);
		printf("	ct=%d\n", data[i].ct);
		printf("	wt=%d\n", data[i].wt);
		printf("	tat=%d\n", data[i].tat);
		printf("	rt=%d\n", data[i].rt);
		sum_wt += data[i].wt;
		sum_tat += data[i].tat;
	}

	printf("data size = %d\n", data_size);
	float avg_wt = (float)sum_wt/data_size;
	float avg_tat = (float)sum_tat/data_size;
	printf("Average results for this run:\n");
	printf("	avg_wt=%f\n", avg_wt);
	printf("	avg_tat=%f\n", avg_tat);
}

void check_burst(){

	for(int i = 0; i < data_size; i++)
		if (data[i].burst > 0)
		    return;

    report();

	exit(EXIT_SUCCESS);
}

void schedule_handler(int signum) {
    total_time++;

    if (running_process != -1) {
        data[running_process].burst--;
        printf("Scheduler: Runtime: %u seconds\nProcess %d is running with %d seconds left\n", total_time, running_process, data[running_process].burst);

        if (data[running_process].burst == 0) {
            printf("Scheduler: Terminating Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            terminate(ps[running_process]);
            waitpid(ps[running_process], NULL, 0);
            data[running_process].ct = total_time;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
            data[running_process].rt = data[running_process].ct - data[running_process].at;
            running_process = -1;
        }
    }

    check_burst();

    ProcessData next_process = find_next_process();

    if (ps[next_process.idx] == 0) {
        if (running_process != -1) {
            printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            suspend(ps[running_process]);
        }
        running_process = next_process.idx;
        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
        create_process(running_process);
        data[running_process].rt = total_time - data[running_process].at;
    } else {
        if (running_process != -1) {
            printf("Scheduler: Resuming Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            resume(ps[running_process]);
        }
    }

}

int main(int argc, char *argv[]) {
    FILE *in_file  = fopen("data.txt", "r");
  	if (in_file == NULL) {   
		printf("File is not found or cannot open it!\n"); 
  		exit(EXIT_FAILURE);
    } else {
        read_file(in_file);  
    }

    struct itimerval timer;

    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, NULL);

    signal(SIGALRM, schedule_handler);

    while(1);
}