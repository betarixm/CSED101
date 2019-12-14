#pragma once // 한번만 컴파일 되고 포함되도록 pragma once 자것ㅇ

typedef struct ele { // element 구조체
    int data; // 데이터
    struct ele *next; // 다음 element를 가리킴
} Element;

typedef struct set { // set 구조체
    char set_name[21]; // set 이름
    int set_size; // set의 element 개수
    Element *ele_head; // set의 첫 element 주소
    struct set *next; // 다음 set 주소
} Set;

// 함수 프로토타입 선언

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

void printResult(int result);