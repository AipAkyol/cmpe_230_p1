#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR_COUNT 1024
int question(int* wordCounter,char* meaningfulWords[],char* inputWords[]){
    
}
int action(int* wordCounter,char* meaningfulWords[],char* inputWords[]){
    int validCheck = 1;
    do{
        *wordCounter++;
        if(subject(wordCounter,meaningfulWords,inputWords)==0){
            validCheck=0;
            break;
        }
        *wordCounter++;
    }while(strcmp("and",inputWords[*wordCounter]))
    
    if(validCheck==0){
        return 0;
    }
    for(int i=0;i)

}
int subject(int* wordCounter,char* meaningfulWords[],char* inputWords[]){
    
}
int object(int* wordCounter,char* meaningfulWords[],char* inputWords[]){
    
}
int verb(int* wordCounter,char* meaningfulWords[],char* inputWords[]){
    
}
int secondVerbs(int* wordCounter,char* meaningfulWords[],char* inputWords[]){
    
}
void main() {
    char* meaningfulWords[] =  {"sell", "buy", "go", "to", "from", "and", "at", "has", "if", "less", "more", "than", "exit", "where", "total", "who","NOBODY", "NOTHING", "NOWHERE"};
    char** item_list = (char**)calloc(17000,sizeof(char*));
    printf("lkdfsghdfskjghlksdjfhg;lkdsfhgjjks");
    
    
    
    
    
    
    
    
    
    
    while (1) { // shell loop
        char input[MAX_CHAR_COUNT + 2]; // +2 for newline and null terminator
        printf(">>"); // shell prompt
        fgets(input, MAX_CHAR_COUNT + 2, stdin); // +2 for newline and null terminator
        
        if (strcmp(input, "exit\n") == 0) {
            break;
        } 
        int input_length = strlen(input);
        printf("%c",input[input_length-2]);

        char* input_words[MAX_CHAR_COUNT];
        int input_words_count = 0;

        char* token = strtok(input, " ");

        while (token != NULL) {
            input_words[input_words_count] = token;
            input_words_count++;
            token = strtok(NULL, " ");
        }
        //printf("Input words count: %d\n", input_words_count);
        //
        //for (int i = 0; i < input_words_count; i++) {
        //    printf("%s\n", input_words[i]);
        //}

    }
    printf("finished");
}
