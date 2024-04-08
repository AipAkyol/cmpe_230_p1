#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR_COUNT 1024
#define MAX_INVERTORY_SIZE 17000

char* keywords[] =  {"sell", "buy", "go", "to", "from",
 "and", "at", "has", "if", "less", "more", "than", "exit", "where",
  "total", "who","NOBODY", "NOTHING", "NOWHERE"};

struct subject {
    char* name;
    char* location;
    int inventory[MAX_INVERTORY_SIZE]; //TODO: check numbers
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
    for (int i = 0; i < MAX_INVERTORY_SIZE; i++) {
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

int getItemIndex(char* item, char** itemList, int *itemListCount) {
    for (int i = 0; i < *itemListCount; i++) {
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

void addItem(char* item, char** itemList, int* itemListCount) {
    itemList[*itemListCount] = malloc(strlen(item) + 1);
    strcpy(itemList[*itemListCount], item);
    (*itemListCount)++;
}

//String check functions
//TODO: contains or equals
int strContainsKeyword(char* word) {
    for (int i = 0; i < 19; i++) {
        if (strstr(word, keywords[i]) != NULL) {
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
    return !strContainsKeyword(word) && strOnlyLettersandUnderscores(word);
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

void freeStringList(struct stringList* head) {
    struct stringList* current = head;
    while (current != NULL) {
        struct stringList* next = current->next;
        free(current->string);
        current->string = NULL;
        free(current);
        current = next;
    }
}

struct questionPackage {
    char* type;
    struct stringList* subjects;
    char* location;
    char* item;
};

void freeQuestionPackage(struct questionPackage* package) {
    free(package->type);
    package->type = NULL;
    freeStringList(package->subjects);
    free(package->location);
    package->location = NULL;
    free(package->item);
    package->item = NULL;
    free(package);
    package = NULL;
}


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

void runQuestion(struct questionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    if (package == NULL) {
        printf("INVALID\n");
        return;
    }
    else if (strcmp(package->type, "who") == 0) {
        struct stringList* peopleAtLocationTail = NULL;
        struct stringList* peopleAtLocationHead = NULL;
        struct node* current = subjectsHead;
        while (current != NULL) {
            if (strcmp(current->person->location, package->location) == 0) {
                if (peopleAtLocationHead == NULL) {
                    peopleAtLocationHead = (struct stringList*)malloc(sizeof(struct stringList));
                    peopleAtLocationHead->string = malloc(strlen(current->person->name) + 1);
                    strcpy(peopleAtLocationHead->string, current->person->name);
                    peopleAtLocationHead->next = NULL;
                    peopleAtLocationTail = peopleAtLocationHead;
                } else {
                    peopleAtLocationTail = addString(peopleAtLocationTail, current->person->name);
                }
            }
            current = current->next;
        }
        if (peopleAtLocationHead == NULL) {
            printf("NOBODY\n");
        } else {
            printf("%s", peopleAtLocationHead->string);
            struct stringList* current = peopleAtLocationHead->next;
            while (current != NULL) {
                printf("and %s", current->string);
                current = current->next;
            }
            printf("\n");
        }
    } else if (strcmp(package->type, "where") == 0) {
        struct subject* current = findSubject(subjectsHead, package->subjects->string);
        if (current == NULL) {
            printf("NOWHERE\n");
        } else {
            printf("%s\n", current->location);
        }
    } else if (strcmp(package->type, "singleTotalwithoutItem") == 0) {
        struct subject* current = findSubject(subjectsHead, package->subjects->string);
        if (current == NULL) {
            printf("NOTHING\n");
            return;
        } 
        int isFirstItem = 1;
        for (int i = 0; i < *itemListCount; i++) {
            if (current->inventory[i] != 0) {
                if (isFirstItem) {
                    printf("%d %s", current->inventory[i], itemList[i]);
                    isFirstItem = 0;
                } else {
                    printf(" and %d %s", current->inventory[i], itemList[i]);
                }
            }
        }
        printf("\n");
    } else if (strcmp(package->type, "singleTotalwithItem") == 0) {
        struct subject* current = findSubject(subjectsHead, package->subjects->string);
        if (current == NULL) {
            printf("NOTHING\n");
            return;
        } 
        int itemIndex = getItemIndex(package->item, itemList, itemListCount);
        if (itemIndex == -1) {
            printf("0\n");
            return;
        }
        printf("%d\n", current->inventory[itemIndex]); 
    } else if (strcmp(package->type, "multipleTotal") == 0) {
        struct stringList* current = package->subjects;
        int itemIndex = getItemIndex(package->item, itemList, itemListCount);
        int total = 0;
        while (current != NULL) {
            struct subject* currentSubject = findSubject(subjectsHead, current->string);
            if (currentSubject != NULL) {
                total += currentSubject->inventory[itemIndex];
            }
            current = current->next;
        }
        printf("%d\n", total);
    }
    else {
        printf("logic error at question answer\n");
    }
}

struct questionPackage* handleWho(char** inputWords, int inputWordsCount) {
    struct questionPackage* package = (struct questionPackage*)malloc(sizeof(struct questionPackage));
    package->type = malloc(strlen("who") + 1);
    strcpy(package->type, "who");
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
    package->type = malloc(strlen("where") + 1);
    strcpy(package->type, "where");
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
    package->type = malloc(strlen("singleTotalwithoutItem") + 1);
    strcpy(package->type, "singleTotalwithoutItem");
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
    package->type = malloc(strlen("singleTotalwithItem") + 1);
    strcpy(package->type, "singleTotalwithItem");
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
    package->type = malloc(strlen("multipleTotal") + 1);
    strcpy(package->type, "multipleTotal");
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

    if (strcmp(inputWords[inputWordsCount-3],"total") != 0) {
        return NULL;
    }
    if (strFormatTrue(inputWords[inputWordsCount-2]) == 0) {
        return NULL;
    }
    package->item = malloc(strlen(inputWords[inputWordsCount-2]) + 1);
    strcpy(package->item, inputWords[inputWordsCount-2]);
    return package;
}

// Action package

struct intList {
    int* number;
    struct intList* next;
};

struct intList* addInt(struct intList* parent, int* number) {
    struct intList* newInt = (struct intList*)malloc(sizeof(struct intList));
    newInt->number = malloc(sizeof(int));
    *newInt->number = *number;
    newInt->next = NULL;
    parent->next = newInt;
    return newInt;
}

void freeIntList(struct intList* head) {
    struct intList* current = head;
    while (current != NULL) {
        struct intList* next = current->next;
        free(current->number);
        current->number = NULL;
        free(current);
        current = next;
    }
}

struct actionPackage {
    char* type;
    struct stringList* buyers;
    struct stringList* sellers;
    struct stringList* goers;
    struct stringList* items;
    struct intList* counts;
    char* location;
};

void freeActionPackage(struct actionPackage* package) {
    free(package->type);
    package->type = NULL;
    freeStringList(package->buyers);
    freeStringList(package->sellers);
    freeStringList(package->goers);
    freeStringList(package->items);
    freeIntList(package->counts);
    free(package->location);
    package->location = NULL;
    free(package);
    package = NULL;
}

// Condition package

struct conditionPackage {
    char* type;
    struct stringList* subjects;
    struct stringList* items;
    struct intList* counts;
    char* location;
};

void freeConditionPackage(struct conditionPackage* package) {
    free(package->type);
    package->type = NULL;
    freeStringList(package->subjects);
    freeStringList(package->items);
    freeIntList(package->counts);
    free(package->location);
    package->location = NULL;
    free(package);
    package = NULL;
}

void main() {

    char** itemList = (char**)calloc(MAX_INVERTORY_SIZE, sizeof(char*));
    int* itemListCount = (int*)malloc(sizeof(int));
    *itemListCount = 0;

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

    changeLocation(findSubject(subjectsHead, "Alice"), "New_York");
    printf("Location of Alice: %s\n", findSubject(subjectsHead, "Alice")->location);

    changeLocation(findSubject(subjectsHead, "Bob"), "Los_Angeles");
    printf("Location of Bob: %s\n", findSubject(subjectsHead, "Bob")->location);

    addItem("apple", itemList, itemListCount);
    addItem("banana", itemList, itemListCount);
    addItem("carrot", itemList, itemListCount);
    printf("Item list count: %d\n", *itemListCount);
    printf("Item list first item: %s\n", itemList[0]);
    printf("Item list second item: %s\n", itemList[1]);
    printf("Item list third item: %s\n", itemList[2]);

    changeItemCount(getItemIndex("apple", itemList, itemListCount), findSubject(subjectsHead, "Alice"), 5);
    printf("Alice's apple count: %d\n", getItemCount(getItemIndex("apple", itemList, itemListCount), findSubject(subjectsHead, "Alice")));

    changeItemCount(getItemIndex("apple", itemList, itemListCount), findSubject(subjectsHead, "Alice"), -2);
    printf("Alice's apple count: %d\n", getItemCount(getItemIndex("apple", itemList, itemListCount), findSubject(subjectsHead, "Alice")));

    changeItemCount(getItemIndex("banana", itemList, itemListCount), findSubject(subjectsHead, "Alice"), 3);
    printf("Alice's banana count: %d\n", getItemCount(getItemIndex("banana", itemList, itemListCount), findSubject(subjectsHead, "Alice")));

    changeItemCount(getItemIndex("banana", itemList, itemListCount), findSubject(subjectsHead, "Alice"), -1);
    printf("Alice's banana count: %d\n", getItemCount(getItemIndex("banana", itemList, itemListCount), findSubject(subjectsHead, "Alice")));

    changeItemCount(getItemIndex("carrot", itemList, itemListCount), findSubject(subjectsHead, "Alice"), 1);
    printf("Alice's carrot count: %d\n", getItemCount(getItemIndex("carrot", itemList, itemListCount), findSubject(subjectsHead, "Alice")));

    changeItemCount(getItemIndex("carrot", itemList, itemListCount), findSubject(subjectsHead, "Bob"), 2);
    printf("Bob's carrot count: %d\n", getItemCount(getItemIndex("carrot", itemList, itemListCount), findSubject(subjectsHead, "Bob")));

    changeItemCount(getItemIndex("carrot", itemList, itemListCount), findSubject(subjectsHead, "John"), 3);
    printf("John's carrot count: %d\n", getItemCount(getItemIndex("carrot", itemList, itemListCount), findSubject(subjectsHead, "John"))); 



    
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
            //printf("%d\n", strFormatTrue(input_words[i]));
        }

        if (strcmp(input_words[inputWordsCount-1], "?") == 0) {
            struct questionPackage* package = parseQuestion(input_words, inputWordsCount);
            runQuestion(package, subjectsHead, itemList, itemListCount);
            freeQuestionPackage(package);
        }
    }
}
