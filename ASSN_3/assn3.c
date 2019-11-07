#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define ROW 9
#define COL 9
#define CORNER 1000
#define LINE_HORI 2000
#define LINE_VERT 3000
#define CELL 4000

#define P1 1
#define P2 2
#define P 10
#define AI 20

#define EASY 100
#define NORMAL 200
#define HARD 300

void menu(int *mode);

void init_board(int board[][COL]);

void view_board(int board[][COL]);

int checkScore(int board[][COL], int playerNum);

void turn(int board[][9], int *player, int *nextPlayer, int *playerNum, int *nextPlayerNum, int *AI_option_1,
          int *AI_option_2);

void PvP(int board[][COL]);

void PvA(int board[][COL], int AI_option_1);

void printTurn(int player, int playerNum, int isNumNeeded);

void printLine();

void printMenu();

void getMenu(int *menu);

int checkType(int row, int col);

int isOdd(int x);

int isValid(int board[][COL], int row, int col);

int maxTurn();

void DEBUG_ARRAY(int board[][COL]);

void swap(int *x, int *y);

void boundary(int board[][9], int bound[], int row, int col);

int countBoundary(const int bound[]);

void AI_function(int *px, int *py, int board[][COL], int mode);

void AI_easy(int *pRow, int *pCol, int board[][COL]);

void AI_normal(int *pRow, int *pCol, int board[][COL]);

void AI_hard(int *pRow, int *pCol, int board[][COL]);

void AI_menu(int *mode);

int main() {
    int board[ROW][COL] = {0};
    int mode = 0, AI_option = 0;
    void (*mode_1)();

    srand(time(NULL));
    printf("Welcome to Dots and boxes!\n");

    init_board(board);
    menu(&mode);

    if (mode == 1) {
        PvP(board);
    } else if (mode == 2) {
        AI_menu(&AI_option);
        PvA(board, AI_option);
    } else if (mode == 3) {

    } else if (mode == 4) {

    }

}

void getInput() {

}

void init_board(int board[][COL]) {
    int row, col = 0;

    for (row = 0; row < ROW; row++) {
        for (col = 0; col < COL; col++) {
            board[row][col] = 0;
        }
    }
}

void view_board(int board[][COL]) {
    int row = 0, col = 0;
    int type = 0;
    int cur = 0;
    int scoreP1 = 0;
    int scoreP2 = 0;

    printf("\n");
    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            cur = board[row][col];
            if (cur == P1) {
                scoreP1++;
            } else if (cur == P2) {
                scoreP2++;
            }
        }
    }
    printLine();
    printf("%6d       vs%9d\n", scoreP1, scoreP2);
    printLine();

    for (row = 0; row < ROW; row++) {
        printf("%3d", row);
    }
    printf("\n");
    for (row = 0; row < ROW; row++) {
        printf("%-2d", row);
        for (col = 0; col < COL; col++) {
            cur = board[row][col];
            type = checkType(row, col);
            if (type == CORNER) {
                printf("+");
            } else if (type == LINE_VERT) {
                if (cur > 0) {
                    printf("|");
                } else {
                    printf(" ");
                }

            } else if (type == LINE_HORI) {
                if (cur > 0) {
                    printf(" --- ");
                } else {
                    printf("     ");
                }

            } else if (type == CELL) {
                if (cur == 1) {
                    printf("  O  ");
                } else if (cur == 2) {
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
    int bound[4] = {0};
    int bonus = 0;
    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            boundary(board, bound, row, col);
            if (countBoundary(bound) == 4) {
                if (board[row][col] == 0) {
                    board[row][col] = playerNum;
                    bonus = 1;
                }
            }

        }
    }

    return bonus;
}

void turn(int board[][9], int *player, int *nextPlayer, int *playerNum, int *nextPlayerNum, int *AI_option_1,
          int *AI_option_2) { // Turn 관련 함수
    int bonus = 0;
    int isNumNeeded = 0;
    int row = 0, col = 0;

    isNumNeeded = ((*player + *nextPlayer) != (P + AI));
    printTurn(*player, *playerNum, isNumNeeded);

    if (*player == P) {
        while (1) {
            printf("Select the position you want to draw.\n");
            scanf("%d %d", &row, &col);

            if (isValid(board, row, col)) {
                break;
            }
            printf("Impossible: Wrong position (Dot or box). Try again.\n");
        }

    } else if (*player == AI) {
        sleep(1);
        AI_function(&row, &col, board, AI_option_1);
        printf("The selected position by AI is %d %d\n", row, col);
    }

    board[row][col] = *playerNum;

    bonus = checkScore(board, *playerNum);
    view_board(board);

    if (bonus == 0) {
        swap(playerNum, nextPlayerNum);
        swap(player, nextPlayer);
        swap(AI_option_1, AI_option_2);
    }
}

void PvP(int board[][COL]) {
    int count = 0;
    int player = P;
    int nextPlayer = P;
    int playerNum = 1;
    int nextPlayerNum = 2;

    init_board(board);
    view_board(board);

    for (count = 0; count < maxTurn(); count++) {
        turn(board, &player, &nextPlayer, &playerNum, &nextPlayerNum, 0, NULL);
    }

}

void PvA(int board[][COL], int AI_option_1) {
    int count = 0;
    int playerNum = 1;
    int nextPlayerNum = 2;
    int player = P;
    int nextPlayer = AI;
    init_board(board);
    view_board(board);

    for (count = 0; count < maxTurn(); count++) {
        turn(board, &player, &nextPlayer, &playerNum, &nextPlayerNum, &AI_option_1, NULL);
    }
}

void AvA(int board[][COL], int mode1, int mode2){
    int count = 0;
    int playerNum = 1;
    int nextPlayerNum = 2;
    int player = AI;
    int nextPlayer = AI;

    init_board(board);
    view_board(board);


}

void printTurn(int player, int playerNum, int isNumNeeded) {
    printf("Turn: ");

    if (player == AI) {
        printf("AI ");
    } else if (player == P) {
        printf("Player ");
    }

    if (isNumNeeded) {
        printf("%d", playerNum);
    }

    printf("\n");
}

int getRand(int start, int end) {
    return rand() % (end - start) + start;
}

void getRandPoint(int *pRow, int *pCol, int board[][COL]){
    while (1) {
        *pRow = getRand(0, ROW);
        *pCol = getRand(0, COL);
        if (isValid(board, *pRow, *pCol)) {
            break;
        }
    }
}

void openBoundary(int board[][COL], int* pRow, int* pCol, int row, int col){
    int i = 0, j = 0;
    int tmpRow = 0, tmpCol = 0;
    int isLine = 0;

    for (i = -1; i < 2; i++){
        for (j = -1; j < 2; j++){
            tmpRow = row + i;
            tmpCol = col + j;
            isLine = ((checkType(tmpRow, tmpCol) == LINE_VERT) || (checkType(tmpRow, tmpCol) == LINE_HORI));
            printf("%d %d is Line? %d!", tmpRow, tmpCol, isLine);
            if(isLine && (board[tmpRow][tmpCol] == 0)){
                *pRow = tmpRow;
                *pCol = tmpCol;
            }
        }
    }
}

void AI_easy(int *pRow, int *pCol, int board[][COL]) {
    getRandPoint(pRow, pCol, board);
}

void AI_normal(int *pRow, int *pCol, int board[][COL]) {
    int row = 0, col = 0;
    int bound[4] = {0};

    *pRow = 0;
    *pCol = 0;

    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            boundary(board, bound, row, col);
            if(countBoundary(bound) == 3){
                printf("FIND OPEN BOUNDARY!\n");
                openBoundary(board, pRow, pCol, row, col);
                printf("CELL: %d %d \n", row, col);
                printf("EDGE: %d %d \n", *pRow, *pCol);
                break;
            }
        }
    }

    if((*pRow == 0)&&(*pCol == 0)){
        getRandPoint(pRow, pCol, board);
    }
}


