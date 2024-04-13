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
    struct subject* newSubject = (struct subject*)malloc(sizeof(struct subject));
    newSubject->name = malloc(strlen(name) + 1);
    strcpy(newSubject->name, name);
    newSubject->location = malloc(strlen("NOWHERE") + 1);
    strcpy(newSubject->location, "NOWHERE");
    for (int i = 0; i < MAX_INVERTORY_SIZE; i++) {
        newSubject->inventory[i] = 0;
    }
    return newSubject;
};

struct subject* findSubject(struct node* head, char* name) { 
    struct node* current = head;
    struct node* previous = NULL;
    while (current != NULL) {
        if (strcmp(current->person->name, name) == 0) {
            return current->person;
        }
        previous = current;
        current = current->next;
    }
    struct subject* newSubject = createSubject(name);
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->person = newSubject;
    newNode->next = NULL;
    previous->next = newNode;
    return newSubject;
};

struct questionPackage* handleWho(char** inputWords, int inputWordsCount);
struct questionPackage* handleWhere(char** inputWords, int inputWordsCount);
struct questionPackage* handleSingleTotal(char** inputWords, int inputWordsCount);
struct questionPackage* handleSingleTotalwithoutItem(char** inputWords, int inputWordsCount);
struct questionPackage* handleSingleTotalwithItem(char** inputWords, int inputWordsCount);
struct questionPackage* handleMultipleTotal(char** inputWords, int inputWordsCount);

void changeLocation(struct subject* person, char* newLocation) {
    free(person->location);
    person->location = malloc((strlen(newLocation) + 1) * sizeof(char));
    strcpy(person->location, newLocation);
}

struct node* addNode(struct node* parent, char* name) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->person = createSubject(name);
    newNode->next = NULL;
    parent->next = newNode;
    return newNode;
}

// Item functions

void addItem(char* item, char** itemList, int* itemListCount) {
    itemList[*itemListCount] = malloc(strlen(item) + 1);
    strcpy(itemList[*itemListCount], item);
    (*itemListCount)++;
}

int getItemIndex(char* item, char** itemList, int *itemListCount) {
    for (int i = 0; i < *itemListCount; i++) {
        if (strcmp(item, itemList[i]) == 0) {
            return i;
        }
    }
    addItem(item, itemList, itemListCount);
    return *itemListCount - 1;
}

int getItemCount(int itemIndex, struct subject* person) {
    return person->inventory[itemIndex];
}

void changeItemCount(int itemIndex, struct subject* person, int diff) {
    person->inventory[itemIndex] += diff;
}


// String check functions
//TODO: contains or equals

