#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR_COUNT 1024

char* keywords[] =  {"sell", "buy", "go", "to", "from",
 "and", "at", "has", "if", "less", "more", "than", "exit", "where",
  "total", "who","NOBODY", "NOTHING", "NOWHERE"};

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

struct questionPackage* handleWho(char** inputWords, int inputWordsCount);
struct questionPackage* handleWhere(char** inputWords, int inputWordsCount);
struct questionPackage* handleSingleTotal(char** inputWords, int inputWordsCount);
struct questionPackage* handleSingleTotalwithoutItem(char** inputWords, int inputWordsCount);
struct questionPackage* handleSingleTotalwithItem(char** inputWords, int inputWordsCount);
struct questionPackage* handleMultipleTotal(char** inputWords, int inputWordsCount);

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

//String check functions
//TODO: contains or equals
int strContainsKeyword(char* word) {
    for (int i = 0; i < 19; i++) {
        if (strstr(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int strOnlyLettersandUnderscores(char* word) {
    for (int i = 0; i < strlen(word); i++) {
        if (!((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z') || word[i] == '_')) {
            return 0;
        }
    }
    return 1;
}

int strFormatTrue(char* word) {
    return strContainsKeyword(word) && strOnlyLettersandUnderscores(word);
}

//Question parser

struct stringList {
    char* string;
    struct stringList* next;
};

struct stringList* addString(struct stringList* parent, char* string) {
    struct stringList* newString = (struct stringList*)malloc(sizeof(struct stringList));
    newString->string = malloc(strlen(string) + 1);
    strcpy(newString->string, string);
    newString->next = NULL;
    parent->next = newString;
    return newString;
}

struct questionPackage {
    char* type;
    struct stringList* subjects;
    char* location;
    char* item;
};


struct questionPackage* parseQuestion(char** inputWords, int inputWordsCount) {
    if (strcmp(inputWords[0],"who") == 0) {
        return handleWho(inputWords, inputWordsCount);
    } else if (strcmp(inputWords[1],"where") == 0) {
        return handleWhere(inputWords, inputWordsCount);
    } else if (strcmp(inputWords[1],"total") == 0) {
        return handleSingleTotal(inputWords, inputWordsCount);
    } else if (strcmp(inputWords[1],"and") == 0) {
        return handleMultipleTotal(inputWords, inputWordsCount);
    } else {
        return NULL;
    }
}

struct questionPackage* handleWho(char** inputWords, int inputWordsCount) {
    struct questionPackage* package = (struct questionPackage*)malloc(sizeof(struct questionPackage));
    package->type = "who";
    package->subjects = NULL;
    package->location = NULL;
    package->item = NULL;

    if (inputWordsCount != 4) {
        return NULL;
    }
    if (strcmp(inputWords[1],"at") != 0) {
        return NULL;
    }
    if (strFormatTrue(inputWords[2]) == 0) {
        return NULL;
    }
    package->location = malloc(strlen(inputWords[2]) + 1);
    strcpy(package->location, inputWords[2]);
    return package;

}
    
struct questionPackage* handleWhere(char** inputWords, int inputWordsCount) {
    struct questionPackage* package = (struct questionPackage*)malloc(sizeof(struct questionPackage));
    package->type = "where";
    package->subjects = NULL;
    package->location = NULL;
    package->item = NULL;

    if (inputWordsCount != 3) {
        return NULL;
    }
    if (strFormatTrue(inputWords[0]) == 0) {
        return NULL;
    }
    package->subjects = (struct stringList*)malloc(sizeof(struct stringList));
    package->subjects->string = malloc(strlen(inputWords[0]) + 1);
    strcpy(package->subjects->string, inputWords[0]);
    package->subjects->next = NULL;
    return package;
}

struct questionPackage* handleSingleTotal(char** inputWords, int inputWordsCount) {
    if (inputWordsCount ==3) {
        return handleSingleTotalwithoutItem(inputWords, inputWordsCount);
    } else if (inputWordsCount == 4) {
        return handleSingleTotalwithItem(inputWords, inputWordsCount);
    } else {
        return NULL;
    }
} 

struct questionPackage* handleSingleTotalwithoutItem(char** inputWords, int inputWordsCount) {
    struct questionPackage* package = (struct questionPackage*)malloc(sizeof(struct questionPackage));
    package->type = "singleTotalwithoutItem";
    package->subjects = NULL;
    package->location = NULL;
    package->item = NULL;

    if (strFormatTrue(inputWords[0]) == 0) {
        return NULL;
    }
    package->subjects = (struct stringList*)malloc(sizeof(struct stringList));
    package->subjects->string = malloc(strlen(inputWords[0]) + 1);
    strcpy(package->subjects->string, inputWords[0]);
    package->subjects->next = NULL;
    return package;
}

struct questionPackage* handleSingleTotalwithItem(char** inputWords, int inputWordsCount) {
    struct questionPackage* package = (struct questionPackage*)malloc(sizeof(struct questionPackage));
    package->type = "singleTotalwithItem";
    package->subjects = NULL;
    package->location = NULL;
    package->item = NULL;

    if (strFormatTrue(inputWords[0]) == 0) {
        return NULL;
    }
    package->subjects = (struct stringList*)malloc(sizeof(struct stringList));
    package->subjects->string = malloc(strlen(inputWords[0]) + 1);
    strcpy(package->subjects->string, inputWords[0]);
    package->subjects->next = NULL;

    if (strFormatTrue(inputWords[2]) == 0) {
        return NULL;
    }
    package->item = malloc(strlen(inputWords[2]) + 1);
    strcpy(package->item, inputWords[2]);
    return package;
}

struct questionPackage* handleMultipleTotal(char** inputWords, int inputWordsCount) {
    struct questionPackage* package = (struct questionPackage*)malloc(sizeof(struct questionPackage));
    package->type = "multipleTotal";
    package->subjects = NULL;
    package->location = NULL;
    package->item = NULL;

    if (strFormatTrue(inputWords[0]) == 0) {
        return NULL;
    }
    package->subjects = (struct stringList*)malloc(sizeof(struct stringList));
    package->subjects->string = malloc(strlen(inputWords[0]) + 1);
    strcpy(package->subjects->string, inputWords[0]);
    package->subjects->next = NULL;

    struct stringList* current = package->subjects;

    for (int i = 1; i < inputWordsCount-3; i += 2) {
        if (strcmp(inputWords[i],"and") == 0) {
            if (strFormatTrue(inputWords[i+1]) == 0) {
                return NULL;
            }
            current = addString(current, inputWords[i+1]);
            continue;
        }
        return NULL;
    }
    return package;
}

void main() {

    char** itemList = (char**)calloc(17000, sizeof(char*));
    int itemListCount = 0;

    struct node* subjectsHead = NULL;
    struct node* SubjectsTail = NULL;

    //example
    /*subjectsHead = (struct node*)malloc(sizeof(struct node));
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
    printf("Location of Bob: %s\n", findSubject(subjectsHead, "Bob")->location);*/

    
    while (1) { // shell loop
        char input[MAX_CHAR_COUNT + 2]; // +2 for newline and null terminator
        printf(">>"); // shell prompt
        fgets(input, MAX_CHAR_COUNT + 2, stdin); // +2 for newline and null terminator
        
        input[strlen(input) - 1] = '\0'; //remove newline
        
        if (strcmp(input, "exit") == 0) {
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
            //printf("%s\n", input_words[i]);
            printf("%d\n", strFormatTrue(input_words[i]));
        }

        if (strcmp(input_words[inputWordsCount-1], "?") == 0) {
            if (parseQuestion(input_words, inputWordsCount) == NULL) {
                printf("Invalid question format\n");
            } else {
                printf("Valid question format\n");
            }
        }

        
        
    }
}
