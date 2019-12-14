#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


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
void is_element(FILE* Input, S_Info* Info);
void add(FILE* Input, S_Info* Info);
void pop(FILE* Input, S_Info* Info);
void clear(FILE* Input, S_Info* Info);
void quit(S_Info* Info);
void mk_union(FILE* Input, S_Info* Info);
void mk_intsct(FILE* Input, S_Info* Info);
void mk_diff(FILE* Input, S_Info* Info);
void mk_add(Set* tempSet, int add_data);
void is_disjoint(FILE* Input, S_Info* Info);
void init_set(Set* p_set) {
    p_set->ele_head = NULL;
    p_set->ele_tail = NULL;
    p_set->next = NULL;
}
void set(FILE* Input, S_Info* Info) {

    Set* p_set = (Set*)malloc(sizeof(Set));
    Element* temp, *next_temp;
    Set* tempSet;

    init_set(p_set);
    fscanf(Input, "%s%d", p_set->set_name, &p_set->set_size);

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

    if (Info->cnt == 0) {
        Info->head = p_set;
        Info->tail = p_set;
    }
    else {
        for (tempSet = Info->head; tempSet != NULL; tempSet = tempSet->next) {
            if (!strcmp(tempSet->set_name, p_set->set_name)) {
                for (temp = p_set->ele_head; temp; temp = next_temp) {
                    next_temp = temp->next;
                    free(temp);
                }
                free(p_set);
                return;
            }
            if (tempSet == Info->tail) {
                Info->tail->next = p_set;
                Info->tail = p_set;
                break;
            }
        }
    }

    Info->cnt++;

    Sort(p_set);
    check_Dupele(p_set);
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
                else if (i == 0 && j == p_set->set_size - 2) {
                    prev->next = head;
                    p_set->ele_tail = head->next;
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

    if (set_name[0] == '\n' || set_name[0] == '\0') {
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
                printf("\n");
            }
        }
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
void is_element(FILE* Input, S_Info* Info) {
    Set* tempSet;
    char set_name[21];
    int find_num, result = 0;
    fscanf(Input, "%s%d", set_name, &find_num);

    for (tempSet = Info->head; tempSet != NULL; tempSet = tempSet->next) {
        if (!strcmp(tempSet->set_name, set_name)) {
            for (Element* temp = tempSet->ele_head; temp; temp = temp->next) {
                if (find_num == temp->data)
                    result = 1;
            }
            break;
        }
    }
    if (tempSet == NULL)
        return;

    fprintf(stdout, "%d\n", result);
}
void add(FILE* Input, S_Info* Info) {
    char set_name[21];
    Element* ele_add = (Element*)malloc(sizeof(Element));

    fscanf(Input, "%s", set_name);
    fscanf(Input, "%d", &ele_add->data);

    for (Set* tempSet = Info->head; tempSet != NULL; tempSet = tempSet->next) {
        if (!strcmp(tempSet->set_name, set_name)) {
            if (tempSet->ele_head == NULL || ele_add->data < tempSet->ele_head->data) {
                ele_add->next = tempSet->ele_head;
                if(tempSet->ele_head == NULL)
                    tempSet->ele_tail = ele_add;
                tempSet->ele_head = ele_add;
            }

            else if (ele_add->data > tempSet->ele_tail->data) {
                ele_add->next = tempSet->ele_tail->next;
                tempSet->ele_tail->next = ele_add;
                tempSet->ele_tail = ele_add;
            }

            else {
                for (Element* bf_temp = tempSet->ele_head; bf_temp->next; bf_temp = bf_temp->next) {
                    if (ele_add->data == bf_temp->data || ele_add->data == bf_temp->next->data) {
                        free(ele_add);
                        return;
                    }
                    else if (bf_temp->data < ele_add->data && bf_temp->next->data > ele_add->data) {
                        ele_add->next = bf_temp->next;
                        bf_temp->next = ele_add;
                        break;
                    }
                }
            }
            tempSet->set_size++;
        }
    }
}
void pop(FILE* Input, S_Info* Info) {
    char set_name[21];
    int dele_num;
    Element* dele_temp;

    fscanf(Input, "%s", set_name);
    fscanf(Input, "%d", &dele_num);

    for (Set* tempSet = Info->head; tempSet != NULL; tempSet = tempSet->next) {
        if (!strcmp(tempSet->set_name, set_name)) {

            if (tempSet->ele_head == NULL)
                return;

            else if (tempSet->ele_head->data == dele_num) {
                dele_temp = tempSet->ele_head;
                tempSet->ele_head = tempSet->ele_head->next;
                free(dele_temp);
                tempSet->set_size--;
            }
            else {
                for (Element* temp = tempSet->ele_head; temp->next; temp = temp->next) {
                    if (dele_num == temp->next->data) {
                        if (temp->next == tempSet->ele_tail)
                            tempSet->ele_tail = temp;
                        dele_temp = temp->next;
                        temp->next = temp->next->next;
                        free(dele_temp);
                        tempSet->set_size--;
                        break;
                    }
                }
            }
            break;
        }
    }
}
void clear(FILE* Input, S_Info* Info) {
    char set_name[21];
    Element* dele_temp;
    Set* dele_tempSet;

    fscanf(Input, "%s", set_name);

    if (!strcmp(Info->head->set_name, set_name)) {
        for (Element* temp = Info->head->ele_head; temp; ) {
            dele_temp = temp;
            temp = temp->next;
            free(dele_temp);
        }
        dele_tempSet = Info->head;
        Info->head = Info->head->next;
        free(dele_tempSet);
        Info->cnt--;
    }
    else {
        for (Set* tempSet = Info->head; tempSet->next; tempSet = tempSet->next) {
            if (!strcmp(tempSet->next->set_name, set_name)) {
                for (Element* temp = tempSet->next->ele_head; temp; ) {
                    dele_temp = temp;
                    temp = temp->next;
                    free(dele_temp);
                }
                if (tempSet->next == Info->tail)
                    Info->tail = tempSet;
                dele_tempSet = tempSet->next;
                tempSet->next = tempSet->next->next;
                free(dele_tempSet);
                Info->cnt--;
                break;
            }
        }
    }
}
void quit(S_Info* Info) {
    Element* dele_temp;
    Set* dele_tempSet;
    for (Set* tempSet = Info->head; tempSet; ) {
        for (Element* temp = tempSet->ele_head; temp; ) {
            dele_temp = temp;
            temp = temp->next;
            free(dele_temp);
        }
        dele_tempSet = tempSet;
        tempSet= tempSet->next;
        free(dele_tempSet);
    }
}

void mk_union(FILE* Input, S_Info* Info) {
    char set1_name[21], set2_name[21];
    Set* N_set = (Set*)malloc(sizeof(Set));
    Set* tempSet, * set1 = NULL, * set2 = NULL;
    Element* ele_temp1, * ele_temp2;


    init_set(N_set);
    fscanf(Input, "%s%s%s", set1_name, set2_name, N_set->set_name);

    for (tempSet = Info->head; tempSet != NULL; tempSet = tempSet->next) {
        if (!strcmp(tempSet->set_name, N_set->set_name)) {
            free(N_set);
            return;
        }
        if (!strcmp(tempSet->set_name, set1_name))
            set1 = tempSet;
        if (!strcmp(tempSet->set_name, set2_name))
            set2 = tempSet;
    }
    if (set1 == NULL || set2 == NULL) {
        free(N_set);
        return;
    }

    ele_temp1 = set1->ele_head;
    ele_temp2 = set2->ele_head;

    while (ele_temp1 != NULL || ele_temp2 != NULL) {
        if (ele_temp2 == NULL || (ele_temp1 != NULL && ele_temp1->data < ele_temp2->data)) {
            mk_add(N_set, ele_temp1->data);
            ele_temp1 = ele_temp1->next;
        }
        else if (ele_temp1 == NULL || (ele_temp2 != NULL && ele_temp1->data > ele_temp2->data)) {
            mk_add(N_set, ele_temp2->data);
            ele_temp2 = ele_temp2->next;
        }
        else {
            mk_add(N_set, ele_temp1->data);
            ele_temp1 = ele_temp1->next;
            ele_temp2 = ele_temp2->next;
        }
    }

    Info->tail->next = N_set;
    Info->tail = N_set;

    Info->cnt++;
}
void mk_intsct(FILE* Input, S_Info* Info) {
    char set1_name[21], set2_name[21];
    Set* N_set = (Set*)malloc(sizeof(Set));
    Set* tempSet, * set1 = NULL, * set2 = NULL;
    Element* ele_temp1, *ele_temp2;

    init_set(N_set);
    fscanf(Input, "%s%s%s", set1_name, set2_name, N_set->set_name);

    for (tempSet = Info->head; tempSet != NULL; tempSet = tempSet->next) {
        if (!strcmp(tempSet->set_name, N_set->set_name)) {
            free(N_set);
            return;
        }
        if (!strcmp(tempSet->set_name, set1_name))
            set1 = tempSet;
        if (!strcmp(tempSet->set_name, set2_name))
            set2 = tempSet;
    }
    if (set1 == NULL || set2 == NULL) {
        free(N_set);
        return;
    }

    ele_temp1 = set1->ele_head;
    ele_temp2 = set2->ele_head;

    while (ele_temp1 != NULL && ele_temp2 != NULL) {
        if (ele_temp1->data < ele_temp2->data)
            ele_temp1 = ele_temp1->next;
        else if (ele_temp1->data > ele_temp2->data)
            ele_temp2 = ele_temp2->next;
        else {
            mk_add(N_set, ele_temp1->data);
            ele_temp1 = ele_temp1->next;
            ele_temp2 = ele_temp2->next;
        }
    }

    Info->tail->next = N_set;
    Info->tail = N_set;

    Info->cnt++;
}
void mk_diff(FILE* Input, S_Info* Info) {
    char set1_name[21], set2_name[21];
    Set* N_set = (Set*)malloc(sizeof(Set));
    Set* tempSet, * set1 = NULL, * set2 = NULL;
    Element* ele_temp1, * ele_temp2;


    init_set(N_set);
    fscanf(Input, "%s%s%s", set1_name, set2_name, N_set->set_name);

    for (tempSet = Info->head; tempSet != NULL; tempSet = tempSet->next) {
        if (!strcmp(tempSet->set_name, N_set->set_name)) {
            free(N_set);
            return;
        }
        if (!strcmp(tempSet->set_name, set1_name))
            set1 = tempSet;
        if (!strcmp(tempSet->set_name, set2_name))
            set2 = tempSet;
    }
    if (set1 == NULL || set2 == NULL) {
        free(N_set);
        return;
    }

    ele_temp1 = set1->ele_head;
    ele_temp2 = set2->ele_head;

    while (ele_temp1 != NULL) {
        if (ele_temp2 == NULL || ele_temp1->data < ele_temp2->data) {
            mk_add(N_set, ele_temp1->data);
            ele_temp1 = ele_temp1->next;
        }
        else if (ele_temp1->data > ele_temp2->data)
            ele_temp2 = ele_temp2->next;
        else {
            ele_temp1 = ele_temp1->next;
            ele_temp2 = ele_temp2->next;
        }
    }

    Info->tail->next = N_set;
    Info->tail = N_set;

    Info->cnt++;
}
void mk_add(Set* tempSet, int add_data) {
    Element* ele_add = (Element*)malloc(sizeof(Element));
    ele_add->data = add_data;
    ele_add->next = NULL;

    if (tempSet->ele_head == NULL)
        tempSet->ele_head = ele_add;
    else{
        ele_add->next = tempSet->ele_tail->next;
        tempSet->ele_tail->next = ele_add;
    }
    tempSet->ele_tail = ele_add;

    tempSet->set_size++;
}
void is_disjoint(FILE* Input, S_Info* Info) {
    char set1_name[21], set2_name[21];
    Set* tempSet, * set1 = NULL, * set2 = NULL;
    Element* ele_temp1, * ele_temp2;
    int result = 1;

    fscanf(Input, "%s%s", set1_name, set2_name);

    for (tempSet = Info->head; tempSet != NULL; tempSet = tempSet->next) {
        if (!strcmp(tempSet->set_name, set1_name))
            set1 = tempSet;
        if (!strcmp(tempSet->set_name, set2_name))
            set2 = tempSet;
    }
    if (set1 == NULL || set2 == NULL)
        return;

    ele_temp1 = set1->ele_head;
    ele_temp2 = set2->ele_head;

    while (ele_temp1 != NULL && ele_temp2 != NULL) {
        if (ele_temp1->data < ele_temp2->data)
            ele_temp1 = ele_temp1->next;
        else if (ele_temp1->data > ele_temp2->data)
            ele_temp2 = ele_temp2->next;
        else {
            result = 0;
            break;
        }
    }
    fprintf(stdout, "%d\n", result);
}
void is_subset(FILE* Input, S_Info* Info) {
    char set1_name[21], set2_name[21];
    Set* tempSet, * set1 = NULL, * set2 = NULL;
    Element* ele_temp1, * ele_temp2;
    int result = 0, cnt = 0;

    fscanf(Input, "%s%s", set1_name, set2_name);

    for (tempSet = Info->head; tempSet != NULL; tempSet = tempSet->next) {
        if (!strcmp(tempSet->set_name, set1_name))
            set1 = tempSet;
        if (!strcmp(tempSet->set_name, set2_name))
            set2 = tempSet;
    }
    if (set1 == NULL || set2 == NULL)
        return;

    ele_temp1 = set1->ele_head;
    ele_temp2 = set2->ele_head;

    while (ele_temp1 != NULL && ele_temp2 != NULL) {
        if (ele_temp1->data < ele_temp2->data)
            ele_temp1 = ele_temp1->next;
        else if (ele_temp1->data > ele_temp2->data)
            ele_temp2 = ele_temp2->next;
        else {
            cnt++;
            ele_temp1 = ele_temp1->next;
            ele_temp2 = ele_temp2->next;
        }
    }
    if (cnt == set1->set_size)
        result = 1;

    fprintf(stdout, "%d\n", result);
}

int main(int argc, char* argv[]) {
    char command[20], file_Name[30];
    S_Info Info = {NULL, NULL, 0};
    FILE* Input = (argc > 1) ? fopen(argv[1], "r") : stdin;

    while (1) {

        if (Input == stdin)
            printf(">> ");

        if (fscanf(Input, "%s", command) == EOF){
            Input = stdin;
        };

        if (strcmp(command, "set") == 0)
            set(Input, &Info);

        else if (strcmp(command, "show") == 0)
            show(Input, &Info);

        else if (strcmp(command, "is_element") == 0)
            is_element(Input, &Info);

        else if (strcmp(command, "add") == 0)
            add(Input, &Info);

        else if (strcmp(command, "pop") == 0)
            pop(Input, &Info);

        else if (strcmp(command, "clear") == 0)
            clear(Input, &Info);

        else if (strcmp(command, "union") == 0)
            mk_union(Input, &Info);

        else if (strcmp(command, "intersection") == 0)
            mk_intsct(Input, &Info);

        else if (strcmp(command, "difference") == 0)
            mk_diff(Input, &Info);

        else if (strcmp(command, "is_disjoint") == 0)
            is_disjoint(Input, &Info);

        else if (strcmp(command, "is_subset") == 0)
            is_subset(Input, &Info);

        else if (strcmp(command, "quit") == 0) {
            quit(&Info);
            break;
        }
        strcpy(command, "");

    }

    return 0;
}