int strContainsKeyword(char* word) {
    for (int i = 0; i < 19; i++) {
        if (strcmp(word, keywords[i]) == 0) {
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

int isStrNumber(char* word) {
    for (int i = 0; i < strlen(word); i++) {
        if (!((word[i] >= '0' && word[i] <= '9'))) {
            return 0;
        }
    }
    return 1;
}

int strToNumber(char* word){
    int sum =0;
    for (int i = 0; i < strlen(word); i++) {
        sum = sum*10;
        sum+=(int)(word[i]-'0');
    }
    return sum;
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
    
int stringListLength(struct stringList* root) {
    struct stringList* current = root;
    int length = 0;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

void freeStringList(struct stringList* head) {
    if (head == NULL) {
        return;
    }
    struct stringList* current = head;
    while (current != NULL) {
        struct stringList* next = current->next;
        if (current->string != NULL) {
            free(current->string);
            current->string = NULL;
        }
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
    if (package->type != NULL) {
        free(package->type);
        package->type = NULL;
    }
    freeStringList(package->subjects);
    if (package->location != NULL) {
        free(package->location);
        package->location = NULL;
    }
    if (package->item != NULL) {
        free(package->item);
        package->item = NULL;
    }
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
                printf(" and %s", current->string);
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
        if (isFirstItem) {
            printf("NOTHING");
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
    if (package->type != NULL) {
        free(package->type);
        package->type = NULL;
    }
    freeStringList(package->subjects);
    if (package->secondSubject != NULL) {
        free(package->secondSubject);
        package->secondSubject = NULL;
    }
    freeStringList(package->items);
    freeIntList(package->counts);
    if (package->location != NULL) {
        free(package->location);
        package->location = NULL;
    }
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
    if (package->type != NULL) {
        free(package->type);
        package->type = NULL;
    }
    freeStringList(package->subjects);
    freeStringList(package->items);
    freeIntList(package->counts);
    if (package->location != NULL) {
        free(package->location);
        package->location = NULL;
    }
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

// Condition runner

int runAtCondition(struct conditionPackage* package, struct node* subjectsHead);
int runMultipleHasCondition(struct conditionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount);
int runSingleHasCondition(struct subject person, int itemIndex, char* type, int count);
    
int runCondition(struct conditionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    if (package->type == NULL) {
        return 1;
    }
    if (strcmp(package->type, "at") == 0) {
        return runAtCondition(package, subjectsHead);
    } else if (strcmp(package->type, "has") == 0 
    || strcmp(package->type, "more") == 0 
    || strcmp(package->type, "less") == 0) {
        return runMultipleHasCondition(package, subjectsHead, itemList, itemListCount);
    } else {
        printf("logic error at condition runner\n");
    }
}

int runAtCondition(struct conditionPackage* package, struct node* subjectsHead) {
    struct stringList* current = package->subjects;
    while (current != NULL) {
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        if (currentSubject == NULL) {
            return 0;
        }
        if (strcmp(currentSubject->location, package->location) != 0) {
            return 0;
        }
        current = current->next;
    }
    return 1;
}

int runMultipleHasCondition(struct conditionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    struct stringList* current = package->subjects;
    while (current != NULL) {
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        struct stringList* currentItem = package->items;
        struct intList* currentCount = package->counts;
        while (currentItem != NULL) {
            int itemIndex = getItemIndex(currentItem->string, itemList, itemListCount);
            int singleHasCondition = runSingleHasCondition(*currentSubject, itemIndex, package->type, currentCount->number);
            if (singleHasCondition == 0) {
                return 0;
            }
            currentItem = currentItem->next;
            currentCount = currentCount->next;
        }
        current = current->next;
    }
    return 1;
}
        
int runSingleHasCondition(struct subject person, int itemIndex, char* type, int count) {
    if (strcmp(type, "has") == 0) {
        return person.inventory[itemIndex] == count;
    } else if (strcmp(type, "more") == 0) {
        return person.inventory[itemIndex] > count;
    } else if (strcmp(type, "less") == 0) {
        return person.inventory[itemIndex] < count;
    } else {
        printf("logic error at runSingleHasCondition\n");
    }
}
    
// Condition list runner

int runConditionList(struct conditionPackageList* head, struct node* subjectsHead, char** itemList, int* itemListCount) {
    struct conditionPackageList* current = head;
    while (current != NULL) {
        int currentCondition = runCondition(current->package, subjectsHead, itemList, itemListCount);
        if (currentCondition == 0) {
            return 0;
        }
        current = current->next;
    }
    return 1;
}

// Action runner

void runGoAction(struct actionPackage* package, struct node* subjectsHead);
void runBuyAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount);
void runBuyFromAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount);
void runSellAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount);
void runSellToAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount);

void runAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    if (strcmp(package->type, "go") == 0) {
        runGoAction(package, subjectsHead);
    } else if (strcmp(package->type, "buy") == 0) {
        if (package->secondSubject == NULL) {
            runBuyAction(package, subjectsHead, itemList, itemListCount);
        } else {
            runBuyFromAction(package, subjectsHead, itemList, itemListCount);
        }
    } else if (strcmp(package->type, "sell") == 0) {
        if (package->secondSubject == NULL) {
            runSellAction(package, subjectsHead, itemList, itemListCount);
        } else {
            runSellToAction(package, subjectsHead, itemList, itemListCount);
        }
    } else {
        printf("logic error at action runner\n");
    }
}

void runGoAction(struct actionPackage* package, struct node* subjectsHead) {
    struct stringList* current = package->subjects;
    while (current != NULL) {
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        changeLocation(currentSubject, package->location);
        current = current->next;
    }
}

void runBuyAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    struct stringList* current = package->subjects;
    while (current != NULL) {
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        struct stringList* currentItem = package->items;
        struct intList* currentCount = package->counts;
        while (currentItem != NULL) {
            int itemIndex = getItemIndex(currentItem->string, itemList, itemListCount);
            changeItemCount(itemIndex, currentSubject, currentCount->number);
            currentItem = currentItem->next;
            currentCount = currentCount->next;
        }
        current = current->next;
    }
}

void runBuyFromAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    int buyerCount = stringListLength(package->subjects);
    struct subject* seller = findSubject(subjectsHead, package->secondSubject);
    struct stringList* currentItem = package->items;
    struct intList* currentCount = package->counts;
    // first check if seller has enough items
    while (currentItem != NULL) {
        int itemIndex = getItemIndex(currentItem->string, itemList, itemListCount);
        if (runSingleHasCondition(*seller, itemIndex, "less", currentCount->number * buyerCount) == 1) {
            return;
        }
        currentItem = currentItem->next;
        currentCount = currentCount->next;
    }
    // reset currentCount and currentItem
    currentItem = package->items;
    currentCount = package->counts;
    // then buy
    struct stringList* current = package->subjects;
    while (current != NULL) {
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        while (currentItem != NULL) {
            int itemIndex = getItemIndex(currentItem->string, itemList, itemListCount);
            changeItemCount(itemIndex, currentSubject, currentCount->number);
            changeItemCount(itemIndex, seller, (-1) * currentCount->number);
            currentItem = currentItem->next;
            currentCount = currentCount->next;
        }
        current = current->next;
    }
}

void runSellAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    struct stringList* current = package->subjects;
    struct stringList* currentItem = package->items;
    struct intList* currentCount = package->counts;
    // first check if sellers have enough items
    while (current != NULL) {
        struct subject* currentSubject = findSubject(subjectsHead, current->string);

        while (currentItem != NULL) {
            int itemIndex = getItemIndex(currentItem->string, itemList, itemListCount);
            if (runSingleHasCondition(*currentSubject, itemIndex, "less", currentCount->number) == 1) {
                return;
            }
            currentItem = currentItem->next;
            currentCount = currentCount->next;
        }
        current = current->next;
    }
    // reset current, currentCount and currentItem
    current = package->subjects;
    currentItem = package->items;
    currentCount = package->counts;
    // then sell
    while (current != NULL) {
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        while (currentItem != NULL) {
            int itemIndex = getItemIndex(currentItem->string, itemList, itemListCount);
            changeItemCount(itemIndex, currentSubject, (-1) * currentCount->number);
            currentItem = currentItem->next;
            currentCount = currentCount->next;
        }
        current = current->next;
    }
}

void runSellToAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    struct stringList* current = package->subjects;
    struct stringList* currentItem = package->items;
    struct intList* currentCount = package->counts;
    // first check if sellers have enough items
    while (current != NULL) {
        struct subject* currentSubject = findSubject(subjectsHead, current->string);

        while (currentItem != NULL) {
            int itemIndex = getItemIndex(currentItem->string, itemList, itemListCount);
            if (runSingleHasCondition(*currentSubject, itemIndex, "less", currentCount->number) == 1) {
                printf("Not eligable to sell in sell\n");
                return;
            }
            currentItem = currentItem->next;
            currentCount = currentCount->next;
        }
        current = current->next;
    }
    // reset current, currentCount and currentItem
    current = package->subjects;
    currentItem = package->items;
    currentCount = package->counts;
    // then sell
    struct subject* buyer = findSubject(subjectsHead, package->secondSubject);
    while (current != NULL) {
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        while (currentItem != NULL) {
            int itemIndex = getItemIndex(currentItem->string, itemList, itemListCount);
            changeItemCount(itemIndex, currentSubject, (-1) * currentCount->number);
            changeItemCount(itemIndex, buyer, currentCount->number);
            currentItem = currentItem->next;
            currentCount = currentCount->next;
        }
        current = current->next;
    }
}

// Action list runner

void runActionList(struct actionPackageList* head, struct node* subjectsHead, char** itemList, int* itemListCount) {
    struct actionPackageList* current = head;
    while (current != NULL) {
        runAction(current->package, subjectsHead, itemList, itemListCount);
        current = current->next;
    }
}

// Sentence runner

void runSentence(struct sentencePackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    if (package->conditions == NULL) {
        runActionList(package->actions, subjectsHead, itemList, itemListCount);
    } else {
        if (runConditionList(package->conditions, subjectsHead, itemList, itemListCount) == 1) {
            runActionList(package->actions, subjectsHead, itemList, itemListCount);
        }
    }
}

// Sentence list runner

void runSentenceList(struct sentencePackageList* head, struct node* subjectsHead, char** itemList, int* itemListCount) {
    struct sentencePackageList* current = head;
    while (current != NULL) {
        runSentence(current->package, subjectsHead, itemList, itemListCount);
        current = current->next;
    }
}

