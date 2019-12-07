#pragma once

typedef struct ele {
    int data;
    struct ele* next;
} Element;

typedef struct set {
    char set_name[21];
    int set_size;
    Element* ele_head;
    struct set* next;
} Set;

void clearAllSet(Set* baseSet);
int exeuter(int (*f)(Set*, int), Set* baseSet, char* setName, int target);
int addElement(Set* set, int data);
int isElement(Set* set, int target);
int addSet(Set *baseSet, FILE *fp);
void showSet(Set *baseSet, char *setName);
void initSet(Set *set);
int showElements(Set *set);
int popElement(Set* set, int data);
void clearElements(Element* baseEle);
int clearSetIn(Set* baseSet, char* setName);
int clearSet(Set* baseSet, Set* set);
int isDisjoint(Set* baseSet, char* setName_1, char* setName_2);
int isSubset(Set* baseSet, char* setName_1, char* setName_2);
Set* getSetByName(Set* baseSet, char* setName);
Set* appendSet(Set* baseSet, char*setName);
Element* maxElement(Element* a, Element*b);
Set * setUnion(Set* baseSet, char* targetName_1, char* targetName_2, char* resultName);
Set * setInter(Set* baseSet, char* srcName, char* targetName, char* resultName);
Set* setDiff(Set* baseSet, char* srcName, char* targetName, char*resultName);
int isValidName(Set* baseSet, char* setName);