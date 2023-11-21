#include <stdio.h>

long long int convert(long long num, short int source, short int target);


int main(void){
    long long int num;
    int source, target;

    scanf("%lld %hd %hd", &num, &source, &target);

    if (source < 2 || source > 10 || target < 2 || target > 10){
        printf("cannot convert!\n");
        return 0;
    }

    printf("%lld\n", convert(num, source, target));

    return 0;
}


long long int convert(long long num, short int source, short int target){
    long long int intermediateValue = 0, multiplier = 1;

    while(num > 0){
        intermediateValue += (num % 10) * multiplier;
        num /= 10;
        multiplier *= source;
    }

    long long int result = 0;
    multiplier = 1;
    while(intermediateValue > 0){
        result += (intermediateValue % target) * multiplier;
        intermediateValue /= target;
        multiplier *= 10;
    }

    return result;
}