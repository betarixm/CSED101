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

#define MAX_TURN 40
#define MAX_SIMUL 100

#define DEPTH 5

void menu(int *mode);

int minimax(int board[][COL], int lines[][2], int *pRow, int *pCol, int player1_Num, int player2_Num, int isMyTurn, int prevScore, int depth);
int minimax_v2(int board[][COL], int lines[][2], int *pRow, int *pCol, int player1_Num, int player2_Num, int isMyTurn, int prevScore, int depth);

void init_board(int board[][COL]);

void view_board(const int board[][9], int *scoreP1, int *scoreP2);

int checkScore(int board[][COL], int playerNum);

void turn(int board[][9], int lines[MAX_TURN][2], int *player, int *nextPlayer, int *playerNum, int *nextPlayerNum, int *AI_option_1,
          int *AI_option_2, int *scoreP1, int *scoreP2, int currentTurn);

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

void swap(int *x, int *y);

void boundary(int board[][9], int bound[], int row, int col);

int countBoundary(const int bound[]);

void AI_function(int *px, int *py, int board[][9], int lines[][2], int AI_option, int meNum, int otherNum, int currentTurn);
void AI_easy(int *pRow, int *pCol, int board[][COL]);

void AI_normal(int *pRow, int *pCol, int board[][COL]);

void AI_hard(int *pRow, int *pCol, const int board[][9], int meNum, int otherNum, int currentTurn);

void AI_menu(int *mode);

void AvA(int board[][COL], int AI_option_1, int AI_option_2);

int getScore(int board[][COL], int playerNum);

void init_lines(int lines[][2]);

void game(int board[][COL], int player, int nextPlayer, int AI_option_1, int AI_option_2);

void setGame(int mode, int *player1, int *player2, int *AI_option_1, int *AI_option_2);

int main() {
    int board[ROW][COL] = {0};
    int mode = 4, AI_option_1 = 0, AI_option_2 = 0, player1 = 0, player2 = 0, input = 0;

    srand(time(NULL));
    printf("Welcome to Dots and boxes!\n");

    do {
        init_board(board);
        menu(&mode);
        if (mode == 4) {
            break;
        }
        setGame(mode, &player1, &player2, &AI_option_1, &AI_option_2);
        if (AI_option_1 == 4 || AI_option_2 == 4) {
            continue;
        }
        game(board, player1, player2, AI_option_1, AI_option_2);
        printf("Press Enter key to return to the Main Menu.");
        getchar();
        while (getchar() != '\n');
    } while (1);

    printf("Thank you for playing Dots and Boxes!");
    return 0;

}

void setGame(int mode, int *player1, int *player2, int *AI_option_1, int *AI_option_2) {
    if (mode == 1) {
        *player1 = P;
        *player2 = P;
    } else if (mode == 2) {
        *player1 = P;
        *player2 = AI;
        printf("Which difficulty do you want?\n");
        AI_menu(AI_option_2);
        if (*AI_option_1 == 4) {
            return;
        }
    } else if (mode == 3) {
        *player1 = AI;
        *player2 = AI;
        printf("Which difficulty do you want for AI 1?\n");
        AI_menu(AI_option_1);
        if (*AI_option_1 == 4) {
            return;
        }
        printf("Which difficulty do you want for AI 2?\n");
        AI_menu(AI_option_2);
        if (*AI_option_2 == 4) {
            return;
        }
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

void view_board(const int board[][9], int *scoreP1, int *scoreP2) {
    int row = 0, col = 0;
    int type = 0;
    int cur = 0;

    printf("\n");
    *scoreP1 = 0;
    *scoreP2 = 0;
    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            cur = board[row][col];
            if (cur == P1) {
                *scoreP1 += 1;
            } else if (cur == P2) {
                *scoreP2 += 1;
            }
        }
    }
    printLine();
    printf("%6d       vs%9d\n", *scoreP1, *scoreP2);
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

int calcScore(int board[][COL], int playerNum, int nextPlayerNum) {
    int row = 0, col = 0;
    int score = 0;
    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            if (board[row][col] == playerNum) {
                score += 1;
            }
            if (board[row][col] == nextPlayerNum) {
                score -= 1;
            }
        }
    }

    return score;
}

