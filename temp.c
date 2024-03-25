#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR_COUNT 1024

void main() {

    
    printf("Welcome to the shell!\n");
        char input[MAX_CHAR_COUNT + 2]; // +2 for newline and null terminator
        printf(">>"); // shell
        fgets(input, MAX_CHAR_COUNT + 2, stdin); // +2 for newline and null terminator
        char* name;
        name = (char*)calloc(strlen(input)+2, sizeof(char));
        printf("Allocated memory size for name: %d\n", (strlen(input)+2) * sizeof(char));
        strcpy(name, input);
        printf("size of name: %d\n", strlen(name));
        printf("name: %s\n", name);
}