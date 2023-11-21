#include <stdio.h>
#include <string.h>

int main(void){
    char string[256];

    scanf("%255[^.\n]", string);

    for(int i = strlen(string) - 1; i >= 0; i--){
        printf("%c", string[i]);
    }

    return 0;
}