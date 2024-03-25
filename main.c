#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR_COUNT 1024

void main() {
    
    while (1) {
        printf("Welcome to the shell!\n");
        char input[MAX_CHAR_COUNT + 2]; // +2 for newline and null terminator
        printf(">>"); // shell
        fgets(input, MAX_CHAR_COUNT + 2, stdin); // +2 for newline and null terminator
        
        if (strcmp(input, "exit\n") == 0) {
            break;
        } 

        char* input_words[MAX_CHAR_COUNT];
        int input_words_count = 0;

        char* token = strtok(input, " ");

        while (token != NULL) {
            input_words[input_words_count] = token;
            input_words_count++;
            token = strtok(NULL, " ");
        }
        printf("Input words count: %d\n", input_words_count);
        
        for (int i = 0; i < input_words_count; i++) {
            printf("%s\n", input_words[i]);
        }

        printf("hello\n");
        
        
    }
}
