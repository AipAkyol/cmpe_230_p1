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

// Subject and Node functions

struct subject* createSubject(char* name) {
    struct subject* new_subject = (struct subject*)malloc(sizeof(struct subject));
    new_subject->name = malloc(strlen(name) + 1);
    strcpy(new_subject->name, name);
    new_subject->location = malloc(strlen("NOWHERE") + 1);
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
    person->location = malloc(strlen(new_location) + 1);
    strcpy(person->location, new_location);
};

struct node* addNode(struct node* parent, char* name) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->person = createSubject(name);
    newNode->next = NULL;
    parent->next = newNode;
    return newNode;
};

// Item functions

int getItemIndex(char* item, char** itemList, int itemListCount) {
    for (int i = 0; i < itemListCount; i++) {
        if (strcmp(item, itemList[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int getItemCount(int itemIndex, struct subject* person) {
    return person->inventory[itemIndex];
}

void changeItemCount(int itemIndex, struct subject* person, int diff) {
    person->inventory[itemIndex] += diff;
}

void main() {

    char** itemList = (char**)calloc(17000, sizeof(char*));
    int itemListCount = 0;

    struct node* subjectsHead = NULL;
    struct node* SubjectsTail = NULL;

    //example
    subjectsHead = (struct node*)malloc(sizeof(struct node));
    subjectsHead->person = createSubject("John");
    subjectsHead->next = NULL;
    SubjectsTail = subjectsHead;
    printf("Name of the first person: %s\n", subjectsHead->person->name);
    printf("Location of the first person: %s\n", subjectsHead->person->location);
    printf("first item of the inventory of the first person: %d\n", subjectsHead->person->inventory[16777]);

    SubjectsTail = addNode(SubjectsTail, "Alice");
    printf("Name of the second person: %s\n", SubjectsTail->person->name);
    printf("Location of the second person: %s\n", SubjectsTail->person->location);
    printf("first item of the inventory of the second person: %d\n", SubjectsTail->person->inventory[16777]);

    SubjectsTail = addNode(SubjectsTail, "Bob");
    printf("Name of the third person: %s\n", SubjectsTail->person->name);
    printf("Location of the third person: %s\n", SubjectsTail->person->location);
    printf("first item of the inventory of the third person: %d\n", SubjectsTail->person->inventory[16777]);

    changeLocation(findSubject(subjectsHead, "Alice"), "New York");
    printf("Location of Alice: %s\n", findSubject(subjectsHead, "Alice")->location);

    changeLocation(findSubject(subjectsHead, "Bob"), "Los Angeles");
    printf("Location of Bob: %s\n", findSubject(subjectsHead, "Bob")->location);

    
    while (1) { // shell loop
        printf("Welcome to the shell!\n");
        char input[MAX_CHAR_COUNT + 2]; // +2 for newline and null terminator
        printf(">>"); // shell prompt
        fgets(input, MAX_CHAR_COUNT + 2, stdin); // +2 for newline and null terminator
        
        if (strcmp(input, "exit\n") == 0) {
            break;
        } 

        char* input_words[MAX_CHAR_COUNT];
        int inputWordsCount = 0;

        char* token = strtok(input, " ");

        while (token != NULL) {
            input_words[inputWordsCount] = token;
            inputWordsCount++;
            token = strtok(NULL, " ");
        }
        printf("Input words count: %d\n", inputWordsCount);
        
        for (int i = 0; i < inputWordsCount; i++) {
            printf("%s\n", input_words[i]);
        }

        /*if (strcmp(input_words[inputWordsCount-1], "?\n") == 0) {
            printf("question mark\n");
        }*/
        printf("hello\n");
        
        
    }
}
