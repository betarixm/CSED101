#include <stdio.h>
#define ROW 9
#define COL 9
#define CORNER 1000
#define LINE_HORI 2000
#define LINE_VERT 3000
#define CELL 4000

#define P1 1
#define P2 2
#define P 0
#define AI 10

/*
 * ■ int main()
게임판을 나타내는 적절한 2차원 배열을 main 함수에서 선언 후 사용해야 합니다.
■ void init_board(...)
전달 받은 게임판(2차원 배열)을 게임 시작을 위하여 적절한 값으로 채워 넣는 역할을 합니다.
■ 콘솔 출력 관련: void view_board(...)
게임이 진행 중일 때 게임판을 콘솔에 출력하는 역할을 합니다. 해당 함수는 2차원 배열(게임판)을 매개변수로 포함하고 있어야 합니다.
■ Turn 관련
게임의 Turn과 관련하여 적절한 이름의 사용자 정의 함수를 정의하여 구현해야 합니다. Turn 관련 사용자 함수는 주석을 통해서 명시해주시고, 정의한 함수가 어떤 기능을 하는지 적어주십시오.
■ 게임 AI 관련: void AI_function(int *px, int *py, ...)
이 함수는 게임의 AI를 구현하는 부분으로 레벨(Easy, Normal, Hard)에 따라 적절하게 좌표를 선택한 후, 포인터 변수 px, py를 통하여 전달합니다. 해당 함수는 2차원 배열을 매개변수로 포함하고 있어야 합니다.
■ 파일 출력 관련: void print_record(...)
진행되는 게임의 기보를 파일에 기록합니다.
 */

void init_board(int board[][COL]);

void view_board(int board[][COL]);

int checkScore(int board[][COL], int playerNum);

int turn(int board[][COL], int player, int *playerNum, int *nextPlayerNum, int row, int col);

void PvP(int board[][COL]);

void printTurn(int player, int playerNum);

void AI_function();

void print_record();

void printLine();

void printMenu();

void menu(int* menu);

int checkType(int row, int col);

int isOdd(int x);

int isValid(int board[][COL], int row, int col);

int maxTurn();

void DEBUG_ARRAY(int board[][COL]);

void swap(int* x, int* y);

int main(){
    int board[ROW][COL] = {0};
    int mode = 0;

    printf("Welcome to Dots and boxes!\n");

    init_board(board);

    while(1){
        menu(&mode);

        if(mode == 1){
            PvP(board);
            break;
        } else if(mode == 2){
            break;
        } else if(mode == 3){
            break;
        } else if(mode == 4){
            break;
        } else {
            printf("Not valid input. Please try again.\n\n");
        }
    }

    view_board(board);



}

void init_board(int board[][COL]){
    int row, col = 0;

    for(row = 0; row < ROW; row++){
        for(col = 0; col < COL; col++){
            board[row][col] = 0;
        }
    }
}

void view_board(int board[][COL]){
    int row = 0, col = 0;
    int type = 0;
    int cur = 0;
    int scoreP1 = 0;
    int scoreP2 = 0;

    printf("\n");
    for(row = 1; row < ROW; row += 2){
        for(col = 1; col < COL; col += 2){
            cur = board[row][col];
            if(cur == P1){
                scoreP1++;
            } else if (cur == P2) {
                scoreP2++;
            }
        }
    }
    printLine();
    printf("%6d       vs%9d\n", scoreP1, scoreP2);
    printLine();

    for(row=0; row < ROW; row++){
        printf("%3d", row);
    }
    printf("\n");
    for(row = 0; row < ROW; row++){
        printf("%-2d", row);
        for(col = 0; col < COL; col++){
            cur = board[row][col];
            type = checkType(row, col);
            if(type == CORNER){
                printf("+");
            } else if(type == LINE_VERT){
                if(cur > 0){
                    printf("|");
                } else {
                    printf(" ");
                }

            } else if(type == LINE_HORI){
                if(cur > 0){
                    printf(" --- ");
                } else {
                    printf("     ");
                }

            } else if(type == CELL){
                if(cur == 1){
                    printf("  O  ");
                } else if(cur == 2){
                    printf("  X  ");
                } else {
                    printf("     ");
                }

            }
        }
        printf("\n");
    }

    printLine();

}

