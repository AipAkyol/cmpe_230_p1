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

// String check functions
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

// String list

struct stringList {
    char* string;
    struct stringList* next;
};

struct stringList* newStringList(char* string) {
    struct stringList* newString = (struct stringList*)malloc(sizeof(struct stringList));
    newString->string = (char*)malloc(strlen(string) + 1);
    strcpy(newString->string, string);
    newString->next = NULL;
    return newString;
}

int addString(struct stringList* root , char* string) {
    struct stringList* current = root;
    if (strcmp(current->string, string) == 0) {
        return 0;
    }
    while (current->next != NULL) {
        if (strcmp(current->next->string, string) == 0) {
            return 0;
        }
        current = current->next;
    }
    struct stringList* newString = newStringList(string);
    current->next = newString;
    return 1;
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

// Question package

struct questionPackage {
    char* type;
    struct stringList* subjects;
    char* location;
    char* item;
};

void freeQuestionPackage(struct questionPackage* package) {
    if (package == NULL) {
        return;
    }
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

// Question parser 

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
    package->subjects = newStringList(inputWords[0]);

    struct stringList* root = package->subjects;

    for (int i = 1; i < inputWordsCount-3; i += 2) {
        if (strcmp(inputWords[i],"and") == 0) {
            if (strFormatTrue(inputWords[i+1]) == 0) {
                return NULL;
            }
            int checkUniqueness = addString(root, inputWords[i+1]);
            if (checkUniqueness == 0) {
                return NULL;
            }
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

// Question runner

void runQuestion(struct questionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    if (package == NULL) {
        printf("INVALID\n");
        return;
    }
    else if (strcmp(package->type, "who") == 0) {
        struct stringList* peopleAtLocation = NULL;
        struct node* current = subjectsHead;
        while (current != NULL) {
            if (strcmp(current->person->location, package->location) == 0) {
                if (peopleAtLocation == NULL) {
                    peopleAtLocation = newStringList(current->person->name);
                } else {
                    int checkwho = addString(peopleAtLocation, current->person->name);
                    //TODO: check if this is necessary
                    if (checkwho == 0) {
                        printf("logic error at who with new string list strcvuteure that implies not unique person at subject list\n");
                    }
                }
            }
            current = current->next;
        }
        if (peopleAtLocation == NULL) {
            printf("NOBODY\n");
        } else {
            printf("%s", peopleAtLocation->string);
            struct stringList* current = peopleAtLocation->next;
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

// Int list

struct intList {
    int number;
    struct intList* next;
};

struct intList* newIntList(int number) {
    struct intList* newInt = (struct intList*)malloc(sizeof(struct intList));
    newInt->number = number;
    newInt->next = NULL;
    return newInt;
}

int addInt(struct intList* root, int number) {
    if (root == NULL) {
        printf("logic error at addInt\n");
    }
    struct intList* current = root;
    while (current->next != NULL) {
        current = current->next;
    }
    struct intList* newInt = newIntList(number);
    current->next = newInt;
    return 1;
}

void freeIntList(struct intList* head) {
    struct intList* current = head;
    while (current != NULL) {
        struct intList* next = current->next;
        free(current);
        current = next;
    }
}

// Action Package

struct actionPackage {
    char* type;
    struct stringList* subjects;
    char* secondSubject;
    struct stringList* items;
    struct intList* counts;
    char* location;
};

struct actionPackage* newActionPackage() {
    struct actionPackage* newPackage = (struct actionPackage*)malloc(sizeof(struct actionPackage));
    newPackage->type = NULL;
    newPackage->subjects = NULL;
    newPackage->secondSubject = NULL;
    newPackage->items = NULL;
    newPackage->counts = NULL;
    newPackage->location = NULL;
    return newPackage;
}

void freeActionPackage(struct actionPackage* package) {
    if (package == NULL) {
        return;
    }
    free(package->type);
    package->type = NULL;
    freeStringList(package->subjects);
    free(package->secondSubject);
    package->secondSubject = NULL;
    freeStringList(package->items);
    freeIntList(package->counts);
    free(package->location);
    package->location = NULL;
    free(package);
    package = NULL;
}

struct actionPackageList {
    struct actionPackage* package;
    struct actionPackageList* next;
};

struct actionPackageList* newActionPackageList(struct actionPackage* package) {
    struct actionPackageList* newPackage = (struct actionPackageList*)malloc(sizeof(struct actionPackageList));
    newPackage->package = package;
    newPackage->next = NULL;
    return newPackage;
}

void freeActionPackageList(struct actionPackageList* head) {
    if (head == NULL) {
        return;
    }
    struct actionPackageList* current = head;
    while (current != NULL) {
        struct actionPackageList* next = current->next;
        freeActionPackage(current->package);
        free(current);
        current = next;
    }
}

// Condition package

struct conditionPackage {
    char* type;
    struct stringList* subjects;
    struct stringList* items;
    struct intList* counts;
    char* location;
};

struct conditionPackage* newConditionPackage() {
    struct conditionPackage* newPackage = (struct conditionPackage*)malloc(sizeof(struct conditionPackage));
    newPackage->type = NULL;
    newPackage->subjects = NULL;
    newPackage->items = NULL;
    newPackage->counts = NULL;
    newPackage->location = NULL;
    return newPackage;
}

void freeConditionPackage(struct conditionPackage* package) {
    if (package == NULL) {
        return;
    }
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

struct conditionPackageList {
    struct conditionPackage* package;
    struct conditionPackageList* next;
};

struct conditionPackageList* newConditionPackageList(struct conditionPackage* package) {
    struct conditionPackageList* newPackage = (struct conditionPackageList*)malloc(sizeof(struct conditionPackageList));
    newPackage->package = package;
    newPackage->next = NULL;
    return newPackage;
}

void freeConditionPackageList(struct conditionPackageList* head) {
    if (head == NULL) {
        return;
    }
    struct conditionPackageList* current = head;
    while (current != NULL) {
        struct conditionPackageList* next = current->next;
        freeConditionPackage(current->package);
        free(current);
        current = next;
    }
}

// Sentence package

struct sentencePackage {
    struct conditionPackageList* conditions; // if null, action; otherwise if
    struct actionPackageList* actions;
};

struct sentencePackage* newSentencePackage() {
    struct sentencePackage* newPackage = (struct sentencePackage*)malloc(sizeof(struct sentencePackage));
    newPackage->conditions = NULL;
    newPackage->actions = NULL;
    return newPackage;
}

void freeSentencePackage(struct sentencePackage* package) {
    if (package == NULL) {
        return;
    }
    freeConditionPackageList(package->conditions);
    freeActionPackageList(package->actions);
    free(package);
    package = NULL;
}

struct sentencePackageList {
    struct sentencePackage* package;
    struct sentencePackageList* next;
};

struct sentencePackageList* newSentencePackageList(struct sentencePackage* package) {
    struct sentencePackageList* newPackage = (struct sentencePackageList*)malloc(sizeof(struct sentencePackageList));
    newPackage->package = package;
    newPackage->next = NULL;
    return newPackage;
}

void freeSentencePackageList(struct sentencePackageList* head) {
    if (head == NULL) {
        return;
    }
    struct sentencePackageList* current = head;
    while (current != NULL) {
        struct sentencePackageList* next = current->next;
        freeSentencePackage(current->package);
        free(current);
        current = next;
    }
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
        


        char* input_words[MAX_CHAR_COUNT];
        int inputWordsCount = 0;

        char* token = strtok(input, " ");

        while (token != NULL) {
            input_words[inputWordsCount] = token;
            inputWordsCount++;
            token = strtok(NULL, " ");
        }
        
        printf("Input words count: %d\n", inputWordsCount);

        if (inputWordsCount == 1 && strcmp(input_words[0], "exit") == 0) {
            break;
        }
            
        
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
