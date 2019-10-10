#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int getResponse();
int getRandomBalls(int start, int end);
int changeBallStatus(int current, int plus);
void printCurrentBallStatus(int num);
int movePlayer1(int player1, int player2);
int movePlayer2(int player1, int player2);
int isResponseValid(int response, int prevResponse);
int calcWinner(int player1, int player2);
void printWinner(int player1, int player2);
void printPlayer(int p, int num);
void printRes(int num, int player1, int player2);


int main(void){
    srand(time(NULL));

    int player1 = 0, player2 = 0;
    int prevInput = 3, input = 3;
    int isPlaying1 = 1, isPlaying2 = 1;

    int count = 0;

    printPlayer(1, player1);
    printPlayer(2, player2);
    printf("\n");

    while(1){
        prevInput = input;
        input = getResponse();
        printf("\n");

        if(isResponseValid(input, prevInput)){
            isPlaying1 = (input == -1)?(0):(isPlaying1);
            isPlaying2 = (input == -2)?(0):(isPlaying2);

            if (!(isPlaying1 || isPlaying2)) break;

            if(isPlaying1){
                player1 = movePlayer1(player1, player2);
                printRes(1, player1, player2);
            }

            if(isPlaying2){
                player2 = movePlayer2(player1, player2);
                printRes(2, player1, player2);
            }

            count = count + (isPlaying1 != isPlaying2);
            if (count == 3) break;

        } else {
            printf("%d IS NOT VALID RESPONSE\n", input);
            break;
        }
    }

    printWinner(player1, player2);
}

int getResponse(){ // 고
    int res;
    printf("PROCEED OR NOT? ENTER ANSWER WITH [GO 3, END –PLAYER NUM(-1 OR -2) ");
    scanf("%d", &res);
    return res;
}

int getRandomBalls(int start, int end){ // 고정
    return rand()%(end-start)+start;
}

int changeBallStatus(int current, int plus){ // 고정
    int tmp = current + plus;
    return (tmp<16)? tmp: tmp-15;
}

void printCurrentBallStatus(int num){ // 고정
    int i = 0;

    for(i=0; i<num; i++){
        printf("●");
    }

    for(i=0; i<(15-num); i++){
        printf("○");
    }

    printf("\n");
}

int movePlayer1(int player1, int player2){ // 고정
    return changeBallStatus(player1, getRandomBalls(1,5));
}

int movePlayer2(int player1, int player2){ // 고정
    return changeBallStatus(player2, getRandomBalls(1,5));
}

int isResponseValid(int response, int prevResponse){ // 고정
    int isInRange = (response == 3 || response == -1 || response == -2);
    int isDupEnd = ((prevResponse==response && response==-1)||(prevResponse==response && response==-2));

    return (isInRange)?(((isDupEnd)?(0):(1))):(0);
}

int calcWinner(int player1, int player2){ // 고정
    return (player1 > player2)?(1):((player1 == player2)?(0):(2));
}

void printWinner(int player1, int player2){ // 고정
    int winner = calcWinner(player1, player2);
    if(winner == 0){
        printf("DRAW P1 %d, P2 %d", player1, player2);
    } else {
        printf("WIN P%d, P1 %d, P2 %d", winner, player1, player2);
    }

}

void printPlayer(int p, int num){
    printf("P%d ", p);
    printCurrentBallStatus(num);
}

void printRes(int num, int player1, int player2){
    printf("AFTER P%d\n", num);
    printPlayer(1, player1);
    printPlayer(2, player2);
    printf("\n");
}