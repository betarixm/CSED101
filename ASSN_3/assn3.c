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
#define DEPTH 6


int max(int a, int b);
int min(int a, int b);
void init_board(int board[][COL]);
int alpha_beta_minimax(int board[][COL], int lines[][2], int *pRow, int *pCol, int player1_Num, int player2_Num, int isPlayersTurn, int prevScore, int depth, int alpha, int beta);
void view_board(const int board[][9], int *scoreP1, int *scoreP2);
int checkScore(int board[][COL], int playerNum);
void print_record(int row, int col, FILE *file);
void
turn(int board[][9], int lines[][2], int *player, int *nextPlayer, int *playerNum, int *nextPlayerNum, int *AI_option_1,
     int *AI_option_2, int *scoreP1, int *scoreP2, int currentTurn, FILE *file);
void printTurn(int player, int playerNum, int isNumNeeded);
void printLine();
void printMenu();
void getMenu(int *menu);
int checkType(int row, int col);
int isOdd(int x);
int isValid(int board[][COL], int row, int col);
void swap(int *x, int *y);
void boundary(int board[][9], int bound[], int row, int col);
int countBoundary(const int bound[]);
void AI_function(int *px, int *py, int board[][9], int lines[][2], int AI_option, int meNum, int otherNum, int currentTurn);
void AI_easy(int *pRow, int *pCol, int board[][COL]);
void AI_normal(int *pRow, int *pCol, int board[][COL]);
void AI_hard(int board[][COL], int lines[][2], int *px, int *py, int meNum, int otherNum, int currentTurn);
void menu(int *mode, int option);
void init_lines(int lines[][2]);
void game(int board[][COL], int player, int nextPlayer, int AI_option_1, int AI_option_2);
void setGame(int mode, int *player1, int *player2, int *AI_option_1, int *AI_option_2);
int getScore(const int board[][COL], int playerNum);
int isLine(int row, int col);

int main() {
    int board[ROW][COL] = {0};
    int mode = 4, AI_option_1 = 0, AI_option_2 = 0, player1 = 0, player2 = 0;

    srand(time(NULL));
    printf("Welcome to Dots and boxes!\n");

    while(1) {
        init_board(board);
        menu(&mode, P);
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

    }

    printf("Thank you for playing Dots and Boxes!");
    return 0;

}

int setDifficulty(int *AI_option, int playerNum){
    printf("Which difficulty do you want");
    if(playerNum > 0){
        printf("for AI %d?\n", playerNum);
    } else {
        printf("?\n");
    }
    menu(AI_option, AI);

    return 1;
}

void setGame(int mode, int *player1, int *player2, int *AI_option_1, int *AI_option_2) {
    *player1 = (mode == 1 || mode == 2)?(P):(AI);
    *player2 = (mode == 1)?(P):(AI);

    if (mode == 2) {
        setDifficulty(AI_option_2, 0);
    } else if (mode == 3) {
        if (setDifficulty(AI_option_1, 1) && *AI_option_1 == 4) {return;}
        if (setDifficulty(AI_option_2, 2) && *AI_option_2 == 4) {return;}
    }
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
    int cur = 0, i = 0;
    int letters[5] = {'1', };
    int isLineHori = 0, isLineVert = 0;
    printf("\n");

    *scoreP1 = getScore(board, P1);
    *scoreP2 = getScore(board, P2);

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

            isLineHori = (type == LINE_HORI) && (cur > 0);
            isLineVert = (type == LINE_VERT) && (cur > 0);

            letters[0] = (type == CORNER || type == LINE_VERT)?(1):(' ');
            letters[1] = (type == CORNER || type == LINE_VERT)?(1):((isLineHori)?('-'):(' '));
            letters[2] = (type == CORNER)?('+'):((isLineVert)?('|'):((isLineHori)?('-'):((cur == 1)?('O'):((cur == 2)?('X'):(' ')))));
            letters[3] = letters[1];
            letters[4] = letters[0];

            for(i = 0; i < 5; i++){
                printf("%c", letters[i]);
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

int getScore(const int board[][COL], int playerNum) {
    int row = 0, col = 0;
    int score = 0;
    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            score += (board[row][col] == playerNum);
        }
    }

    return score;
}