int getScore(int board[][COL], int playerNum) {
    int row = 0, col = 0;
    int score = 0;
    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            score += (board[row][col] == playerNum);
        }
    }

    return score;
}

void turn(int board[][9], int lines[][2], int *player, int *nextPlayer, int *playerNum, int *nextPlayerNum, int *AI_option_1,
          int *AI_option_2, int *scoreP1, int *scoreP2, int currentTurn) { // Turn 관련 함수
    int bonus = 0;
    int isNumNeeded = 0;
    int row = 0, col = 0;
    isNumNeeded = ((*player + *nextPlayer) != (P + AI));
    printTurn(*player, *playerNum, isNumNeeded);
    int printNumber = 0;

    if (*player == P) {
        while (1) {
            printf("Select the position you want to draw.\n");
            scanf("%d %d", &row, &col);

            if (isValid(board, row, col)) {
                break;
            }
            printf("Impossible: Wrong position (Dot or box). Try again.\n");
            getchar();
        }

    } else if (*player == AI) {
        if (*player == *nextPlayer) {
            usleep(500000);
        } else {
            usleep(1000000);
        }

        AI_function(&row, &col, board, lines,  *AI_option_1, *playerNum, *nextPlayerNum, currentTurn);

        if (*player == *nextPlayer) {
            printNumber = *playerNum + 48;
        } else {
            printNumber = 1;
        }
        printf("The selected position by AI %c is %d %d\n", printNumber, row, col);
    }

    board[row][col] = *playerNum;

    bonus = checkScore(board, *playerNum);
    view_board(board, scoreP1, scoreP2);

    if (bonus == 0) {
        swap(playerNum, nextPlayerNum);
        swap(player, nextPlayer);
        swap(AI_option_1, AI_option_2);
    }
}

