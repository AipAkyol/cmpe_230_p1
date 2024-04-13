#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR_COUNT 1024
char* keywords[] =  {"sell", "buy", "go", "to", "from",
 "and", "at", "has", "if", "less", "more", "than", "exit", "where",
  "total", "who","NOBODY", "NOTHING", "NOWHERE"};


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
        if (!((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z') || word[i] == '_' || word[i]=='\n')) {
            return 0;
        }
    }
    return 1;
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

int strFormatTrue(char* word) {
    return !strContainsKeyword(word) && strOnlyLettersandUnderscores(word);
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
                printf("there is no object");
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
        printf("error in object");
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
                printf("there is no object");
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
        printf("error in object");
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
        printf("error in subject");
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
        printf("error in verb");
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
                printf("there is no object");
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
    condition->location = (char*)malloc(strlen(inputWords[(*currentWordIndex)]+1)*sizeof(char));
    condition->location = inputWords[(*currentWordIndex)];
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
        printf("error in subject");
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
        printf("error in verb");
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
void printSentenceList(struct sentencePackageList* head);
void checkPhrase(char** inputWords, int* currentWordIndex,int inputWordsCount){

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
        printf("INVALID");
    }else{
        printf("OK");
        printSentenceList(root);
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
        printf("Conditions: ");
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
        printf("Actions: ");
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
void main() {  
    char** item_list = (char**)calloc(17000,sizeof(char*));
    int currentWordIndexValue = -1;
    int* currentWordIndex = &currentWordIndexValue;
    while (1) { // shell loop
        char input[MAX_CHAR_COUNT + 2]; // +2 for newline and null terminator
        printf(">>"); // shell prompt
        fgets(input, MAX_CHAR_COUNT + 2, stdin); // +2 for newline and null terminator
        input[strlen(input) - 1] = '\0';
        
        if (strcmp(input, "exit") == 0) {
            break;
        }
        int input_length = strlen(input);

        char* inputWords[MAX_CHAR_COUNT];
        int inputWordsCount = 0;

        char* token = strtok(input, " ");

        while (token != NULL) {
            inputWords[inputWordsCount] = token;
            inputWordsCount++;
            token = strtok(NULL, " ");
        }
        
        *currentWordIndex = -1;
        checkPhrase(inputWords,currentWordIndex,inputWordsCount);
    }
    printf("finished");
}
