#pragma once

typedef struct ele {
    int data;
    struct ele *next;
} Element;

typedef struct set {
    char set_name[21];
    int set_size;
    Element *ele_head;
    struct set *next;
} Set;

void addSet(Set *baseSet, FILE *fp);

void showSet(Set *baseSet, char *setName);

int showElements(Set *set);

int isElement(Set *set, int target);

int addElement(Set *set, int data);

int popElement(Set *set, int data);

int clearSet(Set *baseSet, Set *set);

void clearElements(Element *baseEle);

int clearSetIn(Set *baseSet, char *setName);

void clearAllSet(Set *baseSet);

void initSet(Set *set);

int executer(int (*f)(Set *, int), Set *baseSet, char *setName, int target);

Set *getSetByName(Set *baseSet, char *setName);

Set *appendSet(Set *baseSet, char *setName);

Set *setUnion(Set *baseSet, char *targetName_1, char *targetName_2, char *resultName);

Set *setDiffInter(Set *baseSet, char *srcName, char *targetName, char *resultName, int isDiff);

int isSubset_Disjoint(Set *baseSet, char *setName_1, char *setName_2, int isSub);

void makeUniqName(Set *baseSet, char *setName);

Element ** minElement(Element **a, Element **b);