void game(int board[][COL], int player, int nextPlayer, int AI_option_1, int AI_option_2) {
    int count = 0;
    int playerNum = 1;
    int nextPlayerNum = 2;
    int scoreP1 = 0, scoreP2 = 0;

    int player1_tmp = player;
    int player2_tmp = nextPlayer;

    int playerNum1_tmp = playerNum;
    int playerNum2_tmp = nextPlayerNum;

    int lines[MAX_TURN][2]= {0};

    init_lines(lines);
    init_board(board);
    view_board(board, &scoreP1, &scoreP2);

    for (count = 0; count < maxTurn(); count++) {
        turn(board, lines, &player, &nextPlayer, &playerNum, &nextPlayerNum, &AI_option_1, &AI_option_2, &scoreP1, &scoreP2,
             count);
    }

    if (scoreP1 == scoreP2) {
        printf("DRAW!\n");
    } else {
        if (scoreP1 < scoreP2) {
            if (player2_tmp == P) {
                printf("Player ");
            } else if (player2_tmp == AI) {
                printf("AI ");
            }

            if (player1_tmp == player2_tmp) {
                printf("%d ", playerNum2_tmp);
            }

        } else {
            if (player1_tmp == P) {
                printf("Player ");
            } else if (player1_tmp == AI) {
                printf("AI ");
            }

            if (player1_tmp == player2_tmp) {
                printf("%d ", playerNum1_tmp);
            }
        }

        printf("Wins!\n");

    }
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

void getRandPoint(int *pRow, int *pCol, int board[][COL]) {
    while (1) {
        *pRow = getRand(0, ROW);
        *pCol = getRand(0, COL);
        if (isValid(board, *pRow, *pCol)) {
            break;
        }
    }
}

int isLine(int row, int col) {
    return ((checkType(row, col) == LINE_VERT) || (checkType(row, col) == LINE_HORI));
}

void openBoundary(int board[][COL], int *pRow, int *pCol, int row, int col) {
    int i = 0, j = 0;
    int tmpRow = 0, tmpCol = 0;

    for (i = -1; i < 2; i++) {
        for (j = -1; j < 2; j++) {
            tmpRow = row + i;
            tmpCol = col + j;
            if (isLine(tmpRow, tmpCol) && (board[tmpRow][tmpCol] == 0)) {
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
            if (countBoundary(bound) == 3) { ;
                openBoundary(board, pRow, pCol, row, col);
                break;
            }
        }
    }

    if ((*pRow == 0) && (*pCol == 0)) {
        getRandPoint(pRow, pCol, board);
    }
}

void init_simulation(int sd[][MAX_SIMUL], int me, int other) {
    int i = 0;
    for (i = 0; i < MAX_TURN; i++) {
        sd[i][2] = me;
        sd[i][3] = i;
    }
}

void init_lines(int lines[][2]) {
    int row = 0, col = 0, idx = 0;
    for (row = 0; row < ROW; row++) {
        for (col = ((row % 2) == 0); col < COL; col += 2) {
            lines[idx][0] = row;
            lines[idx][1] = col;
            idx++;
        }
    }
}

void copyBoard(const int src[][COL], int dest[][COL]) {
    int row = 0, col = 0;

    for (row = 0; row < ROW; row++) {
        for (col = 0; col < COL; col++) {
            dest[row][col] = src[row][col];
        }
    }

}

int player_(int t) {
    return (t * 2) + 2;
}

int turn_(int t) {
    return (t * 2) + 3;
}

void print_SD(int sd[][MAX_SIMUL]) {
    int row = 0;
    int col = 0;

    printf("  sco       p_0  t_0  p_1  t_1  p_2  t_2  p_3  t_3\n");
    for (row = 0; row < MAX_TURN; row++) {
        for (col = 0; col < 6; col++) {
            printf("%5d", sd[row][col]);
        }
        printf("\n");
    }
}


void AI_hard(int *pRow, int *pCol, const int board[][9], int meNum, int otherNum, int currentTurn) {
    // printf("[+] I'M HARD!\n");
    // printf("[+] My number is %d, counter number is %d!\n", meNum, otherNum);
    int scoreP1 = 0, scoreP2 = 0;
    int sd[MAX_TURN][MAX_SIMUL] = {0};
    int lines[MAX_TURN][2] = {0};
    int simulBoard[ROW][COL] = {0};
    int i = 0, j = 0, t = 0, tmp_turn_0 = 0, tmpElement = 0, tmpScore = 0, tmpRow = 0, tmpCol = 0, tmpPlayerNum = 0, row_0 = 0, col_0 = 0, bonus = 0;
    int row = 0, col = 0, maxIdx = 0;
    int isAllScoreSame = 0;
    int countValid = 0;
    init_simulation(sd, meNum, otherNum);
    init_lines(lines);

    *pRow = 0;
    *pCol = 0;

    // printf("[+] Let's Start!\n");

    for (i = 0; i < MAX_TURN; i++) {
        copyBoard(board, simulBoard);
        bonus = 0;
        row_0 = lines[i][0];
        col_0 = lines[i][1];

        // printf("\n[+] Check about when I first choice row %d, col %d!\n", row_0, col_0);

        tmp_turn_0 = simulBoard[row_0][col_0];
        simulBoard[row_0][col_0] = meNum;
        bonus = checkScore(simulBoard, meNum);

        // printf("[+] Hmm.. I checked it. Let's see!");
        //view_board(simulBoard, &scoreP1, &scoreP2);

        sd[i][0] = calcScore(simulBoard, meNum, otherNum);

        if (bonus) {
            // printf("[+] Oh, I can get score! I got Bonus!\n");
            sd[i][player_(1)] = meNum;
        } else {
            // printf("[+] Oh, I can't get score at this time...\n");
            sd[i][player_(1)] = otherNum;
        }

        for (t = 1; t < 4; t++) {
            // printf("\n[+] Then, Let's see %dth future!\n", t);
            sd[i][turn_(t)] = -1;
            for (j = 0; j < MAX_TURN; j++) {
                tmpRow = lines[j][0];
                tmpCol = lines[j][1];
                tmpPlayerNum = sd[i][player_(t)];

                // printf("[+] Try with row %d, col %d with playerNum %d\n", tmpRow, tmpCol, tmpPlayerNum);
                if (isValid(simulBoard, tmpRow, tmpCol)) {
                    tmpElement = simulBoard[tmpRow][tmpCol];
                    simulBoard[tmpRow][tmpCol] = tmpPlayerNum;

                    bonus = checkScore(simulBoard, tmpPlayerNum);
                    tmpScore = calcScore(simulBoard, meNum, otherNum);

                    // printf("[+] Then, My score becomes %d\n", tmpScore);
                    //view_board(simulBoard, &scoreP1, &scoreP2);

                    if (tmpScore > sd[i][0]) {
                        // printf("[+] Oh, It's good than before!\n");
                        sd[i][turn_(t)] = j;
                        sd[i][0] = tmpScore;
                    }

                    if (bonus) {
                        if (tmpPlayerNum == meNum) {
                            sd[i][player_(t + 1)] = meNum;
                        } else {
                            sd[i][player_(t + 1)] = otherNum;
                        }
                    } else {
                        if (tmpPlayerNum == meNum) {
                            sd[i][player_(t + 1)] = otherNum;
                        } else {
                            sd[i][player_(t + 1)] = meNum;
                        }
                    }

                    simulBoard[tmpRow][tmpCol] = tmpElement;

                } else {
                    // printf("[+] But it's not valid position.. Let's try with another!\n");
                    continue;
                }
            }

            if (sd[i][turn_(t + 1)] == -1) {
                getRandPoint(&row, &col, simulBoard);
            } else {
                row = lines[turn_(t)][0];
                col = lines[turn_(t)][1];
            }

            simulBoard[row][col] = sd[i][player_(t)];
        }
    }

    isAllScoreSame = 1;
    for (i = 0; i < MAX_TURN; i++) {
        if (sd[i][0] != sd[0][0]) {
            isAllScoreSame = 0;
            break;
        }
    }

    maxIdx = 0;
    for (i = 0; i < MAX_TURN; i++) {
        if (sd[maxIdx][0] <= sd[i][0]) {
            if (isValid(board, lines[sd[i][turn_(0)]][0], lines[sd[i][turn_(0)]][1])) {
                maxIdx = i;
                countValid++;
            }
        }
    }

    print_SD(sd);

    if (isAllScoreSame || countValid == 0) {
        printf("[+] get RANDOM!\n");
        getRandPoint(pRow, pCol, board);
    } else {
        *pRow = lines[sd[maxIdx][turn_(0)]][0];
        *pCol = lines[sd[maxIdx][turn_(0)]][1];
    }



    // printf("[+] I can become score %d when row %d, col %d\n", sd[maxIdx][0], *pRow, *pCol);
}

void init_moves(int moves[], int* lenMoves){
    int i = 0;
    for(i = 0; i < *lenMoves; i++){
        moves[i] = 0;
    }
    *lenMoves = 0;
}

int count = 0;
int minimax(int board[][COL], int lines[][2], int *pRow, int *pCol, int player1_Num, int player2_Num, int isMyTurn, int prevScore, int depth){

    int simulBoard[ROW][COL] = {0};
    int currentScore = 0;
    int i = 0, j = 0, row = 0, col = 0, countLine = 0, bonus = 0, nextScore = 0, bestScore = 0, lenMoves = 0;
    int moves[40][2] = {0};

    // printf("[+] Start! Remain depth: %d\n", depth);
    if (depth <= 0){
        return prevScore;
    } else {
        for(i = 0; i < MAX_TURN; i++){
            printf("%d\n",i);
            row = lines[i][0];
            col = lines[i][1];
            // printf("\n[+] CHECK row %d, col %d...\n", row, col);
            if(isValid(board, row, col)){
                // printf("[+] It's Valid!\n");
                copyBoard(board, simulBoard);
                simulBoard[row][col] = player1_Num;
                bonus = checkScore(simulBoard, player1_Num);
                currentScore = calcScore(simulBoard, player1_Num, player2_Num);

                for(j = 0; j < MAX_TURN; j++){
                    if(!isValid(simulBoard, lines[j][0], lines[j][1])){
                        ++countLine;
                    } else {
                        break;
                    }
                }

                if(countLine == 40){
                    moves[lenMoves][0] = currentScore;
                    moves[lenMoves++][1] = -1;
                } else {
                    if(bonus == 0){
                        nextScore = minimax(simulBoard, lines,pRow, pCol, player2_Num, player1_Num, bonus, currentScore, depth - 1);
                    } else {
                        nextScore = minimax(simulBoard, lines, pRow, pCol, player1_Num, player2_Num, bonus, currentScore, depth - 1);
                    }


                    moves[lenMoves][0] = nextScore;
                    moves[lenMoves++][1] = i;
                }
            }
        }

        bestScore = moves[0][0];

        if(depth){
            printf("depth: %d, isMyTurn: %d, player1: %d \n", depth, isMyTurn, player1_Num);
        }
        if (isMyTurn){
            for (i = 0; i < lenMoves; i++){
                if(moves[i][0] > bestScore){
                    bestScore = moves[i][0];
                    *pRow = lines[moves[i][1]][0];
                    *pCol = lines[moves[i][1]][1];
                }
            }
        } else {
            for (i = 0; i  < lenMoves; i++){
                if(moves[i][0] < bestScore){
                    bestScore = moves[i][0];
                    *pRow = lines[moves[i][1]][0];
                    *pCol = lines[moves[i][1]][1];
                }
            }
        }

        return bestScore;
    }
}


int minimax_v2(int board[][COL], int lines[][2], int *pRow, int *pCol, int player1_Num, int player2_Num, int isMyTurn, int prevScore, int depth){

}

void print_moves(int moves[][2]){
    int i = 0;
    printf("\n  sco line\n");
    for (i=0; i < MAX_TURN; i++){
        printf("%5d %5d\n", moves[i][0], moves[i][1]);
    }
}

void print_board(int board[ROW][COL]){
    int i = 0, j = 0;
    for(i = 0; i < ROW; i++){
        for(j = 0; j < COL; j++){
            printf("%5d", board[i][j]);
        }
        printf("\n");
    }
}
int minimax_v3(int board[][COL], int lines[][2], int *pRow, int *pCol, int player1_Num, int player2_Num, int isPlayersTurn, int prevScore, int depth) {

    int row = 0, col = 0, pointValue = 1, tmp_player1 = player1_Num, tmp_player2 = player2_Num;
    int simulBoard[ROW][COL] = {0};
    int bonus = 0, currentScore = 0, countLine = 0, lenMoves = 0, nextScore = 0, bestScore = 0, i = 0, j = 0;
    int moves[MAX_TURN][2] = {0};
    int isValidLine = 0;
    int boundary[4] = {0};
    int bestRow = 0, bestCol = 0;
    int testRow = 0, testCol = 0;

    if(depth <= 0){
        return prevScore;
    } else {
        for (i = 0; i < MAX_TURN; i++){
            row = lines[i][0];
            col = lines[i][1];

            if(isValid(board, row, col)){
                copyBoard(board, simulBoard);
                /*
                 * if(isPlayersTurn == 0){
                    swap(&tmp_player1, &tmp_player2);
                }

                simulBoard[row][col] = tmp_player1;
                bonus = checkScore(simulBoard, tmp_player1);
                 *
                 */

                if(isPlayersTurn){
                    simulBoard[row][col] = player1_Num;
                    bonus = checkScore(simulBoard, player1_Num);
                } else {
                    simulBoard[row][col] = player2_Num;
                    bonus = checkScore(simulBoard, player2_Num);
                }

                currentScore = calcScore(simulBoard, player1_Num, player2_Num);
                for(j = 0; j < MAX_TURN; j++){
                    if(isValid(simulBoard, lines[j][0], lines[j][1])){
                        ++countLine;
                    }
                }

                if(countLine == 0){
                    moves[lenMoves][0] = currentScore;
                    moves[lenMoves++][1] = -1;
                } else {
                    if(depth >= (DEPTH)){
                        // printf("Case %d %d\n", row, col);
                    }

                    nextScore = minimax_v3(simulBoard, lines, pRow, pCol, player1_Num, player2_Num, ((bonus && isPlayersTurn) || ((bonus == 0) && (isPlayersTurn == 0))), currentScore, depth - 1);
                    moves[lenMoves][0] = nextScore;
                    moves[lenMoves++][1] = i;

                }
                if(depth > 1){
                    //printf("[+] Depth %d Row %d Col %d...\n", depth, row, col);
                }
                // print_moves(moves);
            }
        }

        // printf("Dep %d len %d\n", depth, lenMoves);

        bestScore = moves[0][0];
        *pRow = lines[moves[0][1]][0];
        *pCol = lines[moves[0][1]][1];
        for (i = 0; i < lenMoves; i++){
            bestRow = lines[moves[i][1]][0];
            bestCol = lines[moves[i][1]][1];
            isValidLine = isValid(board, bestRow, bestCol);

            if(depth == DEPTH){
                // printf("Dep: %d, i: %d, isPlayersTurn: %d, row: %d, col: %d, bestScore: %d, targetScore: %d, nextDepMove: %d\n", depth, i, isPlayersTurn, bestRow, bestCol, bestScore, moves[i][0], moves[i][1]);
            }
            if(isPlayersTurn == 1){
                if(moves[i][0] > bestScore){
                    // printf("[+] TEST %d LAST BEST %d\n", moves[i][0], bestScore);
                    bestScore = moves[i][0];
                    *pRow = lines[moves[i][1]][0];
                    *pCol = lines[moves[i][1]][1];
                    if(depth == DEPTH - 1){
                        // printf("[+] DEBUG SCORE %d ROW %d COL %d\n", bestScore, bestRow, bestCol);
                    }
                }
            } else {
                if((moves[i][0] < bestScore)){
                    // printf("[+] TEST %d LAST BEST %d\n", moves[i][0], bestScore);
                    bestScore = moves[i][0];
                    *pRow = lines[moves[i][1]][0];
                    *pCol = lines[moves[i][1]][1];
                }
            }


        }

        if(isValid(board, *pRow, *pCol) == 0){
            getRandPoint(pRow, pCol, board);
        }

        int a, b;
        if(depth > -1){
            // view_board(simulBoard, &a, &b);
            // print_board(simulBoard);
            // printf("[+] Best! depth: %d, Score: %d, row: %d, col: %d, isMyTurn: %d, tmp_player1: %d\n", depth, bestScore, *pRow, *pCol, isPlayersTurn, tmp_player1);
        }

        return bestScore;

    }

}

void AI_function(int *px, int *py, int board[][9], int lines[][2], int AI_option, int meNum, int otherNum, int currentTurn) {
    if (AI_option == EASY) {
        AI_easy(px, py, board);
    } else if (AI_option == NORMAL) {
        AI_normal(px, py, board);
    } else if (AI_option == HARD) {
        minimax_v3(board, lines, px, py, meNum, otherNum, 1, 0, DEPTH);
        // minimax(board, lines, px, py, meNum, otherNum, 1, 0, 3);
        // int minimax(int board[][COL], int *pRow, int *pCol, int player1_Num, int player2_Num, int isMyTurn, int prevScore, int depth)
        //AI_hard(px, py, board, meNum, otherNum, currentTurn);
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
            *mode = (*mode == 1) ? (EASY) : (
                    ((*mode == 2) ? (NORMAL) : (
                            ((*mode == 3) ? (HARD) : (*mode)))));
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

int countBoundary(const int bound[]) {
    int i = 0, count = 0;

    for (i = 0; i < 4; i++) {
        count += (bound[i] > 0);
    }

    return count;
}

int isOdd(int x) {
    return (x % 2 == 1) ? (1) : (0);
}

int isValid(int board[][COL], int row, int col) {
    return ((isLine(row, col)) && (board[row][col] == 0));
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

