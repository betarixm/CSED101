// macOS CLion에서 작성됨.

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[21];
    int exam[2];
} STUDENT;

struct node {
    STUDENT data;
    struct node *next;
};

typedef struct node NODE;

struct list {
    int count;
    NODE *head;
};

typedef struct list LIST;

void insert_node(LIST *s);

void add_student(STUDENT *s);

void print_student(LIST *s);

void find(LIST *s);

void print_one(int idx, STUDENT s);

int main() {
    LIST slist;
    char command[20];

    slist.count = 0;
    slist.head = NULL;

    while (1) {
        printf(">> ");
        scanf("%s", command);
        if (strcmp(command, "add") == 0) {
            insert_node(&slist);
        } else if (strcmp(command, "print") == 0) {
            print_student(&slist);
        } else if (strcmp(command, "find") == 0) {
            find(&slist);
        } else if (strcmp(command, "quit") == 0) {
            break;
        }
        printf("\n");
    }
}

void insert_node(LIST *s) {
    NODE *cur = s->head;
    NODE *tmp = (NODE*)malloc(sizeof(NODE));
    int count = (s->count);
    if(s->count == 0){
        tmp->next = s->head;
        s->head = tmp;
    } else {

        while(--count){

            cur = cur->next;
        }
        tmp->next = cur->next;
        cur->next = tmp;
    }

    add_student(&(tmp->data));
    (s->count)++;
}

void add_student(STUDENT *s) {
    printf("이름: ");
    scanf("%s", s->name);

    printf("중간고사: ");
    scanf("%d", &(s->exam[0]));

    printf("기말고사: ");
    scanf("%d", &(s->exam[1]));
}

void print_student(LIST *s) {
    int i = 0;
    NODE *cur;
    printf("번호  이름                    중간  기말  평균 (학점)\n");
    for (cur = s->head; cur != NULL; (cur = cur->next ) && (i++)) {
        print_one(i, cur->data);
    }
}

void find(LIST *s) {
    int i = 0;
    char name[20] = "";
    NODE *cur;

    printf("이름: ");
    scanf("%s", name);

    printf("번호  이름                    중간  기말  평균 (학점)\n");
    for (cur = s->head; cur != NULL; cur = cur->next) {
        if (strcmp(name, cur->data.name) == 0) {
            print_one(i, cur->data);
        }
        i++;
    }
}

void print_one(int idx, STUDENT s) {
    double avg = (double) (s.exam[0] + s.exam[1]) / 2;
    char grade = 'A' + (avg < 90) + (avg < 80) + (avg < 70) + 2 * (avg < 60);
    printf("%2d   %-20s %4d %4d  %4.1f (%c)\n", idx + 1, s.name, s.exam[0], s.exam[1], avg, grade);
}