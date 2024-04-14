#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR_COUNT 1024 //max input length for an input line
#define MAX_INVERTORY_SIZE 17000

char* keywords[] =  {"sell", "buy", "go", "to", "from",
 "and", "at", "has", "if", "less", "more", "than", "exit", "where",
  "total", "who","NOBODY", "NOTHING", "NOWHERE"};

// struct for charachters 
struct subject {
    char* name;
    char* location;
    int inventory[MAX_INVERTORY_SIZE]; //TODO: check numbers
};

// linked list for subjects
struct node {
    struct subject* person;
    struct node* next;
};

/* Subject and Node functions */

// create a new subject
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

// find  a subject from the linked list if it is not found create a new one and add it to the list
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

// change location of a subject
void changeLocation(struct subject* person, char* newLocation) {
    free(person->location);
    person->location = malloc((strlen(newLocation) + 1) * sizeof(char));
    strcpy(person->location, newLocation);
}

// add a new node to the linked list
struct node* addNode(struct node* parent, char* name) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->person = createSubject(name);
    newNode->next = NULL;
    parent->next = newNode;
    return newNode;
}

/* Item functions */

// add an item's name to the item list to keep track of the items like a dictionary that uses parallel arrays
void addItem(char* item, char** itemList, int* itemListCount) {
    itemList[*itemListCount] = malloc(strlen(item) + 1);
    strcpy(itemList[*itemListCount], item);
    (*itemListCount)++;
}

// get the index of an item in the item list if it is not found add it to the list
int getItemIndex(char* item, char** itemList, int *itemListCount) {
    for (int i = 0; i < *itemListCount; i++) {
        if (strcmp(item, itemList[i]) == 0) {
            return i;
        }
    }
    addItem(item, itemList, itemListCount);
    return *itemListCount - 1;
}

// get the count of an item in a subject's inventory using the item's index in the item list
int getItemCount(int itemIndex, struct subject* person) {
    return person->inventory[itemIndex];
}

// change the count of an item in a subject's inventory using the item's index in the item list
void changeItemCount(int itemIndex, struct subject* person, int diff) {
    person->inventory[itemIndex] += diff;
}

/* String check functions */

