#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void){
    int integer = INT_MAX;
    unsigned short int shortInt = USHRT_MAX;
    long int longInt = LONG_MAX;
    float floatNum = FLT_MAX;
    double doubleNum = DBL_MAX;

    printf("Integer size: %d, value: %d\n", (int)sizeof(integer), integer);
    printf("Short integer size: %d, value: %d\n", (int)sizeof(shortInt), shortInt);
    printf("Long integer size: %d, value: %ld\n", (int)sizeof(longInt), longInt);
    printf("Float size: %d, value: %f\n", (int)sizeof(floatNum), floatNum);
    printf("Double size: %d, value: %lf\n", (int)sizeof(doubleNum), doubleNum);

    return 0;
}