void inputPlayer(int board[][COL], int* row, int* col){
    while (1) {
        printf("Select the position you want to draw.\n");
        scanf("%d %d", row, col);

        if (isValid(board, *row, *col)) {
            break;
        }

        printf("Impossible: Wrong position (");
        if ((isLine(*row, *col)) && board[*row][*col] > 0){
            printf("Already drew");
        } else {
            printf("Dot or box");
        }
        printf("). Try again.\n");
        getchar();
    }
}
void turn(int board[][COL], int lines[][2], int *player, int *nextPlayer, int *playerNum, int *nextPlayerNum, int *AI_option_1,
     int *AI_option_2, int *scoreP1, int *scoreP2, int currentTurn, FILE *file) { // Turn 관련 함수
    int bonus = 0;
    int isNumNeeded = 0;
    int row = 0, col = 0;
    isNumNeeded = ((*player + *nextPlayer) != (P + AI));

    int printNumber = 0;

    if (*player == P) {
        printTurn(*player, *playerNum, isNumNeeded);
        inputPlayer(board, &row, &col);
    } else if (*player == AI) {
        if (*player == *nextPlayer) {
            usleep(500000);
        } else {
            usleep(1000000);
        }

        printTurn(*player, *playerNum, isNumNeeded);

        AI_function(&row, &col, board, lines,  *AI_option_1, *playerNum, *nextPlayerNum, currentTurn);

        if (*player == *nextPlayer) {
            printNumber = *playerNum + 48;
        } else {
            printNumber = 1;
        }
        printf("The selected position by AI %c is %d %d\n", printNumber, row, col);
    }


    board[row][col] = *playerNum;
    print_record(row, col, file);
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
    int playerNum = P1;
    int nextPlayerNum = P2;
    int scoreP1 = 0, scoreP2 = 0;

    int player1_tmp = player;
    int player2_tmp = nextPlayer;

    int playerNum1_tmp = playerNum;
    int playerNum2_tmp = nextPlayerNum;

    int lines[MAX_TURN][2]= {0};

    FILE* file;

    if(player == P && nextPlayer == P){
        file = fopen("PvP.txt", "w");
    } else if (player == AI && nextPlayer == AI){
        file = fopen("AvA.txt", "w");
    } else {
        file = fopen("PvA.txt", "w");
    }

    init_lines(lines);
    init_board(board);
    view_board(board, &scoreP1, &scoreP2);

    for (count = 0; count < MAX_TURN; count++) {
        turn(board, lines, &player, &nextPlayer, &playerNum, &nextPlayerNum, &AI_option_1, &AI_option_2, &scoreP1, &scoreP2, count, file);
    }

    fclose(file);
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
            if (countBoundary(bound) == 3) {
                openBoundary(board, pRow, pCol, row, col);
                break;
            }
        }
    }

    if ((*pRow == 0) && (*pCol == 0)) {
        getRandPoint(pRow, pCol, board);
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

int minimax_v3(int board[][COL], int lines[][2], int *pRow, int *pCol, int player1_Num, int player2_Num, int isPlayersTurn, int prevScore, int depth) {

    int row = 0, col = 0;
    int simulBoard[ROW][COL] = {0};
    int bonus = 0, currentScore = 0, countLine = 0, lenMoves = 0, nextScore = 0, bestScore = 0, i = 0, j = 0;
    int moves[MAX_TURN][2] = {0};

    if(depth <= 0){
        return prevScore;
    } else {
        for (i = 0; i < MAX_TURN; i++){
            row = lines[i][0];
            col = lines[i][1];

            if(isValid(board, row, col)){
                copyBoard(board, simulBoard);

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
                    nextScore = minimax_v3(simulBoard, lines, pRow, pCol, player1_Num, player2_Num, ((bonus && isPlayersTurn) || ((bonus == 0) && (isPlayersTurn == 0))), currentScore, depth - 1);
                    moves[lenMoves][0] = nextScore;
                    moves[lenMoves++][1] = i;

                }
            }
        }

        bestScore = moves[0][0];
        *pRow = lines[moves[0][1]][0];
        *pCol = lines[moves[0][1]][1];
        for (i = 0; i < lenMoves; i++){

            if(isPlayersTurn == 1){
                if(moves[i][0] > bestScore){
                    bestScore = moves[i][0];
                    *pRow = lines[moves[i][1]][0];
                    *pCol = lines[moves[i][1]][1];
                }
            } else {
                if((moves[i][0] < bestScore)){
                    bestScore = moves[i][0];
                    *pRow = lines[moves[i][1]][0];
                    *pCol = lines[moves[i][1]][1];
                }
            }


        }

        if(isValid(board, *pRow, *pCol) == 0){
            getRandPoint(pRow, pCol, board);
        }
        return bestScore;
    }
}

