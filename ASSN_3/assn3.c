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

void game(int player1, int player2, int AI_option_1, int AI_option_2);

void turn(int board[][COL], int lines[][2], int *player, int *nextPlayer, int *playerNum, int *nextPlayerNum, int *AI_option_1, int *AI_option_2, int *scoreP1, int *scoreP2, int currentTurn, FILE *file);

void init_lines(int lines[][2]);

void init_board(int board[][COL]);

void view_board(const int board[][9], int *scoreP1, int *scoreP2);

int setDifficulty(int *AI_option, int playerNum);

void setGame(int mode, int *player1, int *player2, int *AI_option_1, int *AI_option_2);

int getScore(const int board[][COL], int playerNum);

int checkScore(int board[][COL], int playerNum);

int checkType(int row, int col);

int calcValue(const int board[][COL], int playerNum, int nextPlayerNum);

void inputPlayer(const int board[][COL], int* row, int* col);

void printResult(int scoreP1, int scoreP2, int player1, int player2, int player1_num, int player2_num);

void printTurn(int player, int playerNum, int isNumNeeded);

int getRand(int start, int end);

void getRandPoint(const int board[][COL], int *pRow, int *pCol);

void openBoundary(const int board[][COL], int *pRow, int *pCol, int row, int col);

void AI_function(int *px, int *py, const int board[][9], int lines[][2], int AI_option, int player1_num, int player2_num, int turn_num);

void AI_easy(const int board[][COL], int *pRow, int *pCol);

void AI_normal(const int board[][COL], int *pCol, int *pRow);

void AI_hard(const int board[][COL], int lines[][2], int *px, int *py, int player1_num, int player2_num, int turn_num);

int alpha_beta_minimax(const int board[][COL], int lines[][2], int *pRow, int *pCol, int player1_Num, int player2_Num, int isPlayersTurn, int prevScore, int depth, int alpha, int beta);

void copyBoard(const int src[][COL], int dst[][COL]);

void print_record(int row, int col, FILE *file);

void printLine();

void printMenu();

void printAIMenu();

void getMenu(int *menu);

void getAIMenu(int *menu);

int inputMenu(int *mode, int option);

void menu(int *mode, int option);

void boundary(const int board[][9], int bound[], int row, int col);

int countBoundary(const int board[][9], int row, int col);

int isOdd(int x);

int isValid(const int board[][COL], int row, int col);

int isLine(int row, int col);

int max(int a, int b);

int min(int a, int b);

void swap(int *x, int *y);

int main() {
    int mode = 4, AI_option_1 = 0, AI_option_2 = 0, player1 = 0, player2 = 0;

    srand(time(NULL));
    printf("Welcome to Dots and boxes!\n");

    while(1) {
        menu(&mode, P);
        if (mode == 4) {
            break;
        }
        setGame(mode, &player1, &player2, &AI_option_1, &AI_option_2);
        if (AI_option_1 == 4 || AI_option_2 == 4) {
            continue;
        }
        game(player1, player2, AI_option_1, AI_option_2);
        printf("Press Enter key to return to the Main Menu.");
        getchar();
        while (getchar() != '\n');

    }

    printf("Thank you for playing Dots and Boxes!");
    return 0;

}

void game(int player1, int player2, int AI_option_1, int AI_option_2) {
    FILE* file;
    int lines[MAX_TURN][2]= {0};
    int board[ROW][COL] = {0};
    int count = 0, player1_num = P1, player2_num = P2, scoreP1 = 0, scoreP2 = 0;
    int tmp_player1 = player1, tmp_player2 = player2, tmp_player1_num = player1_num, tmp_player2_num = player2_num;

    if(player1 == P && player2 == P){
        file = fopen("PvP.txt", "w");
    } else if (player1 == AI && player2 == AI){
        file = fopen("AvA.txt", "w");
    } else {
        file = fopen("PvA.txt", "w");
    }

    init_lines(lines);
    init_board(board);
    view_board(board, &scoreP1, &scoreP2);

    for (count = 0; count < MAX_TURN; count++) {
        turn(board, lines, &player1, &player2, &player1_num, &player2_num, &AI_option_1, &AI_option_2, &scoreP1, &scoreP2, count, file);
    }

    fclose(file);
    printResult(scoreP1, scoreP2, tmp_player1, tmp_player2, tmp_player1_num, tmp_player2_num);
}

