#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*));
void* add(const void* a, const void* b);
void* multiply(const void* a, const void* b);
void* maxim(const void* a, const void* b);
void* intAdd(const void* a, const void* b);
void* intMultiply(const void* a, const void* b);
void* intMax(const void* a, const void* b);
void* doubleAdd(const void* a, const void* b);
void* doubleMultiply(const void* a, const void* b);
void* doubleMax(const void* a, const void* b);
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


    int* int_result = aggregate(int_arr, sizeof(int), 5, &zero, intAdd);
    int* int_result2 = aggregate(int_arr, sizeof(int), 5, &one, intMultiply);
    int* int_result3 = aggregate(int_arr, sizeof(int), 5, &min_int, intMax);


    double* double_result = aggregate(double_arr, sizeof(double), 5, &zero_double, doubleAdd);
    double* double_result2 = aggregate(double_arr, sizeof(double), 5, &one_double, doubleMultiply);
    double* double_result3 = aggregate(double_arr, sizeof(double), 5, &min_double, doubleMax);


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
    free(int_result2);
    free(int_result3);
    free(double_result);
    free(double_result2);
    free(double_result3);
    
    return 0;
}


void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)){
    if(size == sizeof(int)){
        int* result = (int*) initial_value;
        int* array = (int*) base;
        for(int i = 0; i < n; i++){
            result = opr(result, array + i);
        }
        return result;
    }
    else if(size == sizeof(double)){
        double* result = (double*) initial_value;
        double* array = (double*) base;
        for(int i = 0; i < n; i++){
            result = opr(result, array + i);
        }
        return result;
    }
    else{
        printf("Error: unsupported datatype\n");
        exit(1);
    }
}

void* intAdd(const void* a, const void* b){
    int* result = malloc(sizeof(int));
    *result = *(int*)a + *(int*)b;
    return result;
}

void* intMultiply(const void* a, const void* b){
    int* result = malloc(sizeof(int));
    *result = *(int*)a * *(int*)b;
    return result;
}

void* intMax(const void* a, const void* b){
    int* result = malloc(sizeof(int));
    *result = *(int*)a > *(int*)b ? *(int*)a : *(int*)b;
    return result;
}

void* doubleAdd(const void* a, const void* b){
    double* result = malloc(sizeof(double));
    *result = *(double*)a + *(double*)b;
    return result;
}

void* doubleMultiply(const void* a, const void* b){
    double* result = malloc(sizeof(double));
    *result = *(double*)a * *(double*)b;
    return result;
}

void* doubleMax(const void* a, const void* b){
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