int alpha_beta_minimax(int board[][COL], int lines[][2], int *pRow, int *pCol, int player1_Num, int player2_Num, int isPlayersTurn, int prevScore, int depth, int alpha, int beta) {

    int row = 0, col = 0, pointValue = 1, tmp_player1 = player1_Num, tmp_player2 = player2_Num;
    int simulBoard[ROW][COL] = {0};
    int bonus = 0, currentScore = 0, countLine = 0, lenMoves = 0, nextScore = 0, bestScore = 0, i = 0, j = 0;
    int moves[MAX_TURN][2] = {0};
    int isValidLine = 0;
    int boundary[4] = {0};
    int bestRow = 0, bestCol = 0;
    int testRow = 0, testCol = 0, best = 1000;

    if(depth <= 0){
        return prevScore;
    } else {

        if(isPlayersTurn){
            best = best * -1;
        }
        for (i = 0; i < MAX_TURN; i++){
            row = lines[i][0];
            col = lines[i][1];

            if(isValid(board, row, col)){
                copyBoard(board, simulBoard);

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

                    nextScore = alpha_beta_minimax(simulBoard, lines, pRow, pCol, player1_Num, player2_Num, ((bonus && isPlayersTurn) || ((bonus == 0) && (isPlayersTurn == 0))), currentScore, depth - 1, alpha, beta);


                    if(isPlayersTurn){
                        best = max(best, nextScore);
                        alpha = max(alpha, best);
                    } else {
                        best = min(best, nextScore);
                        beta = min(beta, best);
                    }

                    moves[lenMoves][0] = nextScore;
                    moves[lenMoves++][1] = i;

                    if (beta <= alpha){
                        break;
                    }
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
        if(depth > 1){
            // view_board(simulBoard, &a, &b);
            // print_board(simulBoard);
            // printf("[+] Best! depth: %d, Score: %d, row: %d, col: %d, isMyTurn: %d, tmp_player1: %d\n", depth, bestScore, *pRow, *pCol, isPlayersTurn, tmp_player1);
        }

        return bestScore;

    }

}

void AI_hard(int board[][COL], int lines[][2], int *px, int *py, int meNum, int otherNum, int currentTurn){
    int depth = 0;

    if(currentTurn < 15){
        depth = DEPTH - 1;
    } else if (currentTurn < 25) {
        depth = DEPTH;
    } else {
        depth = DEPTH + 1;
    }

    alpha_beta_minimax(board, lines, px, py, meNum, otherNum, 1, 0, depth, -999, 999);

}
void AI_function(int *px, int *py, int board[][9], int lines[][2], int AI_option, int meNum, int otherNum, int currentTurn) {
    int depth = 0;
    if (AI_option == EASY) {
        AI_easy(px, py, board);
    } else if (AI_option == NORMAL) {
        AI_normal(px, py, board);
    } else if (AI_option == HARD) {
        AI_hard(board, lines, px, py, meNum, otherNum, currentTurn);
    }

}

void print_record(int row, int col, FILE *file) {
    fprintf(file, " %d %d\n", row, col);
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

int inputMenu(int *mode, int option){
    if (*mode == 1 || *mode == 2 || *mode == 3 || *mode == 4) {

        *mode = (option == AI)?(100*(*mode)):(*mode);

        printf("\n");
        return 0;
    } else {
        printf("Not valid input. Please try again.\n\n");

        return 1;
    }
}

void menu(int *mode, int option){
    int isNotValid = 1;
    while (isNotValid) {
        if(option == AI){
            getAIMenu(mode);
        } else {
            getMenu(mode);
        }

        isNotValid = inputMenu(mode, option);
    }
}

int checkType(int row, int col) {
    return (isOdd(col))?
            ((isOdd(row))?(CELL):(LINE_HORI)):
            ((isOdd(row))?(LINE_VERT):(CORNER));
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
    return ((isLine(row, col)) && (board[row][col] == 0) && (row < ROW) && (col <COL));
}

void swap(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

int max(int a, int b){
    return (a>b)?(a):(b);
}

int min(int a, int b){
    return (a<b)?(a):(b);
}