// print sentence list all the way inside

void printSentenceList(struct sentencePackageList* head) {
    struct sentencePackageList* current = head;
    printf("************\n");
    while (current != NULL) {
        struct sentencePackage* currentPackage = current->package;
        struct conditionPackageList* currentConditions = currentPackage->conditions;
        struct actionPackageList* currentActions = currentPackage->actions;
        printf("Conditions: \n");
        while (currentConditions != NULL) {
            struct conditionPackage* currentCondition = currentConditions->package;
            printf("Type: %s, Subjects: ", currentCondition->type);
            struct stringList* currentSubjects = currentCondition->subjects;
            while (currentSubjects != NULL) {
                printf("%s ", currentSubjects->string);
                currentSubjects = currentSubjects->next;
            }
            printf("Items: ");
            struct stringList* currentItems = currentCondition->items;
            while (currentItems != NULL) {
                printf("%s ", currentItems->string);
                currentItems = currentItems->next;
            }
            printf("Counts: ");
            struct intList* currentCounts = currentCondition->counts;
            while (currentCounts != NULL) {
                printf("%d ", currentCounts->number);
                currentCounts = currentCounts->next;
            }
            printf("Location: %s\n", currentCondition->location);
            currentConditions = currentConditions->next;
            printf("\n");
        }
        printf("Actions: \n");
        while (currentActions != NULL) {
            struct actionPackage* currentAction = currentActions->package;
            printf("Type: %s, Subjects: ", currentAction->type);
            struct stringList* currentSubjects = currentAction->subjects;
            while (currentSubjects != NULL) {
                printf("%s ", currentSubjects->string);
                currentSubjects = currentSubjects->next;
            }
            printf("Second Subject: %s, Items: ", currentAction->secondSubject);
            struct stringList* currentItems = currentAction->items;
            while (currentItems != NULL) {
                printf("%s ", currentItems->string);
                currentItems = currentItems->next;
            }
            printf("Counts: ");
            struct intList* currentCounts = currentAction->counts;
            while (currentCounts != NULL) {
                printf("%d ", currentCounts->number);
                currentCounts = currentCounts->next;
            }
            printf("Location: %s\n", currentAction->location);
            currentActions = currentActions->next;
            printf("\n");
        }
        current = current->next;
    }
}

