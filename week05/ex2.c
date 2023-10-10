#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_MSG_SIZE 256

struct Thread {
    pthread_t id;
    int i;
    char message[MAX_MSG_SIZE];
};

void *thread_func(void *arg) {
    struct Thread *thread = (struct Thread *) arg;
    printf("Thread %d is created\n", thread->i);
    printf("Thread %d prints message: %s\n", thread->i, thread->message);
    printf("Thread %d exits\n\n", thread->i);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "n must be a positive integer\n");
        exit(EXIT_FAILURE);
    }

    struct Thread threads[n];
    for (int i = 0; i < n; i++) {
        threads[i].i = i;
        sprintf(threads[i].message, "Hello from thread %d", i);
        pthread_create(&threads[i].id, NULL, thread_func, &threads[i]);
        pthread_join(threads[i].id, NULL);
    }

    return EXIT_SUCCESS;
}