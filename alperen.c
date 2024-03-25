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

struct subject* createSubject(char* name) {
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

struct subject* findSubject(struct node* head, char* name) {
    struct node* current = head;
    while (current != NULL) {
        if (strcmp(current->person->name, name) == 0) {
            return current->person;
        }
        current = current->next;
    }
    return NULL;
};

void changeLocation(struct subject* person, char* new_location) {
    free(person->location);
    person->location = malloc(strlen(new_location) + 1); // +1 for the null terminator
    strcpy(person->location, new_location);
};

struct node* addNode(struct node* parent, char* name) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->person = createSubject(name);
    new_node->next = NULL;
    parent->next = new_node;
    return new_node;
};


void main() {

    struct node* subjects_head = NULL;
    struct node* subjects_tail = NULL;

    //example
    subjects_head = (struct node*)malloc(sizeof(struct node));
    subjects_head->person = createSubject("John");
    subjects_head->next = NULL;
    subjects_tail = subjects_head;
    printf("Name of the first person: %s\n", subjects_head->person->name);
    printf("Location of the first person: %s\n", subjects_head->person->location);
    printf("first item of the inventory of the first person: %d\n", subjects_head->person->inventory[16777]);

    subjects_tail = addNode(subjects_tail, "Alice");
    printf("Name of the second person: %s\n", subjects_tail->person->name);
    printf("Location of the second person: %s\n", subjects_tail->person->location);
    printf("first item of the inventory of the second person: %d\n", subjects_tail->person->inventory[16777]);

    subjects_tail = addNode(subjects_tail, "Bob");
    printf("Name of the third person: %s\n", subjects_tail->person->name);
    printf("Location of the third person: %s\n", subjects_tail->person->location);
    printf("first item of the inventory of the third person: %d\n", subjects_tail->person->inventory[16777]);

    changeLocation(findSubject(subjects_head, "Alice"), "New York");
    printf("Location of Alice: %s\n", findSubject(subjects_head, "Alice")->location);

    changeLocation(findSubject(subjects_head, "Bob"), "Los Angeles");
    printf("Location of Bob: %s\n", findSubject(subjects_head, "Bob")->location);

    
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
