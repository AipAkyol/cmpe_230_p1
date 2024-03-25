#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR_COUNT 1024

struct subject {
    char* name;
    char* location;
    int inventory[17000]; //TODO: check numbers
};

struct node {
    struct subject* person;
    struct node* next;
};

struct subject* create_subject(char* name) {
    struct subject* new_subject = (struct subject*)malloc(sizeof(struct subject));
    new_subject->name = malloc(strlen(name) + 1); // +1 for the null terminator
    strcpy(new_subject->name, name);
    new_subject->location = malloc(strlen("NOWHERE") + 1); // +1 for the null terminator
    strcpy(new_subject->location, "NOWHERE");
    for (int i = 0; i < 17000; i++) {
        new_subject->inventory[i] = 0;
    }
    return new_subject;
};

void change_location(struct subject* person, char* new_location) {
    free(person->location);
    person->location = malloc(strlen(new_location) + 1); // +1 for the null terminator
    strcpy(person->location, new_location);
};

struct node* add_node(struct node* parent, char* name) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->person = create_subject(name);
    new_node->next = NULL;
    parent->next = new_node;
    return new_node;
};


void main() {

    struct node* subjects_head = NULL;
    struct node* subjects_tail = NULL;

    //example
    subjects_head = (struct node*)malloc(sizeof(struct node));
    subjects_head->person = create_subject("John");
    subjects_head->next = NULL;
    subjects_tail = subjects_head;
    printf("Name of the first person: %s\n", subjects_head->person->name);
    printf("Location of the first person: %s\n", subjects_head->person->location);
    printf("first item of the inventory of the first person: %d\n", subjects_head->person->inventory[16777]);

    
    while (1) { // shell loop
        printf("Welcome to the shell!\n");
        char input[MAX_CHAR_COUNT + 2]; // +2 for newline and null terminator
        printf(">>"); // shell prompt
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