int checkScore(int board[][COL], int playerNum) {
    int row = 0, col = 0;
    int up = 0, down = 0, left = 0, right = 0;
    int bonus = 0;
    for(row = 1; row < ROW; row += 2){
        for(col = 1; col < COL; col += 2){
            up = board[row - 1][col];
            down = board[row + 1][col];
            left = board[row][col - 1];
            right = board[row][col + 1];

            if(up * down * left * right != 0){
                if(board[row][col] == 0){
                    board[row][col] = playerNum;
                    bonus = 1;
                }
            }

        }
    }

    return bonus;
}

int turn(int board[][COL], int player, int *playerNum, int *nextPlayerNum, int row, int col) { // Turn 관련 함수
    int bonus = 0;
    printTurn(player, *playerNum);

    if(player == P){
        while(1){
            printf("Select the position you want to draw.\n");
            scanf("%d %d", &row, &col);

            if(isValid(board, row, col)){
                break;
            }
            printf("Impossible: Wrong position (Dot or box). Try again.\n");
        }

    } else {
        printf("The selected position by AI is %d %d\n\n", row, col);
    }

    board[row][col] = *playerNum;

    bonus = checkScore(board, *playerNum);
    view_board(board);

    if(bonus == 0){
        swap(playerNum, nextPlayerNum);
    }
}

void PvP(int board[][COL]){
    int count = 0;
    int playerNum = 1;
    int nextPlayerNum = 2;

    init_board(board);
    view_board(board);

    while(count < maxTurn()){
        turn(board, P, &playerNum, &nextPlayerNum, 0, 0);
        count++;
    }

}

void printTurn(int player, int playerNum){
    printf("Turn: ");

    if(player == AI){
        printf("AI ");
    } else if (player == P) {
        printf("Player ");
    }

    if(playerNum != 0){
        printf("%d", playerNum);
    }

    printf("\n");
}

void AI_function(){

}

void print_record(){

}

void printLine(){
    printf("======================\n");
}

void printMenu(){
    printLine();
    printf("1. Game: Player vs Player\n");
    printf("2. Game: Player vs AI\n");
    printf("3. Game: AI vs AI\n");
    printf("4. Quit\n");
    printLine();
    printf("Select menu number: ");
}

void menu(int* menu){
    printMenu();
    scanf("%d", menu);
}

int checkType(int row, int col){
    if(isOdd(col)){
        if(isOdd(row)){
            return CELL;
        } else {
            return LINE_HORI;
        }
    } else {
        if(isOdd(row)){
            return LINE_VERT;
        } else {
            return CORNER;
        }
    }
}

int isOdd(int x){
    return (x % 2 == 1)?(1):(0);
}

int isValid(int board[][COL], int row, int col){
    int type = checkType(row, col);
    /*
    if (type == LINE_HORI){
        printf("line_hori\n");
    }
    if (type == LINE_VERT){
        printf("line_vert\n");
    }
    if (type == CORNER) {
        printf("corner\n");
    }
    if (type == CELL){
        printf("cell\n");
    }
     */
    return (((checkType(row, col) == LINE_VERT) || (checkType(row, col) == LINE_HORI)) && (board[row][col]==0));
}

int maxTurn(){
    int vert = ((ROW/2) * (COL/2+1));
    int hori = ((COL/2) * (ROW/2+1));
    return vert + hori;
}

void DEBUG_ARRAY(int board[][COL]){
    int row = 0, col = 0;

    for(row = 0; row < ROW; row++){
        for(col = 0; col <COL; col++){
            printf("%3d ", board[row][col]);
        }
        printf("\n");
    }
}

void swap(int* x, int* y){
    int tmp = *x;
    *x = *y;
    *y = tmp;
}