void turn(int board[][COL], int lines[][2], int *player, int *nextPlayer, int *playerNum, int *nextPlayerNum, int *AI_option_1,
          int *AI_option_2, int *scoreP1, int *scoreP2, int currentTurn, FILE *file) { // Turn 관련 함수
    int bonus = 0, isSamePlayer = 0, row = 0, col = 0, sleepTime = 0;

    isSamePlayer = (*player == *nextPlayer);

    if (*player == P) {
        printTurn(*player, *playerNum, isSamePlayer);
        inputPlayer(board, &row, &col);
    } else if (*player == AI) {
        sleepTime = (isSamePlayer)?(500000):(1000000);
        usleep(sleepTime);
        printTurn(*player, *playerNum, isSamePlayer);
        AI_function(&row, &col, board, lines,  *AI_option_1, *playerNum, *nextPlayerNum, currentTurn);
        printf("The selected position by AI %c is %d %d\n", (isSamePlayer)?(*playerNum + 48):(1), row, col);
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

void init_lines(int lines[][2]) {
    int row = 0, col = 0, idx = 0;
    for (row = 0; row < ROW; row++) {
        for (col = ((row % 2) == 0); col < COL; col += 2) {
            lines[idx][0] = row;
            lines[idx++][1] = col;
        }
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
    int row = 0, col = 0, type = 0, cur = 0, i = 0, isLineHori = 0, isLineVert = 0;
    int letters[5] = {'1', };

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

int setDifficulty(int *AI_option, int playerNum){
    printf("Which difficulty do you want");
    if(playerNum > 0){
        printf(" for AI %d?\n", playerNum);
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

int getScore(const int board[][COL], int playerNum) {
    int row = 0, col = 0, score = 0;
    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            score += (board[row][col] == playerNum);
        }
    }

    return score;
}

int checkScore(int board[][COL], int playerNum) {
    int row = 0, col = 0, bonus = 0;
    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            if ((countBoundary(board, row, col) == 4) && (board[row][col] == 0)) {
                board[row][col] = playerNum;
                bonus = 1;
            }
        }
    }

    return bonus;
}

int checkType(int row, int col) {
    return (isOdd(col))?
           ((isOdd(row))?(CELL):(LINE_HORI)):
           ((isOdd(row))?(LINE_VERT):(CORNER));
}

int calcValue(const int board[][COL], int playerNum, int nextPlayerNum) {
    int row = 0, col = 0, score = 0;
    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            score = score + (board[row][col] == playerNum) - (board[row][col] == nextPlayerNum);
        }
    }

    return score;
}

