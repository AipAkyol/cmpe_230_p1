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