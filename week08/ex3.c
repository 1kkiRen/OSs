#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <string.h>

#define SIZE 10 * 1024 * 1024

int main() {
    for (int i = 0; i < 10; i++) {
        void *memory = malloc(SIZE);
        memset(memory, 0, SIZE);
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        printf("Memory usage: %ld\n", usage.ru_maxrss);
        sleep(1);
    }
    return 0;
}