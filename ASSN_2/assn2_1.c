#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int getResponse(); // 사용자로부터 입력 받은 값을 반환한다.
int getRandomBalls(int start, int end); // [start, end] 범위의 랜덤 값을 반환한다.
int changeBallStatus(int current, int plus); // 현재 공 개수와 추가 공 개수를 더해서 반환한다. 15개 초과하는 경우를 여기서 처리한다.
void printCurrentBallStatus(int num); // 입력받은 만큼의 공 개수를 출력한다.
int movePlayer1(int player1, int player2); // player1의 공 개수를 변화시킨다.
int movePlayer2(int player1, int player2); // player2의 공 개수를 변화시킨다.
int isResponseValid(int response, int prevResponse); // 현재와 이전 response를 입력받아서 입력을 검증한다.
int calcWinner(int player1, int player2); // 승자 번호를 반환한다.
void printWinner(int player1, int player2); // 승자를 출력한다.
void printPlayerBallStatus(int p, int num); // 플레이어 번호와 printCurrentBallStatus를 호출하여 보여준다.
void printUnit(int num, int player1, int player2); // 게임 화면의 기본 단위를 출력한다.


int main(void){
    srand((unsigned int)time(NULL)); // 랜덤 함수를 위해 시드를 지정한다.

    int player1 = 0, player2 = 0; // 각 플레이어의 현재 공 개수.
    int prevInput = 3, input = 3; // 사용자의 이전, 현재 입력 값 저장.
    int isPlaying1 = 1, isPlaying2 = 1; // isPlaying1: player1이 게임을 진행 중인가? isPlaying2: player2가 게임을 진행 중인가?

    int count = 0; // 플레이어 한 명이 게임을 포기한 후 다른 플레이어가 얼마나 플레이 했는지 세는 변수.

    // 게임 초기 화면을 출력한다.
    printPlayerBallStatus(1, player1);
    printPlayerBallStatus(2, player2);
    printf("\n");

    while(1){ // 게임이 끝날 때 까지 무한 루프를 시작한다.
        prevInput = input; // 이전 입력 값을 미리 저장해놓는다.
        input = getResponse(); // 현재 입력 값을 입력받는다.

        if(isResponseValid(input, prevInput)){ // 입력이 유효한지 검사한다.
            printf("\n");

            //input에 따라서 player의 게임 진행 여부를 바꾼다. 0: 게임 종료, 1: 게임 진행 중
            isPlaying1 = (input == -1)?(0):(isPlaying1);
            isPlaying2 = (input == -2)?(0):(isPlaying2);

            if (!(isPlaying1 || isPlaying2)) break; // 플레이어 둘 다 playing 하고 있지 않다면 게임을 중단한다.

            if(isPlaying1){ // player1이 게임을 포기하지 않은 경우 player1의 공 개수를 바꿔주고 변화된 정보를 출력한다.
                player1 = movePlayer1(player1, player2);
                printUnit(1, player1, player2);
            }

            if(isPlaying2){ // player2가 게임을 포기하지 않은 경우 player2의 공 개수를 바꿔주고 변화된 정보를 출력한다.
                player2 = movePlayer2(player1, player2);
                printUnit(2, player1, player2);
            }

            count = count + (isPlaying1 != isPlaying2); // 둘 중 한 명만 playing 중인 상황이라면, count를 늘려준다.
            if (count == 3) break; // count가 3이 되면, 추가 턴을 모두 썼기에 게임을 종료한다.

        } else { // 입력이 유효하지 않을 경우 종료한다.
            printf("%d IS NOT VALID RESPONSE\n", input); // 잘못된 입력을 출력.
            break;
        }

    }

    printWinner(player1, player2); // 승자를 출력한다.

    return 0;
}

int getResponse(){ // 사용자로부터 입력을 받아 반환한다.
    int input = 0; // 입력 받은 값을 저장할 변수
    printf("PROCEED OR NOT? ENTER ANSWER WITH [GO 3, END –PLAYER NUM(-1 OR -2)] "); // 안내 메세지 출력
    scanf("%d", &input); // input에 저장
    return input; // 반환
}

int getRandomBalls(int start, int end){ // [start, end] 범위의 랜덤 값을 반환한다.
    return rand()%(end-start+1)+start; // 랜덤 값을 (end-start+1)로 나눈 나머지를 구하고, 시작 점을 더해서 주어진 범위의 랜덤 값을 반환한다.
}

int changeBallStatus(int current, int plus){ // 고정
    int sum = current + plus; // 우선 현재 값과 더할 값을 더한다.
    return (sum>15)? sum-15: sum; // 만약 sum이 15를 초과한다면 15를 뺀 값을, 아니면 그대로 반환한다.
}

void printCurrentBallStatus(int num){ // 현재 공 개수를 출력한다.
    int i = 0;

    for(i=0; i<num; i++){
        printf("●"); // num 만큼 검정 공을 출력한다.
    }

    for(i=0; i<(15-num); i++){
        printf("○"); // 15-num 만큼 빈 공을 출력한다.
    }

    printf("\n");
}

int movePlayer1(int player1, int player2){ // player1의 공 개수를 랜덤 값을 받아서 변경시킨다.
    return changeBallStatus(player1, getRandomBalls(1,5));
}

int movePlayer2(int player1, int player2){ // player2의 공 개수를 랜덤 값을 받아서 변경시킨다.
    return changeBallStatus(player2, getRandomBalls(1,5));
}

int isResponseValid(int response, int prevResponse){ // 사용자 의 입력값을 검증한다.
    int isInRange = (response == 3 || response == -1 || response == -2); // 입력 값이 3, -1, -2 중 하나인가?
    int isDupEnd = ((prevResponse==response) && (response==-1 || response==-2)); // 이전 입력 값과 입력 값이 같은데, 그 값이 -1이나 -2 인가?

    //isInRange이 아니면 0을 반환. (isDupEnd가 0이므로)
    //isInRange인데, isDupEnd이면 0을 반환.
    //isInRange인데, isDupEnd가 아니면 1을 반환.
    return isInRange - isDupEnd;
}

int calcWinner(int player1, int player2){ // 승자 번호를 반환한다.
    return (player1 > player2)?(1):((player1 == player2)?(0):(2)); // 점수에 따라 0, 1, 2를 반환한다.
}

void printWinner(int player1, int player2){ // 승자를 출력한다.
    int winner = calcWinner(player1, player2); // 승자 번호를 가져온다.
    if(winner == 0){
        printf("\nDRAW P1 %d, P2 %d\n", player1, player2); // 동점일 경우 무승부 결과를 출력한다.
    } else {
        printf("\nWIN P%d, P1 %d, P2 %d\n", winner, player1, player2); // 승부 결과를 출력한다.
    }

}

void printPlayerBallStatus(int p, int num){ // 플레이어의 공 개수를 플레이어 번호를 붙여서 출력한다.
    printf("P%d ", p); // 플레이어 번호 출력
    printCurrentBallStatus(num); // 공 개수 출력
}

void printUnit(int num, int player1, int player2){ // 게임의 가장 작은 출력 단위를 출력한다. [AFTER ~ P2 공 개수]
    printf("AFTER P%d\n", num); // AFTER 플레이어 번호 출력
    printPlayerBallStatus(1, player1); // 플레이어 1 공 출력
    printPlayerBallStatus(2, player2); // 플레이어 2 공 출력
    printf("\n");
}