void inputPlayer(const int board[][COL], int* row, int* col){
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

void printResult(int scoreP1, int scoreP2, int player1, int player2, int player1_num, int player2_num){
    if (scoreP1 == scoreP2) {
        printf("DRAW!\n");
    } else {
        if (scoreP1 < scoreP2) {
            swap(&player1, &player2);
            swap(&player1_num, &player2_num);
        }
        if (player1 == P) {
            printf("Player ");
        } else if (player1 == AI) {
            printf("AI ");
        }
        if (player1 == player2) {
            printf("%d ", player1_num);
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

void getRandPoint(const int board[][COL], int *pRow, int *pCol) {
    do {
        *pRow = getRand(0, ROW);
        *pCol = getRand(0, COL);
    } while(isValid(board, *pRow, *pCol) == 0);
}

void openBoundary(const int board[][COL], int *pRow, int *pCol, int row, int col) {
    int i = 0, j = 0, tmpRow = 0, tmpCol = 0;
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

void AI_function(int *px, int *py, const int board[][9], int lines[][2], int AI_option, int player1_num, int player2_num, int turn_num) {
    if (AI_option == EASY) {
        AI_easy(board, px, py);
    } else if (AI_option == NORMAL) {
        AI_normal(board, py, px);
    } else if (AI_option == HARD) {
        AI_hard(board, lines, px, py, player1_num, player2_num, turn_num);
    }

}

void AI_easy(const int board[][COL], int *pRow, int *pCol) {
    getRandPoint(board, pRow, pCol);
}

void AI_normal(const int board[][COL], int *pCol, int *pRow) {
    int row = 0, col = 0;

    *pRow = 0;
    *pCol = 0;

    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            if (countBoundary(board, row, col) == 3) {
                openBoundary(board, pRow, pCol, row, col);
                break;
            }
        }
    }

    if ((*pRow == 0) && (*pCol == 0)) {
        getRandPoint(board, pRow, pCol);
    }
}

void AI_hard(const int board[][COL], int lines[][2], int *px, int *py, int player1_num, int player2_num, int turn_num){
    int depth = 0;

    if(turn_num < 15){
        depth = DEPTH - 1;
    } else if (turn_num < 25) {
        depth = DEPTH;
    } else {
        depth = DEPTH + 1;
    }

    alpha_beta_minimax(board, lines, px, py, player1_num, player2_num, 1, 0, depth, -999, 999);

}

int alpha_beta_minimax(const int board[][COL], int lines[][2], int *pRow, int *pCol, int player1_Num, int player2_Num, int isPlayersTurn, int prevScore, int depth, int alpha, int beta) {

    int row = 0, col = 0, extraTurn = 0, currentScore = 0, countLine = 0, lenMoves = 0, expectedValue = 0, optimalValue = 0, i = 0, j = 0, best = 1000;
    int simulBoard[ROW][COL] = {0};
    int moves[MAX_TURN][2] = {0};

    if(depth <= 0){
        return prevScore;
    } else {
        best = (isPlayersTurn)?(-1000):(1000);

        for (i = 0; i < MAX_TURN; i++){
            row = lines[i][0];
            col = lines[i][1];

            if(isValid(board, row, col)){
                copyBoard(board, simulBoard);

                if(isPlayersTurn){
                    simulBoard[row][col] = player1_Num;
                    extraTurn = checkScore(simulBoard, player1_Num);
                } else {
                    simulBoard[row][col] = player2_Num;
                    extraTurn = checkScore(simulBoard, player2_Num);
                }

                currentScore = calcValue(simulBoard, player1_Num, player2_Num);
                for(j = 0; j < MAX_TURN; j++){
                    if(isValid(simulBoard, lines[j][0], lines[j][1])){
                        ++countLine;
                    }
                }

                if(countLine == 0){
                    moves[lenMoves][0] = currentScore;
                    moves[lenMoves++][1] = -1;
                } else {
                    expectedValue = alpha_beta_minimax(simulBoard, lines, pRow, pCol, player1_Num, player2_Num, ((extraTurn && isPlayersTurn) || ((extraTurn == 0) && (isPlayersTurn == 0))), currentScore, depth - 1, alpha, beta);

                    if(isPlayersTurn){
                        best = max(best, expectedValue);
                        alpha = max(alpha, best);
                    } else {
                        best = min(best, expectedValue);
                        beta = min(beta, best);
                    }

                    moves[lenMoves][0] = expectedValue;
                    moves[lenMoves++][1] = i;

                    if (beta <= alpha){
                        break;
                    }
                }
            }
        }

        optimalValue = moves[0][0];
        *pRow = lines[moves[0][1]][0];
        *pCol = lines[moves[0][1]][1];
        for (i = 0; i < lenMoves; i++){
            if(isPlayersTurn){
                if(moves[i][0] > optimalValue){
                    optimalValue = moves[i][0];
                    *pRow = lines[moves[i][1]][0];
                    *pCol = lines[moves[i][1]][1];
                }
            } else {
                if((moves[i][0] < optimalValue)){
                    optimalValue = moves[i][0];
                    *pRow = lines[moves[i][1]][0];
                    *pCol = lines[moves[i][1]][1];
                }
            }

        }

        if(isValid(board, *pRow, *pCol) == 0){
            getRandPoint(board, pRow, pCol);
        }
        return optimalValue;
    }
}

void copyBoard(const int src[][COL], int dst[][COL]) {
    int row = 0, col = 0;
    for (row = 0; row < ROW; row++) {
        for (col = 0; col < COL; col++) {
            dst[row][col] = src[row][col];
        }
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
        *mode = (option == AI && *mode <= 3)?(100*(*mode)):(*mode);
        if(!(*mode == 4 && option == P)) {printf("\n");}
        return 0;
    } else {
        printf("Not valid input. Please try again.\n\n");

        return 1;
    }
}

void menu(int *mode, int option){
    do {
        if(option == AI){
            getAIMenu(mode);
        } else {
            getMenu(mode);
        }
    } while (inputMenu(mode, option));
}

void boundary(const int board[][9], int bound[], int row, int col) {
    bound[0] = board[row - 1][col];
    bound[1] = board[row + 1][col];
    bound[2] = board[row][col - 1];
    bound[3] = board[row][col + 1];
}

int countBoundary(const int board[][9], int row, int col) {
    int i = 0, count = 0;
    int bound[4] = {0};
    boundary(board, bound, row, col);
    for (i = 0; i < 4; i++) {
        count += (bound[i] > 0);
    }

    return count;
}

int isOdd(int x) {
    return (x % 2 == 1) ? (1) : (0);
}

int isValid(const int board[][COL], int row, int col) {
    return ((isLine(row, col)) && (board[row][col] == 0) && (row < ROW) && (col <COL));
}

int isLine(int row, int col) {
    return ((checkType(row, col) == LINE_VERT) || (checkType(row, col) == LINE_HORI));
}

int max(int a, int b){
    return (a>b)?(a):(b);
}

int min(int a, int b){
    return (a<b)?(a):(b);
}

void swap(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}