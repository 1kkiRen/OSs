#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

bool is_prime(int n){
    if (n <= 1) return false ;
        int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0)
            return false;
        }
    return true;
}

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

int k = 0;
int c = 0;

int n = 0;

int get_number_to_check(){
    int ret = k;
    if (k != n)
        k++;
    return ret;
}

void increment_primes(){
    pthread_mutex_lock(&global_lock);
    c++;
    pthread_mutex_unlock(&global_lock);
}

void* check_primes(void* arg){
    while (1) {
        pthread_mutex_lock(&global_lock);
        int number = get_number_to_check();
        pthread_mutex_unlock(&global_lock);
        if (number == n)
            break;
        if (is_prime(number))
            increment_primes();
    }
}

int main(int argc, char* argv[]){
    struct timespec start, end;
    double elapsed_time;

    clock_gettime(CLOCK_MONOTONIC, &start);

    if (argc != 3){
        printf("Usage: %s n m\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t* threads = malloc(m * sizeof(pthread_t));

    int i;
    for (i = 0; i < m; i++)
        pthread_create(&threads[i], NULL, check_primes, NULL);

    for (i = 0; i < m; i++)
        pthread_join(threads[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Total primes: %d. It took %lf\n", c, elapsed_time);

    return 0;
}
