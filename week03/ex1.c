#include <stdio.h>
#include <stdlib.h>

int const_tri(int* const p, int n){
    if (n == 0){
        return 0;
    } else if (n == 1 || n == 2) {
        return 1;
    } else {
        int temp = 0;
        for (int i = 3; i <= n; i++) {
            *(p + 2) = temp + *p + *(p + 1);
            temp = *p;
            *p = *(p + 1);
            *(p + 1) = *(p + 2);
        }
        return *(p + 2);
    }
}

int main(void){
    const int x = 1;
    const int* q = &x;

    int* const p = (int *) malloc(3 * sizeof(int));
    if (p == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    *p = x;
    *(p + 1) = x;
    *(p + 2) = 2 * x;

    printf("%p\n", p);
    printf("%p\n", p + 1);
    printf("%p\n", p + 2);

    if (p == p + 1 - 1 && p + 1 == p + 2 - 1){
        printf("The cells are contiguous\n");
    } else {
        printf("The cells are not contiguous\n");
    }

    if (const_tri(p, 36) == 1132436852){
        printf("The function works\n");
    } else {
        printf("The function does not work\n");
    }

    free(p);

    return 0;
}