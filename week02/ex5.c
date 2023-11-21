#include <stdio.h>

int tribonacci(int);

int main(void){
    printf("%lld\n", tribonacci(4));
    printf("%lld\n", tribonacci(36));

    return 0;
}

int tribonacci(int n){
    if (n == 0){
        return 0;
    } else if (n == 1 || n == 2){
        return 1;
    }
    
    int a = 0, b = 1, c = 1, d = 2;
    for(int i = 3; i <= n; i++){
        d = a + b + c;
        a = b;
        b = c;
        c = d;
    }
    return d;
}
