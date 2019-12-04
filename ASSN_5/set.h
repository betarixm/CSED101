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

int addElement(Set* set, int data);
int isElement(Set* set, int target);
int addElementIn(Set* baseSet, char* setName, int data);
int addSet(Set *baseSet, FILE *fp);
int showSet(Set *baseSet, char *setName);
int initSet(Set *set);
int showElements(Set *set);
int isElementIn(Set* baseSet, char* setName, int target);
int popElement(Set* set, int data);
int popElementIn(Set* baseSet, char* setName, int target);
int clearElements(Element* baseEle);
int clearSet(Set* baseSet, char* setName);
Set* findSetbyName(Set* baseSet, char* setName);
Set* appendSet(Set* baseSet, char*setName);