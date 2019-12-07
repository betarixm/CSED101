/*
 * 작성일: 20191206
 * 학  번: 20190084
 */
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_STRING 20

// 큐 노드 구조체 NODE 선언
typedef struct node
{
    char data;
    struct node *next;
} NODE;

// 큐 헤드 구조체 QUEUE 선언
typedef struct
{
    int count;
    NODE *front;
    NODE *rear;
} QUEUE;

void printQueue(QUEUE *q);
int enqueue(QUEUE *q, char data);
int dequeue(QUEUE *q, char *data);
int isEmpty(QUEUE *q);

int main()
{
    int len, i, ioRes;
    char str[MAX_STRING];
    QUEUE queue; // 큐

    // 큐 초기화
    queue.count = 0;
    queue.front = NULL;
    queue.rear = NULL;


    while (1)
    {
        printf(">> ");
        ioRes = scanf("%s", str);

        if (ioRes == EOF)
            break;

        len = strlen(str);
        for (i = 0; i < len; i++)
        {
            enqueue(&queue, str[i]); // enqueue 함수 호출
        }

        printQueue(&queue);

    } // while

    printf("bye!\n");

    return 0;
} // main

void printQueue(QUEUE *q)
{
    char data;

    printf("   ");
    while (!isEmpty(q)){ // 큐가 빌 때까지 dequeue 함수 호출
        dequeue(q, &data);
        printf("%c", data);
    }
    printf("\n\n");

} // printQueue

// enqueue 성공시 1, 실패시 0
int enqueue(QUEUE *q, char data)
{
    NODE *temp;

    // 큐 노드 생성
    temp = (NODE*)malloc(sizeof(NODE));

    if (!temp) // 동적 할당 실패시
        return 0;

    // 생성한 노드에 값 설정
    temp->data = data;
    temp->next = NULL;

    if (isEmpty(q)){ // 큐가 비어 있는 경우(isEmpty 함수 호출)
        q->front = temp;
    } else {
        q->rear->next = temp;
    }
    q->rear = temp;
    q->count++;

    return 1;
} // enqueue

// dequeue 성공시 1, 실패시 0
int dequeue(QUEUE *q, char *data)
{
    NODE *temp;

    if (isEmpty(q)) // 큐가 비어 있는 경우
        return 0;
    temp = q->front;

    if (q->count == 1) {
        q->front = NULL;
        q->rear = NULL;
    } else {
        q->front = temp->next;
    }
    *data = temp->data;
    free(temp);
    q->count--;
    return 1;
} // dequeue

// 큐가 비어 있으면 1, 그렇지 않으면 0을 반환
int isEmpty(QUEUE *q) {
    return (q->count == 0 && q->front == NULL && q->rear == NULL);
}
