#include <stdio.h>

void printTriangle(int);
void printPyramid(int);
void printRectangle(int);

int main(void){
    int n;
    scanf("%d", &n);

    printTriangle(n);
    printPyramid(n);
    printRectangle(n);

    return 0;
}

void printTriangle(int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < i + 1; j++){
            printf("*");
        }
        printf("\n");
    }
}

void printPyramid(int n){
    for(int i = 0; i < n / 2; i++){
        for(int j = 0; j < i + 1; j++){
            printf("*");
        }
        printf("\n");
    }

    for(int i = n / 2; i >= 0; i--){
        for(int j = 0; j < i + 1; j++){
            printf("*");
        }
        printf("\n");
    }
}

void printRectangle(int n){
    for(int i = 0 ; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("*");
        }
        printf("\n");
    }
}