int checkBuy(char** inputWords,int* currentWordIndex,int inputWordsCount,struct actionPackage* action){
    int validCheck = 0;
    struct stringList* objectlist= newStringList("if");
    struct intList* numberlist= newIntList(0);
    do{
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if(!isStrNumber(inputWords[(*currentWordIndex)])){
            if(validCheck==0){
                break;
            }else{
                validCheck=1;
                (*currentWordIndex)--;
                return 1;
            }
        }
        addInt(numberlist,strToNumber(inputWords[(*currentWordIndex)]));
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if(!strFormatTrue(inputWords[(*currentWordIndex)])){
            validCheck=0;
            break;
        }
        if(addString(objectlist,inputWords[(*currentWordIndex)])==0){
            validCheck=0;
            break;
        }
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            validCheck= 3;
            break;
        }
        validCheck = 1;
    }while(strcmp("and",inputWords[(*currentWordIndex)])==0);
    if(validCheck==0){
        return 0;
    }
    action->items=objectlist->next;
    action->counts= numberlist->next;
    free(objectlist->string);
    objectlist->string=NULL;
    free(objectlist);
    free(numberlist);
    if(validCheck==3){
        return 3;
    }
    if(strcmp("from",inputWords[(*currentWordIndex)])==0){
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if(!strFormatTrue(inputWords[(*currentWordIndex)])){
            return 0;
        }
        char* string = inputWords[(*currentWordIndex)];
        struct stringList* current = action->subjects;
        if (strcmp(current->string, string) == 0) {
            return 0;
        }
        while (current->next != NULL) {
            if (strcmp(current->next->string, string) == 0) {
                return 0;
            }
            current = current->next;
        }
        action->secondSubject=(char*)malloc((strlen(inputWords[(*currentWordIndex)])+1)*sizeof(char));
        strcpy(action->secondSubject,inputWords[(*currentWordIndex)]);
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 3;
        }
    }
    if (strcmp("if",inputWords[(*currentWordIndex)])==0){
        return 2;
    }
    if (strcmp("and",inputWords[(*currentWordIndex)])==0){
        return 1;
    }
    return 0;
}
int checkSell(char** inputWords,int* currentWordIndex,int inputWordsCount,struct actionPackage* action){
    int validCheck = 0;
    struct stringList* objectlist= newStringList("if");
    struct intList* numberlist= newIntList(0);
    do{
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if(!isStrNumber(inputWords[(*currentWordIndex)])){
            if(validCheck==0){
                break;
            }else{
                validCheck=1;
                (*currentWordIndex)--;
                return 1;
            }
        }
        addInt(numberlist,strToNumber(inputWords[(*currentWordIndex)]));
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if(!strFormatTrue(inputWords[(*currentWordIndex)])){
            validCheck=0;
            break;
        }
        if(addString(objectlist,inputWords[(*currentWordIndex)])==0){
            validCheck=0;
            break;
        }
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            validCheck= 3;
            break;
        }
        validCheck = 1;
    }while(strcmp("and",inputWords[(*currentWordIndex)])==0);
    if(validCheck==0){
        return 0;
    }
    action->items=objectlist->next;
    action->counts= numberlist->next;
    free(objectlist->string);
    objectlist->string=NULL;
    free(objectlist);
    free(numberlist);
    if(validCheck==3){
        return 3;
    }
    if(strcmp("to",inputWords[(*currentWordIndex)])==0){
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if(!strFormatTrue(inputWords[(*currentWordIndex)])){
            return 0;
        }
        char* string = inputWords[(*currentWordIndex)];
        struct stringList* current = action->subjects;
        if (strcmp(current->string, string) == 0) {
            return 0;
        }
        while (current->next != NULL) {
            if (strcmp(current->next->string, string) == 0) {
                return 0;
            }
            current = current->next;
        }
        action->secondSubject=(char*)malloc((strlen(inputWords[(*currentWordIndex)])+1)*sizeof(char));
        strcpy(action->secondSubject,inputWords[(*currentWordIndex)]);
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 3;
        }
    }
    if (strcmp("if",inputWords[(*currentWordIndex)])==0){
        return 2;
    }
    if (strcmp("and",inputWords[(*currentWordIndex)])==0){
        return 1;
    }
    return 0;
}
int checkGoTo(char** inputWords,int* currentWordIndex,int inputWordsCount,struct actionPackage* action){
    (*currentWordIndex)++;
    if ((*currentWordIndex)==inputWordsCount){
        return 0;
    }
    if (!strcmp("to",inputWords[(*currentWordIndex)])==0){
        return 0;
    }
    (*currentWordIndex)++;
    if ((*currentWordIndex)==inputWordsCount){
        return 0;
    } 
    if(!strFormatTrue(inputWords[(*currentWordIndex)])){
        return 0;
    }
    action->location=(char*)malloc((strlen(inputWords[(*currentWordIndex)])+1)*sizeof(char));
    strcpy(action->location,inputWords[(*currentWordIndex)]);
    (*currentWordIndex)++;
    if ((*currentWordIndex)==inputWordsCount){
        return 3;
    }
    if (strcmp("if",inputWords[(*currentWordIndex)])==0){
        return 2;
    }
    if (strcmp("and",inputWords[(*currentWordIndex)])==0){
        return 1;
    }
    return 0;
}
int checkAction(char** inputWords,int* currentWordIndex,int inputWordsCount,struct actionPackage* action){
    int validCheck = 1;
    struct stringList* subjectlist= newStringList("if");
    do{
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if(!strFormatTrue(inputWords[(*currentWordIndex)])){
            validCheck=0;
            break;
        }
        if(addString(subjectlist,inputWords[(*currentWordIndex)])==0){
            validCheck=0;
            break;
        }
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
    }while(strcmp("and",inputWords[(*currentWordIndex)])==0);
    
    if(validCheck==0){
        return 0;
    }
    action->subjects=subjectlist->next;
    free(subjectlist->string);
    subjectlist->string=NULL;
    free(subjectlist);
    if(strcmp("buy",inputWords[(*currentWordIndex)])==0){
        action->type = (char*)malloc(4*sizeof(char));
        strcpy(action->type,"buy");
        return checkBuy(inputWords,currentWordIndex, inputWordsCount,action);
    }else if(strcmp("sell",inputWords[(*currentWordIndex)])==0){
        action->type = (char*)malloc(5*sizeof(char));
        strcpy(action->type,"sell");
        return checkSell(inputWords,currentWordIndex, inputWordsCount,action);
    }else if(strcmp("go",inputWords[(*currentWordIndex)])==0){
        action->type = (char*)malloc(3*sizeof(char));
        strcpy(action->type,"go");
        return checkGoTo(inputWords,currentWordIndex,inputWordsCount,action);
    }else{
        return 0;
    }

}
int checkHas(char** inputWords,int* currentWordIndex,int inputWordsCount,struct conditionPackage* condition){
    (*currentWordIndex)++;
    if ((*currentWordIndex)==inputWordsCount){
        return 0;
    }
    if (strcmp("more",inputWords[(*currentWordIndex)])==0){
        condition->type = (char*)malloc(5*sizeof(char));
        strcpy(condition->type,"more");
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if (!strcmp("than",inputWords[(*currentWordIndex)])==0){
            return 0;
        }
    }else if (strcmp("less",inputWords[(*currentWordIndex)])==0){
        condition->type = (char*)malloc(5*sizeof(char));
        strcpy(condition->type,"less");
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if (!strcmp("than",inputWords[(*currentWordIndex)])==0){
            return 0;
        }
    }else{
        condition->type = (char*)malloc(4*sizeof(char));
        strcpy(condition->type,"has");
        (*currentWordIndex)--;
    }
    int validCheck = 0;
    struct stringList* objectlist= newStringList("if");
    struct intList* numberlist= newIntList(0);
    do{
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if(!isStrNumber(inputWords[(*currentWordIndex)])){
            if(validCheck==0){
                break;
            }else{
                validCheck=1;
                (*currentWordIndex)--;
                return 1;
            }
        }
        addInt(numberlist,strToNumber(inputWords[(*currentWordIndex)]));
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if(!strFormatTrue(inputWords[(*currentWordIndex)])){
            validCheck=0;
            break;
        }
        if(addString(objectlist,inputWords[(*currentWordIndex)])==0){
            validCheck=0;
            break;
        }
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            validCheck= 3;
            break;
        }
        validCheck = 1;
    }while(strcmp("and",inputWords[(*currentWordIndex)])==0);
    condition->items=objectlist->next;
    condition->counts= numberlist->next;
    free(objectlist->string);
    objectlist->string=NULL;
    free(objectlist);
    free(numberlist);
    return validCheck;
}
int checkAt(char** inputWords,int* currentWordIndex,int inputWordsCount,struct conditionPackage* condition){
    (*currentWordIndex)++;
    if ((*currentWordIndex)==inputWordsCount){
        return 0;
    } 
    if(!strFormatTrue(inputWords[(*currentWordIndex)])){
        return 0;
    }
    condition->location = (char*)malloc((strlen(inputWords[(*currentWordIndex)])+1)*sizeof(char));
    strcpy(condition->location,inputWords[(*currentWordIndex)]);
    (*currentWordIndex)++;
    if ((*currentWordIndex)==inputWordsCount){
        return 3;
    }
    if (strcmp("and",inputWords[(*currentWordIndex)])==0){
        return 1;
    }
    return 0;
}
int checkCondition(char** inputWords,int* currentWordIndex,int inputWordsCount,struct conditionPackage* condition){
    int validCheck = 1;
    int sentenceControl= (*currentWordIndex);
    struct stringList* subjectlist= newStringList("if");
    do{
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if(!strFormatTrue(inputWords[(*currentWordIndex)])){
            validCheck=0;
            break;
        }
        if(addString(subjectlist,inputWords[(*currentWordIndex)])==0){
            validCheck=0;
            break;
        }
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
    }while(strcmp("and",inputWords[(*currentWordIndex)])==0);
    
    if(validCheck==0){
        return 0;
    }
    condition->subjects=subjectlist->next;
    free(subjectlist->string);
    subjectlist->string=NULL;
    free(subjectlist);
    if(strcmp("has",inputWords[(*currentWordIndex)])==0){
        return checkHas(inputWords,currentWordIndex, inputWordsCount,condition);
    }else if(strcmp("at",inputWords[(*currentWordIndex)])==0){
        return checkAt(inputWords,currentWordIndex, inputWordsCount,condition);
        condition->type = (char*)malloc(3*sizeof(char));
        strcpy(condition->type,"at");
    }else if((strcmp("buy",inputWords[(*currentWordIndex)])==0)||(strcmp("sell",inputWords[(*currentWordIndex)])==0)||(strcmp("go",inputWords[(*currentWordIndex)])==0)){
        (*currentWordIndex)=sentenceControl;
        return 2;
    }else{
        return 0;
    }
}
int checkSentence(char** inputWords,int* currentWordIndex,int inputWordsCount,struct sentencePackage* package){
    package->actions = newActionPackageList(newActionPackage());
    struct actionPackageList* action = package->actions;
    int validCheck=1;
    while (1){
        validCheck=checkAction(inputWords,currentWordIndex,inputWordsCount,action->package);
        if(validCheck==0){//invalid
            return 0;
        }else if(validCheck==1){//no problem continue with next action
            action->next=newActionPackageList(newActionPackage());
            action=action->next;
            continue;
        }else if(validCheck==2){//there is a if so go on with condition part
            break;
        }else if(validCheck==3){//we have reached the last word and sentence is completed
            return 3;
        }
    }
    package->conditions = newConditionPackageList(newConditionPackage());
    struct conditionPackageList* condition = package->conditions;
    int ifCheck = 0;
    while (1){
        validCheck=checkCondition(inputWords,currentWordIndex,inputWordsCount,condition->package);
        if(validCheck==0){//invalid
            return 0;
        }else if(validCheck==1){//no problem continue with next condition
            condition->next = newConditionPackageList(newConditionPackage());
            condition=condition->next;
            ifCheck=1;
            continue;
        }else if(validCheck==2){//condition part is finished so sentence is completed
            if (ifCheck==0){
                return 0;
            }
            break;
        }else if(validCheck==3){//we have reached the last word and sentence is completed
            return 3;
        }
    }
    return 1;
}