void AI_hard(int *pRow, int *pCol, int board[][COL]) {

}

void AI_function(int *px, int *py, int board[][COL], int mode) {
    if (mode == EASY) {
        AI_easy(px, py, board);
    } else if (mode == NORMAL) {
        AI_normal(px, py, board);
    } else if (mode == HARD) {
        AI_hard(px, py, board);
    }
}

void print_record() {

}

void printLine() {
    printf("======================\n");
}

void printMenu() {
    printLine();
    printf("1. Game: Player vs Player\n");
    printf("2. Game: Player vs AI\n");
    printf("3. Game: AI vs AI\n");
    printf("4. Quit\n");
    printLine();
    printf("Select menu number: ");
}

void printAIMenu() {
    printLine();
    printf("1. Easy\n");
    printf("2. Normal\n");
    printf("3. Hard\n");
    printf("4. Quit\n");
    printLine();
    printf("Select menu number: ");
}

void getMenu(int *menu) {
    printMenu();
    scanf("%d", menu);
}

void getAIMenu(int *menu) {
    printAIMenu();
    scanf("%d", menu);
}

void menu(int *mode) {
    while (1) {
        getMenu(mode);
        if (*mode == 1 || *mode == 2 || *mode == 3 || *mode == 4) {
            break;
        } else {
            printf("Not valid input. Please try again.\n\n");
        }
    }
}

void AI_menu(int *mode) {
    while (1) {
        getAIMenu(mode);
        if (*mode == 1 || *mode == 2 || *mode == 3 || *mode == 4) {
            *mode = (*mode == 1) ? (EASY) : (((*mode == 2) ? (NORMAL) : (HARD)));
            break;
        } else {
            printf("Not valid input. Please try again.\n\n");
        }
    }
}

int checkType(int row, int col) {
    if (isOdd(col)) {
        if (isOdd(row)) {
            return CELL;
        } else {
            return LINE_HORI;
        }
    } else {
        if (isOdd(row)) {
            return LINE_VERT;
        } else {
            return CORNER;
        }
    }
}

void boundary(int board[][9], int bound[], int row, int col) {
    bound[0] = board[row - 1][col];
    bound[1] = board[row + 1][col];
    bound[2] = board[row][col - 1];
    bound[3] = board[row][col + 1];
}

int countBoundary(const int bound[]){
    int i = 0, count = 0;

    for(i = 0; i < 4; i++){
        count += (bound[i] > 0);
    }

    return count;
}

int isOdd(int x) {
    return (x % 2 == 1) ? (1) : (0);
}

int isValid(int board[][COL], int row, int col) {
    return (((checkType(row, col) == LINE_VERT) || (checkType(row, col) == LINE_HORI)) && (board[row][col] == 0));
}

int maxTurn() {
    int vert = ((ROW / 2) * (COL / 2 + 1));
    int hori = ((COL / 2) * (ROW / 2 + 1));
    return vert + hori;
}

void swap(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}