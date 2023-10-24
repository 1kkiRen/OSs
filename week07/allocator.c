#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#define MAX_SIZE 10000000

unsigned int memory[MAX_SIZE];

void allocate_first_fit(unsigned int adrs, int size);
void allocate_best_fit(unsigned int adrs, int size);
void allocate_worst_fit(unsigned int adrs, int size);
void clear(unsigned int adrs);
void print_throughput(int num_queries, double time);

int main() {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int num_queries = 0;
    double time = 0;

    struct timespec start, end;


    for(int i = 0; i < 3; i++){
        memset(memory, 0, sizeof
        (memory));

        fp = fopen("queries.txt", "r");
        if (fp == NULL) {
            printf("Error opening file!\n");
            exit(EXIT_FAILURE);
        }

        clock_gettime(CLOCK_MONOTONIC, &start);

        while ((read = getline(&line, &len, fp)) != -1) {
            if (strcmp(line, "end") == 0) {
                break;
            }

            char* token = strtok(line, " ");
            unsigned int adrs = (unsigned int) atoi(strtok(NULL, " "));

            if (strcmp(token, "allocate") == 0) {
                int size = atoi(strtok(NULL, " "));
                if (i == 0) {
                    allocate_first_fit(adrs, size);
                } else if (i == 1) {
                    allocate_best_fit(adrs, size);
                } else if (i == 2) {
                    allocate_worst_fit(adrs, size);
                }
            } else if (strcmp(token, "clear") == 0) {
                clear(adrs);
            }

            num_queries++;
        }


        clock_gettime(CLOCK_MONOTONIC, &end);
        time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        print_throughput(num_queries, time);

        fclose(fp);

        num_queries = 0;
        time = 0;

        if (line) {
            free(line);
        }
    }

    return 0;
}

void allocate_first_fit(unsigned int adrs, int size) {
    int start = -1;
    int end = -1;
    int count = 0;

    for (int i = 0; i < MAX_SIZE; i++) {
        if (memory[i] == 0 && i != MAX_SIZE - 1) {
            if (start == -1) {
                start = i;
            }
            count++;
        } else if (memory[i] != 0 || i == MAX_SIZE - 1) {
            if (memory[i] == 0)
                count++;

            if (count >= size)
                break;
            
            start = -1;
            count = 0;
        }
    }

    if (start != -1 && count >= size) {
        for (int i = start; i < start + size; i++) {
            memory[i] = adrs;
        }
        // printf("Allocated %d bytes at address %d\n", size, start);
    } else {
        // printf("Error: Could not allocate %d bytes\n", size);
    }
}

void allocate_best_fit(unsigned int adrs, int size){
    int start = -1;
    int end = -1;
    int count = 0;
    int best_start = -1;
    int best_count = MAX_SIZE + 1;

    for (int i = 0; i < MAX_SIZE; i++) {
        if (memory[i] == 0 && i != MAX_SIZE - 1) {
            if (start == -1) {
                start = i;
            }
            count++;
        } else if (memory[i] != 0 || i == MAX_SIZE - 1) {
            if (memory[i] == 0) {
                count++;
            }
            if (count >= size && count <= best_count) {
                best_start = start;
                best_count = count;
            }
            start = -1;
            count = 0;
        }
    }

    if (best_start != -1 && best_count >= size) {
        for (int i = best_start; i < best_start + size; i++) {
            memory[i] = adrs;
        }
        // printf("Allocated %d bytes at address %d\n", size, best_start);
    } else {
        // printf("Error: Could not allocate %d bytes\n", size);
    }
}

void allocate_worst_fit(unsigned int adrs, int size) {
    int start = -1;
    int end = -1;
    int count = 0;
    int worst_start = -1;
    int worst_count = 0;

    for (int i = 0; i < MAX_SIZE; i++) {
        if (memory[i] == 0 && i != MAX_SIZE - 1) {
            if (start == -1) {
                start = i;
            }
            count++;
        } else if (memory[i] != 0 || i == MAX_SIZE - 1) {
            if (memory[i] == 0) {
                count++;
            }
            if (count >= size && count >= worst_count) {
                worst_start = start;
                worst_count = count;
            }
            start = -1;
            count = 0;
        }
    }

    if (worst_start != -1 && worst_count >= size) {
        for (int i = worst_start; i < worst_start + size; i++) {
            memory[i] = adrs;
        }
        // printf("Allocated %d bytes at address %d\n", size, worst_start);
    } else {
        // printf("Error: Could not allocate %d bytes\n", size);
    }

    return;
}

void clear(unsigned int adrs) {
    for (int i = 0; i < MAX_SIZE; i++) {
        if (memory[i] == adrs) {
            memory[i] = 0;
        }
    }

    return;
}

void print_throughput(int num_queries, double time) {
    printf("Throughput = %f\n", num_queries / time);
    printf("Time = %f\n\n", time);

    return;
}