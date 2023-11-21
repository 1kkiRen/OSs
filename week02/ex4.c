#include <stdio.h>
#include <string.h>
#include <ctype.h>

void count(char[], char);
void countAll(char[]);

int main(void){
    char string[256];
    char character = '\0';
    
    scanf("%255[^,\n], %c", string, &character);

    for(int i = 0; i < strlen(string); i++){
        string[i] = tolower(string[i]);
    }

    if (character == '\0'){
        countAll(string);
    } else {
        count(string, character);
    }
}

void count(char string[], char character){
    int count = 0;
    for(int i = 0; i < strlen(string); i++){
        if (string[i] == character){
            count++;
        }
    }
    printf("%c:%d", character, count);
}

void countAll(char string[]){
    for (int i = 0; i < strlen(string); i++){
        if (string[i] == ' '){
            continue;
        }
        count(string, string[i]);
        if (i != strlen(string) - 1){
            printf(", ");
        }
    }
}
