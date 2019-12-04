#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

int setUnion(Set* baseSet, char* targetName_1, char* targetName_2, char* resultSetName){
    Set* targetSet_1 = findSetbyName(baseSet, targetName_1);
    Set* targetSet_2 = findSetbyName(baseSet, targetName_2);
    Set* resultSet = (Set*)malloc(sizeof(Set));


}
int isElementIn(Set* baseSet, char* setName, int target){
    Set* set = findSetbyName(baseSet, setName);
    if(set != 0){
        return isElement(set, target);
    }
    return 0;
}

int popElementIn(Set* baseSet, char* setName, int target){
    Set* set = findSetbyName(baseSet, setName);
    if(set != 0){
        popElement(set, target);
    }
}

int isElement(Set* set, int target){
    Element* e = set->ele_head;
    while(e != NULL){
        if(e->data == target){
            return 1;
        }
        e = e->next;
    }
    return 0;
}

int addElementIn(Set* baseSet, char* setName, int data){
    Set* set = findSetbyName(baseSet, setName);
    if(set != 0){
        addElement(set, data);
    }
}

int addElement(Set* set, int data){
    int i = 0;
    Element* prevEle = set->ele_head;
    Element* tmp = (Element*)malloc(sizeof(Element));
    tmp->next = set->ele_head;
    tmp->data = data;
    if(!isElement(set, data)){
        if(set->set_size == 0){
            set->ele_head = tmp;
        } else {
            while(prevEle->next != NULL && (prevEle->next->data < data)){
                prevEle = prevEle->next;
                i++;
            }
            if(i == 0 && data < prevEle->data){
                set->ele_head = tmp;
                tmp->next = prevEle;
            } else {
                tmp->next = prevEle->next;
                prevEle->next = tmp;
            }
        }
        (set->set_size)++;
    }
}

int popElement(Set* set, int data){
    Element* cur = set->ele_head;
    Element* tmp;

    if(cur->data == data){
        tmp = set->ele_head;
        set->ele_head = tmp->next;
        free(tmp);
    } else {
        while(cur != NULL && cur->next != NULL){
            if(cur->next->data == data){
                tmp = cur->next;
                cur->next = tmp->next;
                free(tmp);
            }
            cur = cur->next;
        }
    }
    (set->set_size)--;

}

int addSet(Set *baseSet, FILE *fp) {
    int i = 0, set_size = 0, tmp = 0;
    char set_name[21] = "";
    Set* newSet, *prevSet = baseSet;

    fscanf(fp, "%s", set_name);
    fscanf(fp, "%d", &(set_size));

    if((!findSetbyName(baseSet, set_name)) && set_size >= 0){
        newSet = (Set*)malloc(sizeof(Set));
        while(prevSet->next != NULL){
            prevSet = prevSet->next;
        }

        prevSet->next = newSet;

        newSet->set_size = 0;
        newSet->next = NULL;
        newSet->ele_head = NULL;
        strcpy(newSet->set_name, set_name);

        for(i = 0; i < set_size; i++){
            fscanf(fp, "%d", &tmp);
            addElement(newSet, tmp);
        }
    }

}

int showSet(Set *baseSet, char *setName) {
    Set* setTarget = baseSet->next;
    int isPrint;
    while(setTarget != NULL){
        isPrint = (strcmp(setName, "") == 0)?(1):(strcmp(setName, setTarget->set_name) == 0);
        (isPrint) && (showElements(setTarget));
        setTarget = setTarget -> next;
    }
}

int showElements(Set *set){
    Element* eleTarget = set->ele_head;
    printf("%s : ", set->set_name);
    while(eleTarget != NULL){
        printf("%d ", eleTarget->data);
        eleTarget = eleTarget->next;
    }
    printf("\n");
}

int initSet(Set *set){
    set->set_size = 0;
    set->next = NULL;
    set->ele_head = NULL;
}

int clearElements(Element* baseEle){
    Element* cur = baseEle;
    Element* tmp;
    while(cur != NULL){
        tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

int clearSet(Set* baseSet, char* setName){
    Set* cur = baseSet->next;
    Set* tmp;

    if(strcmp(cur->set_name, setName) == 0){
        tmp = baseSet->next;
        baseSet->next = tmp->next;
        clearElements(cur->ele_head);
        free(cur);
    } else {
        while(cur != NULL && cur->next != NULL){
            if(strcmp(cur->next->set_name, setName) == 0){
                tmp = cur->next;
                cur->next = tmp->next;
                clearElements(tmp->ele_head);
                free(tmp);
            }
            cur = cur->next;
        }
    }
}

Set* findSetbyName(Set* baseSet, char* setName){
    Set* cur = baseSet->next;
    while(cur != NULL){
        if(strcmp(cur->set_name, setName) == 0){
            return cur;
        }
        cur = cur->next;
    }
    return 0;
}

Set* appendSet(Set* baseSet, char*setName){
    Set* newSet, *prevSet = baseSet;
    if(findSetbyName(baseSet, setName) != 0){
        return 0;
    } else {
        newSet = (Set*)malloc(sizeof(Set));
        while(prevSet->next != NULL){
            prevSet = prevSet->next;
        }

        prevSet->next = newSet;
        newSet->set_size = 0;
        newSet->next = NULL;
        newSet->ele_head = NULL;
        strcpy(newSet->set_name, setName);
        return newSet;
    }
}