#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct ele
{
    int data;
    struct ele* next;
} Element;

typedef struct set
{
    char set_name[21];
    int set_size;
    Element* ele_head;
    Element* ele_tail;
    struct set* next;
} Set;

typedef struct set_Info {
    Set* head;
    Set* tail;
    int cnt;
}S_Info;

void init_set(Set* p_set);
void set(FILE* Input, S_Info* Info);
Element* swap_element(Element* p_cur, Element* p_next);
void Sort(Set* p_set);
void show(FILE* Input, S_Info* Info);
void check_Dupele(Set* p_set);


void init_set(Set* p_set) {
    p_set->ele_head = NULL;
    p_set->ele_tail = NULL;
    p_set->next = NULL;
}

void set(FILE* Input, S_Info* Info) {

    Set* p_set = (Set*)malloc(sizeof(Set));
    Element* temp;
    Set* tempSet;

    init_set(p_set);
    fscanf(Input, "%s%d", p_set->set_name, &p_set->set_size);

    if(Info->cnt == 0) {
        Info->head = p_set;
        Info->tail = p_set;
    } else {
        for (tempSet = Info->head; tempSet != NULL; tempSet = tempSet->next) {
            if (!strcmp(tempSet->set_name, p_set->set_name)) {
                free(p_set);
                return;
            } 
            if(tempSet == Info->tail){
                Info->tail->next = p_set;
                Info->tail = p_set;
                break;
            }
        }
    }

    Info->cnt++;

    for (int i = 0; i < p_set->set_size; i++) {

        temp = (Element*)malloc(sizeof(Element));
        fscanf(Input, "%d", &temp->data);
        temp->next = NULL;

        if (i == 0)
            p_set->ele_head = temp;
        else
            p_set->ele_tail->next = temp;

        p_set->ele_tail = temp;
    }

    Sort(p_set);
    check_Dupele(p_set);

    return;
}

Element* swap_element(Element* p_cur, Element* p_next) {
    Element* temp = p_next->next;
    p_next->next = p_cur;
    p_cur->next = temp;
    return p_next;
}

void Sort(Set* p_set) {
    int cnt;
    Element *prev, *cur, *next, *head;

    for (int i = 0; i < p_set->set_size; i++) {
        prev = p_set->ele_head;
        head = p_set->ele_head;
        cnt = 0;
        for (int j = i; j < p_set->set_size - 1; j++) {
            cur = head;
            next = cur->next;
            if (cur->data > next->data) {
                head = swap_element(cur, next);
                if (j == i) {
                    p_set->ele_head = head;
                    prev = p_set->ele_head;
                }
                else
                    prev->next = head;
                cnt++;
            }
            head = head->next;
            if (j != i)
                prev = prev->next;
        }
        if (cnt == 0)
            break;
    }
}

void show(FILE* Input, S_Info* Info) {
    char set_name[21] = "";
    char* name;
    fgets(set_name, 21, Input);

    if (set_name[0] == '\n') {
        for (Set* temp = Info->head; temp; temp = temp->next) {
            fprintf(stdout, "%s :", temp->set_name);
            for (Element* ele_temp = temp->ele_head; ele_temp; ele_temp = ele_temp->next) {
                fprintf(stdout, " %d", ele_temp->data);
            }
            printf("\n");
        }
    }
    else {
        name = strtok(set_name, " ");
        strtok(name, "\n");
        for (Set* temp = Info->head; temp; temp = temp->next) {
            if (strcmp(name, temp->set_name) == 0) {
                fprintf(stdout, "%s :", temp->set_name);
                for (Element* ele_temp = temp->ele_head; ele_temp; ele_temp = ele_temp->next)
                    fprintf(stdout, " %d", ele_temp->data);
            }
        }
        printf("\n");
    }
}

void check_Dupele(Set* p_set) {
    Element* temp, * bf_temp = p_set->ele_head;

    if (p_set->ele_head == NULL)
        return;

    for (temp = p_set->ele_head->next; temp; ) {
        if (temp->data == bf_temp->data) {
            bf_temp->next = temp->next;
            free(temp);
            temp = bf_temp->next;
            p_set->set_size--;
        }
        else {
            bf_temp = bf_temp->next;
            temp = temp->next;
        }
    }
}

int main(int argc, char** Argv) {
    char command[20], file_Name[30];
    S_Info Info = {NULL, NULL, 0};
    FILE* Input = stdin;

    while (1) {
        printf(">> ");
        fscanf(Input, "%s", command);

        if (strcmp(command, "set") == 0) {
            set(Input, &Info);
        }

        else if (strcmp(command, "show") == 0)
            show(Input, &Info);

        else if (strcmp(command, "is_element") == 0)
            break;

        else if (strcmp(command, "add") == 0)
            break;

        else if (strcmp(command, "pop") == 0)
            break;

        else if (strcmp(command, "clear") == 0)
            break;

        else if (strcmp(command, "quit") == 0)
            break;

        printf("\n");
    }

    return 0;
}