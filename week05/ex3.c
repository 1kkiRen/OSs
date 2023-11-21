#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>


bool is_prime (int n) {
    if (n <= 1) return false ;
        int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0)
            return false;
        }
    return true;
}

int primes_count (int a, int b){
    int ret = 0;
    for (int i = a; i < b; i++)
        if (is_prime(i) != 0)
            ret ++;
    return ret ;
}

typedef struct prime_request{
    int a, b;
} prime_request;


void *prime_count_thread(void *arg){
    prime_request *request = (prime_request *) arg;
    int *count = malloc(sizeof(int));
    *count = primes_count(request->a, request->b);
    return count;
}

int main(int argc, char *argv[]){
    struct timespec start, end;
    double elapsed_time;

    clock_gettime(CLOCK_MONOTONIC, &start);

    if (argc != 3){
        printf("Usage: %s n m\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t *threads = malloc(m * sizeof(pthread_t));
    prime_request *requests = malloc(m * sizeof(prime_request));

    int interval = n / m;
    int i;
    for (i = 0; i < m; i++){
        requests[i].a = i * interval;
        requests[i].b = (i + 1) * interval;
    }

    requests[m - 1].b = n;

    for (i = 0; i < m; i++)
        pthread_create(&threads[i], NULL, prime_count_thread, &requests[i]);

    int *count;
    int total = 0;
    for (i = 0; i < m; i++){
        pthread_join(threads[i], (void **) &count);
        total += *count;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Total primes: %d. It took %lf\n", total, elapsed_time);

    free(threads);
    free(requests);

    return 0;
}