// check if a string is a keyword
int strContainsKeyword(char* word) {
    for (int i = 0; i < 19; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// check if a string contains only letters and underscores
int strOnlyLettersandUnderscores(char* word) {
    for (int i = 0; i < strlen(word); i++) {
        if (!((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z') || word[i] == '_')) {
            return 0;
        }
    }
    return 1;
}

// check if a string is in the correct format for an entity name
int strFormatTrue(char* word) {
    return !strContainsKeyword(word) && strOnlyLettersandUnderscores(word);
}

// check if a string is a number
int isStrNumber(char* word) {
    for (int i = 0; i < strlen(word); i++) {
        if (!((word[i] >= '0' && word[i] <= '9'))) {
            return 0;
        }
    }
    return 1;
}

// convert a string to a number
int strToNumber(char* word){
    int sum =0;
    for (int i = 0; i < strlen(word); i++) {
        sum = sum*10;
        sum+=(int)(word[i]-'0');
    }
    return sum;
}

/* String list */

// struct for a string holding linked list
struct stringList {
    char* string;
    struct stringList* next;
};

// create a new string list
struct stringList* newStringList(char* string) {
    struct stringList* newString = (struct stringList*)malloc(sizeof(struct stringList));
    newString->string = (char*)malloc(strlen(string) + 1);
    strcpy(newString->string, string);
    newString->next = NULL;
    return newString;
}

// add a string to the string list if it is not already in the list if it is already in the list return 0 that means it is not unique
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
    
// get the length of the string list
int stringListLength(struct stringList* root) {
    struct stringList* current = root;
    int length = 0;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

// free the string list and its strings in the memory to prevent memory leaks if not Null
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

/* Question package */

// struct for a question package that holds the type of the question, the subjects, the location and the item
struct questionPackage {
    char* type;
    struct stringList* subjects;
    char* location;
    char* item;
};

// free the question package and its contents in the memory to prevent memory leaks if not Null 
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

/* Question parser */

// implicit declaration of functions
struct questionPackage* handleWho(char** inputWords, int inputWordsCount);
struct questionPackage* handleWhere(char** inputWords, int inputWordsCount);
struct questionPackage* handleSingleTotal(char** inputWords, int inputWordsCount);
struct questionPackage* handleSingleTotalwithoutItem(char** inputWords, int inputWordsCount);
struct questionPackage* handleSingleTotalwithItem(char** inputWords, int inputWordsCount);
struct questionPackage* handleMultipleTotal(char** inputWords, int inputWordsCount);

// parse question and return a question package to be used in the question runner
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

// check the syntax of who question type and return a question package to be used in the question runner
// if the syntax is not correct return NULL to indicate that the question is invalid
// this explanation is valid for the other question types as well
struct questionPackage* handleWho(char** inputWords, int inputWordsCount) {
    struct questionPackage* package = (struct questionPackage*)malloc(sizeof(struct questionPackage));
    package->type = malloc(strlen("who") + 1);
    strcpy(package->type, "who");
    package->subjects = NULL;
    package->location = NULL;
    package->item = NULL;

    if (inputWordsCount != 4) { // input words count should be 4 for a who question
        return NULL;
    }
    if (strcmp(inputWords[1],"at") != 0) { // the second word should be "at"
        return NULL;
    }
    if (strFormatTrue(inputWords[2]) == 0) { // the third word should be a location name in the correct format
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

    if (inputWordsCount != 3) { // input words count should be 3 for a where question
        return NULL;
    }
    if (strFormatTrue(inputWords[0]) == 0) { // the first word should be a subject name in the correct format
        return NULL;
    }
    package->subjects = (struct stringList*)malloc(sizeof(struct stringList));
    package->subjects->string = malloc(strlen(inputWords[0]) + 1);
    strcpy(package->subjects->string, inputWords[0]);
    package->subjects->next = NULL;
    return package;
}

struct questionPackage* handleSingleTotal(char** inputWords, int inputWordsCount) {
    if (inputWordsCount ==3) { // if the input words count is 3, it is a single subject total without item question that asks the total inventory of a subject
        return handleSingleTotalwithoutItem(inputWords, inputWordsCount);
    } else if (inputWordsCount == 4) { // if the input words count is 4, it is a single subject total with item question that asks the inventory of a specific item of a subject
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

    if (strFormatTrue(inputWords[0]) == 0) { // the first word should be a subject name in the correct format
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

    if (strFormatTrue(inputWords[0]) == 0) {  // the first word should be a subject name in the correct format
        return NULL;
    }
    package->subjects = (struct stringList*)malloc(sizeof(struct stringList));
    package->subjects->string = malloc(strlen(inputWords[0]) + 1);
    strcpy(package->subjects->string, inputWords[0]);
    package->subjects->next = NULL;

    if (strFormatTrue(inputWords[2]) == 0) {  // the third word should be an item name in the correct format
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

    if (strFormatTrue(inputWords[0]) == 0) {  // the first word should be a subject name in the correct format
        return NULL;
    }
    package->subjects = newStringList(inputWords[0]);  // add the first subject to the subject list

    struct stringList* root = package->subjects;

    for (int i = 1; i < inputWordsCount-3; i += 2) { // loop through the input words to add the rest of the subjects to the subject list
        if (strcmp(inputWords[i],"and") == 0) {  // the word should be "and" to separate the subjects at the odd indexes
            
            if (strFormatTrue(inputWords[i+1]) == 0) {  // the next word should be a subject name in the correct format at the even indexes
                return NULL;
            }
            int checkUniqueness = addString(root, inputWords[i+1]); // add the subject to the subject list if it is not already in the list if it is already in the list return 0 that means it is not unique
            if (checkUniqueness == 0) { // if the subject is not unique return NULL to indicate that the question is invalid
                return NULL;
            }
            continue;
        }
        return NULL;
    }

    if (strcmp(inputWords[inputWordsCount-3],"total") != 0) { // the word before the last word should be "total"
        return NULL;
    }
    if (strFormatTrue(inputWords[inputWordsCount-2]) == 0) { // the second word before the last word should be an item name in the correct format
        return NULL;
    }
    package->item = malloc(strlen(inputWords[inputWordsCount-2]) + 1);
    strcpy(package->item, inputWords[inputWordsCount-2]);
    return package;
}

/* Question runner */

// run the question and print the answer if the question is invalid print "INVALID"
void runQuestion(struct questionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    if (package == NULL) { // if the package is NULL, the question is invalid so print "INVALID"
        printf("INVALID\n");
        return;
    }
    else if (strcmp(package->type, "who") == 0) { // if the type is "who" run the who question
        struct stringList* peopleAtLocation = NULL;
        struct node* current = subjectsHead;
        while (current != NULL) { // loop through the subjects to find the subjects at the location
            if (strcmp(current->person->location, package->location) == 0) {
                if (peopleAtLocation == NULL) {
                    peopleAtLocation = newStringList(current->person->name);
                } else {
                    int checkwho = addString(peopleAtLocation, current->person->name); 
                    //TODO: check if this is necessary
                    if (checkwho == 0) { // if the subject is not unique (expected to be unique)
                        printf("logic error at who with new string list strcvuteure that implies not unique person at subject list\n");
                    }
                }
            }
            current = current->next; // go to the next subject
        }
        if (peopleAtLocation == NULL) { // if there is no subject at the location print "NOBODY"
            printf("NOBODY\n");
        } else { // print the subjects at the location separated by "and"
            printf("%s", peopleAtLocation->string);
            struct stringList* current = peopleAtLocation->next;
            while (current != NULL) {
                printf(" and %s", current->string);
                current = current->next;
            }
            printf("\n");
        }
    } else if (strcmp(package->type, "where") == 0) { // if the type is "where" run the where question
        struct subject* current = findSubject(subjectsHead, package->subjects->string); // find the subject
        if (current == NULL) { // if the subject is not found print "NOWHERE"
            printf("NOWHERE\n");
        } else { // print the location of the subject
            printf("%s\n", current->location);
        }
    } else if (strcmp(package->type, "singleTotalwithoutItem") == 0) { // if the type is "singleTotalwithoutItem" run the single total without item question
        struct subject* current = findSubject(subjectsHead, package->subjects->string);  // find the subject
        if (current == NULL) { // if the subject is not found print "NOTHING" 
            printf("NOTHING\n");
            return;
        } 
        int isFirstItem = 1; // to check if it is the first item to print "and" or not
        for (int i = 0; i < *itemListCount; i++) {
            if (current->inventory[i] != 0) { // print the items that the subject has with their counts
                if (isFirstItem) {
                    printf("%d %s", current->inventory[i], itemList[i]);
                    isFirstItem = 0;
                } else {
                    printf(" and %d %s", current->inventory[i], itemList[i]);
                }
            }
        }
        if (isFirstItem) { // if the subject has no items print "NOTHING"
            printf("NOTHING");
        } 
        printf("\n");
    } else if (strcmp(package->type, "singleTotalwithItem") == 0) { // if the type is "singleTotalwithItem" run the single total with item question
        struct subject* current = findSubject(subjectsHead, package->subjects->string); 
        if (current == NULL) { // if the subject is not found print "NOTHING"
            printf("NOTHING\n");
            return;
        } 
        int itemIndex = getItemIndex(package->item, itemList, itemListCount); // get the index of the item
        if (itemIndex == -1) { // if the item is not found print "0"
            printf("0\n");
            return;
        }
        printf("%d\n", current->inventory[itemIndex]); // print the count of the item
    } else if (strcmp(package->type, "multipleTotal") == 0) { // if the type is "multipleTotal" run the multiple total question
        struct stringList* current = package->subjects; // get the first subject
        int itemIndex = getItemIndex(package->item, itemList, itemListCount); // get the index of the item
        int total = 0; // to keep track of the total count
        while (current != NULL) { // loop through the subjects to find the total count of the item
            struct subject* currentSubject = findSubject(subjectsHead, current->string);
            if (currentSubject != NULL) {
                total += currentSubject->inventory[itemIndex];
            }
            current = current->next; // go to the next subject
        }
        printf("%d\n", total); // print the total count
    }
    else { // type is invalid
        printf("logic error at question answer\n"); 
    }
}

/* Int list */

// struct for an integer holding linked list
struct intList {
    int number;
    struct intList* next;
};

// create a new integer list
struct intList* newIntList(int number) {
    struct intList* newInt = (struct intList*)malloc(sizeof(struct intList));
    newInt->number = number;
    newInt->next = NULL;
    return newInt;
}

// add an integer to the integer list
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

// free the integer list in the memory to prevent memory leaks if not Null
void freeIntList(struct intList* head) {
    struct intList* current = head;
    while (current != NULL) {
        struct intList* next = current->next;
        free(current);
        current = next;
    }
}

/* Action Package */

// struct for an action package that holds the type of the action, the subjects, the second subject, the items, the counts and the location
struct actionPackage {
    char* type;
    struct stringList* subjects;
    char* secondSubject;
    struct stringList* items;
    struct intList* counts;
    char* location;
};

// opens a new action package 
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

// free the action package and its contents in the memory to prevent memory leaks if not Null
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

/* Action Package List */

// struct for an action linked list
struct actionPackageList {
    struct actionPackage* package;
    struct actionPackageList* next;
};

// create a new action package list
struct actionPackageList* newActionPackageList(struct actionPackage* package) {
    struct actionPackageList* newPackage = (struct actionPackageList*)malloc(sizeof(struct actionPackageList));
    newPackage->package = package;
    newPackage->next = NULL;
    return newPackage;
}

// free the action package list and its contents in the memory to prevent memory leaks if not Null
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

/* Condition package */

// struct for a condition package that holds the type of the condition, the subjects, the items, the counts and the location
struct conditionPackage {
    char* type;
    struct stringList* subjects;
    struct stringList* items;
    struct intList* counts;
    char* location;
};

// create a new condition package
struct conditionPackage* newConditionPackage() {
    struct conditionPackage* newPackage = (struct conditionPackage*)malloc(sizeof(struct conditionPackage));
    newPackage->type = NULL;
    newPackage->subjects = NULL;
    newPackage->items = NULL;
    newPackage->counts = NULL;
    newPackage->location = NULL;
    return newPackage;
}

// free the condition package and its contents in the memory to prevent memory leaks if not Null
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

/* Condition Package List */

// struct for a condition package linked list
struct conditionPackageList {
    struct conditionPackage* package;
    struct conditionPackageList* next;
};

// create a new condition package list
struct conditionPackageList* newConditionPackageList(struct conditionPackage* package) {
    struct conditionPackageList* newPackage = (struct conditionPackageList*)malloc(sizeof(struct conditionPackageList));
    newPackage->package = package;
    newPackage->next = NULL;
    return newPackage;
}

// free the condition package list and its contents in the memory to prevent memory leaks if not Null
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

/* Sentence Package */

// struct for a sentence package that holds the conditions and the actions
struct sentencePackage {
    struct conditionPackageList* conditions; // if null, action; otherwise if
    struct actionPackageList* actions;
};

// create a new sentence package
struct sentencePackage* newSentencePackage() {
    struct sentencePackage* newPackage = (struct sentencePackage*)malloc(sizeof(struct sentencePackage));
    newPackage->conditions = NULL;
    newPackage->actions = NULL;
    return newPackage;
}

// free the sentence package and its contents in the memory to prevent memory leaks if not Null
void freeSentencePackage(struct sentencePackage* package) {
    if (package == NULL) {
        return;
    }
    freeConditionPackageList(package->conditions);
    freeActionPackageList(package->actions);
    free(package);
    package = NULL;
}

/* Sentence Package List */

// struct for a sentence package linked list
struct sentencePackageList {
    struct sentencePackage* package;
    struct sentencePackageList* next;
};

// create a new sentence package list
struct sentencePackageList* newSentencePackageList(struct sentencePackage* package) {
    struct sentencePackageList* newPackage = (struct sentencePackageList*)malloc(sizeof(struct sentencePackageList));
    newPackage->package = package;
    newPackage->next = NULL;
    return newPackage;
}

// free the sentence package list and its contents in the memory to prevent memory leaks if not Null
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

/* Condition runner */

// implicit declaration of functions
int runAtCondition(struct conditionPackage* package, struct node* subjectsHead);
int runMultipleHasCondition(struct conditionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount);
int runSingleHasCondition(struct subject person, int itemIndex, char* type, int count);
    
// run the condition and return 1 if the condition is true, 0 if the condition is false
int runCondition(struct conditionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    if (package->type == NULL) { // if the type is NULL, assume the condition is true
        return 1;
    }
    if (strcmp(package->type, "at") == 0) { // if the type is "at" run the at condition
        return runAtCondition(package, subjectsHead);
    } else if (strcmp(package->type, "has") == 0 
    || strcmp(package->type, "more") == 0 
    || strcmp(package->type, "less") == 0) { // if the type is "has", "more" or "less" run the multiple has condition
        return runMultipleHasCondition(package, subjectsHead, itemList, itemListCount);
    } else { // type is invalid
        printf("logic error at condition runner\n");
    }
}

// run the at condition and return 1 if the condition is true, 0 if the condition is false
// this is same for all the conditions runners
int runAtCondition(struct conditionPackage* package, struct node* subjectsHead) {
    struct stringList* current = package->subjects;
    while (current != NULL) { // loop through the subjects to check if they are at the location
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        if (currentSubject == NULL) { // if the subject is not found return 0
            return 0;
        }
        if (strcmp(currentSubject->location, package->location) != 0) { // if the subject is not at the location return 0
            return 0;
        }
        current = current->next;
    }
    return 1; // if all subjects are at the location return 1
}

int runMultipleHasCondition(struct conditionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    struct stringList* current = package->subjects;
    while (current != NULL) { // loop through the subjects to check if they have the items with the counts
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        struct stringList* currentItem = package->items; // reset the current item to the first item
        struct intList* currentCount = package->counts; // reset the current count to the first count
        while (currentItem != NULL) { // loop through the items and the counts
            int itemIndex = getItemIndex(currentItem->string, itemList, itemListCount);
            // run the single has condition for each item and subject
            int singleHasCondition = runSingleHasCondition(*currentSubject, itemIndex, package->type, currentCount->number);
            if (singleHasCondition == 0) { // if any single has condition is false return 0
                return 0; 
            }
            currentItem = currentItem->next;
            currentCount = currentCount->next;
        }
        current = current->next; 
    }
    return 1; // if all subjects have the items with the counts return 1
}
        
int runSingleHasCondition(struct subject person, int itemIndex, char* type, int count) {
    if (strcmp(type, "has") == 0) { // check if the subject has the item with the count
        return person.inventory[itemIndex] == count;
    } else if (strcmp(type, "more") == 0) { // check if the subject has more than the count of the item
        return person.inventory[itemIndex] > count;
    } else if (strcmp(type, "less") == 0) { // check if the subject has less than the count of the item
        return person.inventory[itemIndex] < count;
    } else { // type is invalid
        printf("logic error at runSingleHasCondition\n");
    }
}
    
/* Condition list runner */

// run the condition list and return 1 if all the conditions are true, 0 if any of the conditions are false
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

/* Action runner */

// implicit declaration of functions
void runGoAction(struct actionPackage* package, struct node* subjectsHead);
void runBuyAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount);
void runBuyFromAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount);
void runSellAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount);
void runSellToAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount);

// run the action and change the subjects' locations and inventories accordingly
void runAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    if (strcmp(package->type, "go") == 0) { // if the type is "go" run the go action
        runGoAction(package, subjectsHead);
    } else if (strcmp(package->type, "buy") == 0) { // if the type is "buy",
        if (package->secondSubject == NULL) { // if the second subject is NULL run the buy action
            runBuyAction(package, subjectsHead, itemList, itemListCount);
        } else { // if the second subject is not NULL run the buy from action
            runBuyFromAction(package, subjectsHead, itemList, itemListCount);
        }
    } else if (strcmp(package->type, "sell") == 0) { // if the type is "sell",
        if (package->secondSubject == NULL) { // if the second subject is NULL run the sell action
            runSellAction(package, subjectsHead, itemList, itemListCount);
        } else { // if the second subject is not NULL run the sell to action
            runSellToAction(package, subjectsHead, itemList, itemListCount);
        }
    } else { // type is invalid
        printf("logic error at action runner\n");
    }
}

// run the go action and change the subjects' locations accordingly
void runGoAction(struct actionPackage* package, struct node* subjectsHead) {
    struct stringList* current = package->subjects;
    while (current != NULL) {
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        changeLocation(currentSubject, package->location);
        current = current->next;
    }
}

// run the buy action and change the subjects' inventories accordingly
// this is same for all the buy and sell runners
void runBuyAction(struct actionPackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    struct stringList* current = package->subjects;
    while (current != NULL) { // loop through the subjects to increase the count of the items
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        struct stringList* currentItem = package->items; // reset the current item to the first item
        struct intList* currentCount = package->counts; // reset the current count to the first count
        while (currentItem != NULL) { // loop through the items and the counts
            int itemIndex = getItemIndex(currentItem->string, itemList, itemListCount);
            changeItemCount(itemIndex, currentSubject, currentCount->number); // increase the count of the item
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
    // then buy
    struct stringList* current = package->subjects;
    while (current != NULL) { 
        currentItem = package->items; // reset the current item to the first item
        currentCount = package->counts; // reset the current count to the first count
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        while (currentItem != NULL) {
            int itemIndex = getItemIndex(currentItem->string, itemList, itemListCount);
            changeItemCount(itemIndex, currentSubject, currentCount->number); // increase the count of the item to the buyer
            changeItemCount(itemIndex, seller, (-1) * currentCount->number); // decrease the count of the item from the seller
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
    // reset current
    current = package->subjects;
    // then sell
    while (current != NULL) {
        currentItem = package->items; // reset the current item to the first item
        currentCount = package->counts; // reset the current count to the first count
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        while (currentItem != NULL) {
            int itemIndex = getItemIndex(currentItem->string, itemList, itemListCount); // get the index of the item to change the count
            changeItemCount(itemIndex, currentSubject, (-1) * currentCount->number); // decrease the count of the item from the seller
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
                return;
            }
            currentItem = currentItem->next;
            currentCount = currentCount->next;
        }
        current = current->next;
    }
    // reset current
    current = package->subjects;
    // then sell
    struct subject* buyer = findSubject(subjectsHead, package->secondSubject);
    while (current != NULL) {
        currentItem = package->items; // reset the current item to the first item
        currentCount = package->counts; // reset the current count to the first count
        struct subject* currentSubject = findSubject(subjectsHead, current->string);
        while (currentItem != NULL) {
            int itemIndex = getItemIndex(currentItem->string, itemList, itemListCount); // get the index of the item to change the count 
            changeItemCount(itemIndex, currentSubject, (-1) * currentCount->number); // decrease the count of the item from the seller
            changeItemCount(itemIndex, buyer, currentCount->number); // increase the count of the item to the buyer
            currentItem = currentItem->next;
            currentCount = currentCount->next;
        }
        current = current->next;
    }
}

/* Action list runner */

// run the action list and change the subjects' locations and inventories accordingly for each action
void runActionList(struct actionPackageList* head, struct node* subjectsHead, char** itemList, int* itemListCount) {
    struct actionPackageList* current = head;
    while (current != NULL) {
        runAction(current->package, subjectsHead, itemList, itemListCount);
        current = current->next;
    }
}

/* Sentence runner */

// run the sentence and change the subjects' locations and inventories accordingly for each action if the conditions are true
void runSentence(struct sentencePackage* package, struct node* subjectsHead, char** itemList, int* itemListCount) {
    if (package->conditions == NULL) {
        runActionList(package->actions, subjectsHead, itemList, itemListCount);
    } else {
        if (runConditionList(package->conditions, subjectsHead, itemList, itemListCount) == 1) {
            runActionList(package->actions, subjectsHead, itemList, itemListCount);
        }
    }
}

/* Sentence list runner */

// run the sentence list for each sentence
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

// parse buy action sentence
int checkBuy(char** inputWords,int* currentWordIndex,int inputWordsCount,struct actionPackage* action){
    int validCheck = 0;
    struct stringList* objectlist= newStringList("if");
    struct intList* numberlist= newIntList(0);
    do{//loop through the items
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if(!isStrNumber(inputWords[(*currentWordIndex)])){//check count of the item
            if(validCheck==0){
                break;
            }else{
                validCheck=1;
                (*currentWordIndex)--;
                action->items=objectlist->next;
                action->counts= numberlist->next;
                free(objectlist->string);
                objectlist->string=NULL;
                free(objectlist);
                free(numberlist);                
                return 1;//continue with action
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
    if(validCheck==0){//if the input is not in the correct format
        return 0;
    }
    //assign the items and counts to the action package
    action->items=objectlist->next;
    action->counts= numberlist->next;
    free(objectlist->string);
    objectlist->string=NULL;
    free(objectlist);
    free(numberlist);
    if(validCheck==3){//if the input is in the correct format and there is no more words
        return 3;
    }
    if(strcmp("from",inputWords[(*currentWordIndex)])==0){//if from exists
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
        while (current->next != NULL) {//check if the subject is already in the subjectlist
            if (strcmp(current->next->string, string) == 0) {
                return 0;
            }
            current = current->next;
        }
        action->secondSubject=(char*)malloc((strlen(inputWords[(*currentWordIndex)])+1)*sizeof(char));
        strcpy(action->secondSubject,inputWords[(*currentWordIndex)]);
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 3;//if the input is in the correct format and there is no more words
        }
    }
    if (strcmp("if",inputWords[(*currentWordIndex)])==0){
        return 2;//continue wıth condition
    }
    if (strcmp("and",inputWords[(*currentWordIndex)])==0){
        return 1;//continue with action
    }
    return 0;
}
//parse sell action sentence
int checkSell(char** inputWords,int* currentWordIndex,int inputWordsCount,struct actionPackage* action){
    int validCheck = 0;
    struct stringList* objectlist= newStringList("if");
    struct intList* numberlist= newIntList(0);
    do{//loop through the items
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if(!isStrNumber(inputWords[(*currentWordIndex)])){//check count of the item
            if(validCheck==0){
                break;
            }else{
                validCheck=1;
                (*currentWordIndex)--;
                action->items=objectlist->next;
                action->counts= numberlist->next;
                free(objectlist->string);
                objectlist->string=NULL;
                free(objectlist);
                free(numberlist);
                return 1;//continue with action
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
    if(validCheck==0){//if the input is not in the correct format
        return 0;
    }
    //assign the items and counts to the action package
    action->items=objectlist->next;
    action->counts= numberlist->next;
    free(objectlist->string);
    objectlist->string=NULL;
    free(objectlist);
    free(numberlist);
    if(validCheck==3){//if the input is in the correct format and there is no more words
        return 3;
    }
    if(strcmp("to",inputWords[(*currentWordIndex)])==0){//if from exists
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
        while (current->next != NULL) {//check if the subject is already in the subjectlist
            if (strcmp(current->next->string, string) == 0) {
                return 0;
            }
            current = current->next;
        }
        action->secondSubject=(char*)malloc((strlen(inputWords[(*currentWordIndex)])+1)*sizeof(char));
        strcpy(action->secondSubject,inputWords[(*currentWordIndex)]);
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 3;//if the input is in the correct format and there is no more words
        }
    }
    if (strcmp("if",inputWords[(*currentWordIndex)])==0){
        return 2;//continue wıth condition
    }
    if (strcmp("and",inputWords[(*currentWordIndex)])==0){
        return 1;//continue with action
    }
    return 0;
}
//parse go action sentence
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
    //assign the location to the action package
    action->location=(char*)malloc((strlen(inputWords[(*currentWordIndex)])+1)*sizeof(char));
    strcpy(action->location,inputWords[(*currentWordIndex)]);
    (*currentWordIndex)++;
    if ((*currentWordIndex)==inputWordsCount){
        return 3;//if the input is in the correct format and there is no more words
    }
    if (strcmp("if",inputWords[(*currentWordIndex)])==0){
        return 2;//continue wıth condition
    }
    if (strcmp("and",inputWords[(*currentWordIndex)])==0){
        return 1;//continue with action
    }
    return 0;
}
//parse action sentence
int checkAction(char** inputWords,int* currentWordIndex,int inputWordsCount,struct actionPackage* action){
    int validCheck = 1;
    struct stringList* subjectlist= newStringList("if");
    do{//loop through the subjects
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if(!strFormatTrue(inputWords[(*currentWordIndex)])){//check if the subject is in the correct format
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
    
    if(validCheck==0){//if the input is not in the correct format
        return 0;
    }
    //assign the subjects to the action package
    action->subjects=subjectlist->next;
    free(subjectlist->string);
    subjectlist->string=NULL;
    free(subjectlist);
    if(strcmp("buy",inputWords[(*currentWordIndex)])==0){
        action->type = (char*)malloc(4*sizeof(char));
        strcpy(action->type,"buy");
        return checkBuy(inputWords,currentWordIndex, inputWordsCount,action);//continue with buy
    }else if(strcmp("sell",inputWords[(*currentWordIndex)])==0){
        action->type = (char*)malloc(5*sizeof(char));
        strcpy(action->type,"sell");
        return checkSell(inputWords,currentWordIndex, inputWordsCount,action);//continue with sell
    }else if(strcmp("go",inputWords[(*currentWordIndex)])==0){
        action->type = (char*)malloc(3*sizeof(char));
        strcpy(action->type,"go");
        return checkGoTo(inputWords,currentWordIndex,inputWordsCount,action);//continue with go
    }else{
        return 0;//if the input is not in the correct format
    }

}
//parse has condition sentence
int checkHas(char** inputWords,int* currentWordIndex,int inputWordsCount,struct conditionPackage* condition){
    (*currentWordIndex)++;
    if ((*currentWordIndex)==inputWordsCount){
        return 0;
    }
    if (strcmp("more",inputWords[(*currentWordIndex)])==0){
        condition->type = (char*)malloc(5*sizeof(char));
        strcpy(condition->type,"more");//if the type is more
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if (!strcmp("than",inputWords[(*currentWordIndex)])==0){
            return 0;
        }
    }else if (strcmp("less",inputWords[(*currentWordIndex)])==0){
        condition->type = (char*)malloc(5*sizeof(char));
        strcpy(condition->type,"less");//if the type is less
        (*currentWordIndex)++;
        if ((*currentWordIndex)==inputWordsCount){
            return 0;
        }
        if (!strcmp("than",inputWords[(*currentWordIndex)])==0){
            return 0;
        }
    }else{
        condition->type = (char*)malloc(4*sizeof(char));
        strcpy(condition->type,"has");//if the type is has
        (*currentWordIndex)--;
    }
    int validCheck = 0;
    struct stringList* objectlist= newStringList("if");
    struct intList* numberlist= newIntList(0);
    do{//loop through the items
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
                condition->items=objectlist->next;
                condition->counts= numberlist->next;
                free(objectlist->string);
                objectlist->string=NULL;
                free(objectlist);
                free(numberlist);                
                return 1;//continue with condition
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
    //assign the items and counts to the condition package
    condition->items=objectlist->next;
    condition->counts= numberlist->next;
    free(objectlist->string);
    objectlist->string=NULL;
    free(objectlist);
    free(numberlist);
    return validCheck;
}
//parse at condition sentence
int checkAt(char** inputWords,int* currentWordIndex,int inputWordsCount,struct conditionPackage* condition){
    (*currentWordIndex)++;
    if ((*currentWordIndex)==inputWordsCount){
        return 0;
    } 
    if(!strFormatTrue(inputWords[(*currentWordIndex)])){
        return 0;
    }
    //assign the location to the condition package
    condition->location = (char*)malloc((strlen(inputWords[(*currentWordIndex)])+1)*sizeof(char));
    strcpy(condition->location,inputWords[(*currentWordIndex)]);
    (*currentWordIndex)++;
    if ((*currentWordIndex)==inputWordsCount){
        return 3;//if the input is in the correct format and there is no more words
    }
    if (strcmp("and",inputWords[(*currentWordIndex)])==0){
        return 1;//continue with condition
    }
    return 0;
}
//parse condition sentence
int checkCondition(char** inputWords,int* currentWordIndex,int inputWordsCount,struct conditionPackage* condition){
    int validCheck = 1;
    int sentenceControl= (*currentWordIndex);
    struct stringList* subjectlist= newStringList("if");
    do{//loop through the subjects
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
    
    if(validCheck==0){//if the input is not in the correct format
        return 0;
    }
    //assign the subjects to the condition package
    condition->subjects=subjectlist->next;
    free(subjectlist->string);
    subjectlist->string=NULL;
    free(subjectlist);
    if(strcmp("has",inputWords[(*currentWordIndex)])==0){
        return checkHas(inputWords,currentWordIndex, inputWordsCount,condition);//continue with has
    }else if(strcmp("at",inputWords[(*currentWordIndex)])==0){
        condition->type = (char*)malloc(3*sizeof(char));
        strcpy(condition->type,"at");        
        return checkAt(inputWords,currentWordIndex, inputWordsCount,condition);//continue with at
    }else if((strcmp("buy",inputWords[(*currentWordIndex)])==0)||(strcmp("sell",inputWords[(*currentWordIndex)])==0)||(strcmp("go",inputWords[(*currentWordIndex)])==0)){
        (*currentWordIndex)=sentenceControl;
        return 2;//continue with action
    }else{
        return 0;//if the input is not in the correct format
    }
}
//parse sentence
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
//parse the input
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

    char** itemList = (char**)calloc(MAX_INVERTORY_SIZE, sizeof(char*)); // item list for the items names
    int* itemListCount = (int*)malloc(sizeof(int)); // item list count for the number of items
    *itemListCount = 0; 
    struct node* subjects = (struct node*)malloc(sizeof(struct node)); // subjects list for the subjects
    subjects->person = createSubject("NOBODY"); // dummy head
    subjects->next = NULL; // dummy head
    // for syntax check iteration
    int currentWordIndexValue = -1;
    int* currentWordIndex = &currentWordIndexValue;

    
    while (1) { // shell loop
        char input[MAX_CHAR_COUNT + 2]; // +2 for newline and null terminator
        printf(">> "); // shell prompt
        fflush(stdout); // flush stdout to make sure prompt is printed
        fgets(input, MAX_CHAR_COUNT + 2, stdin); // +2 for newline and null terminator
        
        input[strlen(input) - 1] = '\0'; //remove newline

        char* inputWords[MAX_CHAR_COUNT]; // array to store the words in the input
        int inputWordsCount = 0; // number of words in the input

        char* token = strtok(input, " "); // split the input by spaces

        while (token != NULL) { // loop through the tokens
            inputWords[inputWordsCount] = token;
            inputWordsCount++;
            token = strtok(NULL, " ");
        }
        

        if (inputWordsCount == 1 && strcmp(inputWords[0], "exit") == 0) { // exit the shell
            break;
        }
            

        if (strcmp(inputWords[inputWordsCount-1], "?") == 0) { // if the last word is "?" parse the question
            struct questionPackage* package = parseQuestion(inputWords, inputWordsCount); // parse the question
            runQuestion(package, subjects, itemList, itemListCount); // run the question
            freeQuestionPackage(package); // free the question package
        } else { // if the last word is not "?" parse the phrase
            *currentWordIndex = -1;
            struct sentencePackageList* packageList = checkPhrase(inputWords, currentWordIndex, inputWordsCount); // parse the phrase
            if (packageList == NULL) { // if the phrase is invalid continue
                continue;
            }
            runSentenceList(packageList, subjects, itemList, itemListCount); // run the phrase
            freeSentencePackageList(packageList); // free the phrase package
        }
        fflush(stdout); // flush stdout to make sure all the outputs are printed
    }
}