struct sentencePackageList* checkPhrase(char** inputWords, int* currentWordIndex,int inputWordsCount){

    struct sentencePackageList* root = newSentencePackageList(newSentencePackage());
    struct sentencePackageList* current = root;
    
    int phraseConclusion;
    while(1){
        phraseConclusion=checkSentence(inputWords,currentWordIndex,inputWordsCount,current->package);
        if(phraseConclusion==0){//invalid
            break;
        }else if(phraseConclusion==1){//a valid sectence
            current->next = newSentencePackageList(newSentencePackage());
            current = current->next;
            continue;
        }else if(phraseConclusion==3){//the last sentence
            break;
        }
    }
    if (phraseConclusion==0){
        printf("INVALID\n");
        freeSentencePackageList(root);
        root = NULL;
        return NULL;
    }else{
        printf("OK\n");
        return root;
    }
}

void main() {

    char** itemList = (char**)calloc(MAX_INVERTORY_SIZE, sizeof(char*));
    int* itemListCount = (int*)malloc(sizeof(int));
    *itemListCount = 0;
    struct node* subjects = (struct node*)malloc(sizeof(struct node));
    subjects->person = createSubject("NOBODY");
    subjects->next = NULL;
    // for syntax check iteration
    int currentWordIndexValue = -1;
    int* currentWordIndex = &currentWordIndexValue;

    
    while (1) { // shell loop
        char input[MAX_CHAR_COUNT + 2]; // +2 for newline and null terminator
        printf(">>"); // shell prompt
        fgets(input, MAX_CHAR_COUNT + 2, stdin); // +2 for newline and null terminator
        
        input[strlen(input) - 1] = '\0'; //remove newline
        


        char* inputWords[MAX_CHAR_COUNT];
        int inputWordsCount = 0;

        char* token = strtok(input, " ");

        while (token != NULL) {
            inputWords[inputWordsCount] = token;
            inputWordsCount++;
            token = strtok(NULL, " ");
        }
        

        if (inputWordsCount == 1 && strcmp(inputWords[0], "exit") == 0) {
            break;
        }
            

        if (strcmp(inputWords[inputWordsCount-1], "?") == 0) {
            struct questionPackage* package = parseQuestion(inputWords, inputWordsCount);
            runQuestion(package, subjects, itemList, itemListCount);
            freeQuestionPackage(package);
        } else {
            *currentWordIndex = -1;
            struct sentencePackageList* packageList = checkPhrase(inputWords, currentWordIndex, inputWordsCount);
            if (packageList == NULL) {
                continue;
            }
            runSentenceList(packageList, subjects, itemList, itemListCount);
            freeSentencePackageList(packageList);
        }
    }
}
