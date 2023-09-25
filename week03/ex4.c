#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*));
void* add(const void* a, const void* b);
void* multiply(const void* a, const void* b);
void* maxim(const void* a, const void* b);
int* intAdd(const void* a, const void* b);
int* intMultiply(const void* a, const void* b);
int* intMax(const void* a, const void* b);
double* doubleAdd(const void* a, const void* b);
double* doubleMultiply(const void* a, const void* b);
double* doubleMax(const void* a, const void* b);
void print_int_array(int* arr, int n);
void print_double_array(double* arr, int n);


int main(void){
    int int_arr[5] = {1, 2, 3, 4, 5};
    double double_arr[5] = {1.1, 2.2, 3.3, 4.4, 5.5};

    int zero = 0;
    double zero_double = 0.0;

    int one = 1;
    double one_double = 1.0;

    int min_int = INT_MIN;
    double min_double = -1.0/0.0;


    int* int_result = aggregate(int_arr, sizeof(int), 5, &zero, add);
    int* int_result2 = aggregate(int_arr, sizeof(int), 5, &one, multiply);
    int* int_result3 = aggregate(int_arr, sizeof(int), 5, &min_int, maxim);


    double* double_result = aggregate(double_arr, sizeof(double), 5, &zero_double, add);
    double* double_result2 = aggregate(double_arr, sizeof(double), 5, &one_double, multiply);
    double* double_result3 = aggregate(double_arr, sizeof(double), 5, &min_double, maxim);


    printf("int array: ");
    print_int_array(int_arr, 5);
    printf("double array: ");
    print_double_array(double_arr, 5);

    printf("int addition: ");
    print_int_array(int_result, 1);
    printf("int multiplication: ");
    print_int_array(int_result2, 1);
    printf("int max: ");
    print_int_array(int_result3, 1);

    printf("double addition: ");
    print_double_array(double_result, 1);
    printf("double multiplication: ");
    print_double_array(double_result2, 1);
    printf("double max: ");
    print_double_array(double_result3, 1);

    free(int_result);
    free(double_result);
    free(int_result2);
    free(int_result3);
    free(double_result2);
    free(double_result3);
    
    return 0;
}


void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)){
    if(size == sizeof(int)){
        int* result = initial_value;
        int* arr = (int*) base;
        for (int i = 0; i < n; i++){
            result = opr(result, arr + i);
        }
        return result;
    }
    else if(size == sizeof(double)){
        double* result = initial_value;
        double* arr = (double*) base;
        for (int i = 0; i < n; i++){
            result = opr(result, arr + i);
        }
        return result;
    }
    else{
        printf("Error: unsupported datatype\n");
        exit(1);
    }
}

int* intAdd(const void* a, const void* b){
    int* result = malloc(sizeof(int));
    *result = *(int*)a + *(int*)b;
    return result;
}

int* intMultiply(const void* a, const void* b){
    int* result = malloc(sizeof(int));
    *result = *(int*)a * *(int*)b;
    return result;
}

int* intMax(const void* a, const void* b){
    int* result = malloc(sizeof(int));
    *result = *(int*)a > *(int*)b ? *(int*)a : *(int*)b;
    return result;
}

double* doubleAdd(const void* a, const void* b){
    double* result = malloc(sizeof(double));
    *result = *(double*)a + *(double*)b;
    return result;
}

double* doubleMultiply(const void* a, const void* b){
    double* result = malloc(sizeof(double));
    *result = *(double*)a * *(double*)b;
    return result;
}

double* doubleMax(const void* a, const void* b){
    double* result = malloc(sizeof(double));
    *result = *(double*)a > *(double*)b ? *(double*)a : *(double*)b;
    return result;
}

void print_int_array(int* arr, int n){
    for (int i = 0; i < n; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void print_double_array(double* arr, int n){
    for (int i = 0; i < n; i++){
        printf("%lf ", arr[i]);
    }
    printf("\n");
}

void* add(const void* a, const void* b){
    if(sizeof(a) == sizeof(int)){
        int* result = malloc(sizeof(int));
        result = intAdd(a, b);
        return result;
    }
    else if(sizeof(a) == sizeof(double)){
        double* result = malloc(sizeof(double));
        result = doubleAdd(a, b);
        return result;
    }
    else{
        printf("Error: unsupported datatype\n");
        exit(1);
    }
}

void* multiply(const void* a, const void* b){
    if(sizeof(a) == sizeof(int)){
        int* result = malloc(sizeof(int));
        result = intMultiply(a, b);
        return result;
    }
    else if(sizeof(a) == sizeof(double)){
        double* result = malloc(sizeof(double));
        result = doubleMultiply(a, b);
        return result;
    }
    else{
        printf("Error: unsupported datatype\n");
        exit(1);
    }
}

void* maxim(const void* a, const void* b){
    if(sizeof(a) == sizeof(int)){
        int* result = malloc(sizeof(int));
        result = intMax(a, b);
        return result;
    }
    else if(sizeof(a) == sizeof(double)){
        double* result = malloc(sizeof(double));
        result = doubleMax(a, b);
        return result;
    }
    else{
        printf("Error: unsupported datatype\n");
        exit(1);
    }
}