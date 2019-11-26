// macOS CLion에서 작성됨.

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

typedef struct {
    char name[21];
    int exam[2];

} STUDENT;

void add_student(STUDENT* s);
void print_student(STUDENT s[], int len);
void find(STUDENT s[], int len);
void print_one(int idx, STUDENT s);

int main(){
    STUDENT slist[10];
    char command[20];
    int len = 0;

    while(1){
        printf(">> ");
        scanf("%s", command);
        if(strcmp(command, "add") == 0){
            add_student(&slist[len++]);
        } else if (strcmp(command, "print") == 0){
            print_student(slist, len);
        } else if (strcmp(command, "find")==0){
            find(slist, len);
        } else if (strcmp(command, "quit") == 0){
            break;
        }
        printf("\n");
    }
}

void add_student(STUDENT* s){
    printf("이름: ");
    scanf("%s", s->name);

    printf("중간고사: ");
    scanf("%d", &s->exam[0]);

    printf("기말고사: ");
    scanf("%d", &s->exam[1]);
}

void print_student(STUDENT s[], int len){
    int i = 0;
    double avg = 0;
    char grade = '0';
    printf("번호  이름                    중간  기말  평균 (학점)\n");
    for (i = 0; i < len; i++){
        print_one(i, s[i]);
    }
}

void find(STUDENT s[], int len){
    int i = 0;
    char name[20] = "";

    printf("이름: ");
    scanf("%s", name);

    for(i = 0; i < len; i++){
        if(strcmp(name, s[i].name) == 0){
            print_one(i, s[i]);
        }
    }
}

void print_one(int idx, STUDENT s){
    double avg = (double)(s.exam[0] + s.exam[1])/2;
    char grade = 'A' + (avg  < 90) + (avg < 80) + (avg < 70) + 2 * (avg < 60);
    printf("%2d   %-20s %4d %4d  %4.1f (%c)\n", idx+1, s.name, s.exam[0], s.exam[1], avg, grade);
}