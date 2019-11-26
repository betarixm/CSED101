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

    srand(time(NULL)); // 랜덤 값을 사용하기 위해 srand를 이용하여 초기화한다.
    printf("Welcome to Dots and boxes!\n");

    while(1) {
        // 무한루프이므로, 변수 초기화.
        mode = 4;
        AI_option_1 = 0;
        AI_option_2 = 0;
        player1 = 0;
        player2 = 0;

        menu(&mode, P); // 메인 메뉴 출력.
        if (mode == 4) { // 입력값이 4라면 종료한다.
            break;
        }
        setGame(mode, &player1, &player2, &AI_option_1, &AI_option_2); // setGame을 호출하여 게임에 관련된 매개변수들을 초기화한다.
        if (AI_option_1 == 4 || AI_option_2 == 4) { // 만약 옵션 둘 중 하나라도 4라면 메인 메뉴로 돌아간다.
            continue;
        }
        game(player1, player2, AI_option_1, AI_option_2); // game 시작.
        printf("Press Enter key to return to the Main Menu.");
        getchar();
        while (getchar() != '\n'); // 엔터키를 누를 때 까지 대기한다.

    }

    printf("Thank you for playing Dots and Boxes!");
    return 0;

}

void game(int player1, int player2, int AI_option_1, int AI_option_2) {
    FILE* file;
    int lines[MAX_TURN][2]= {0}; // 0 ~ 39 까지의 숫자와 라인의 좌표를 대응시키는 배열이다.
    int board[ROW][COL] = {0}; // 게임 메인 보드이다.
    int count = 0, player1_num = P1, player2_num = P2, scoreP1 = 0, scoreP2 = 0;
    int tmp_player1 = player1, tmp_player2 = player2, tmp_player1_num = player1_num, tmp_player2_num = player2_num; // tmp를 쓰는 이유는, 함수 turn에 의해 player 관련 변수들이 계속 바뀌기 때문이다.

    // 상황에 맞게 파일을 연다.
    if(player1 == P && player2 == P){
        file = fopen("PvP.txt", "w");
    } else if (player1 == AI && player2 == AI){
        file = fopen("AvA.txt", "w");
    } else {
        file = fopen("PvA.txt", "w");
    }

    init_lines(lines); // lines 배열을 초기화한다.
    init_board(board); // board 배열을 초기화한다.
    view_board(board, &scoreP1, &scoreP2); // 우선 보드를 보여준다.

    // 최대 턴 만큼 turn을 실행한다.
    for (count = 0; count < MAX_TURN; count++) {
        turn(board, lines, &player1, &player2, &player1_num, &player2_num, &AI_option_1, &AI_option_2, &scoreP1, &scoreP2, count, file);
    }

    fclose(file); // 파일 스트림을 닫는다.
    printResult(scoreP1, scoreP2, tmp_player1, tmp_player2, tmp_player1_num, tmp_player2_num); // 결과를 보여준다.
}

void turn(int board[][COL], int lines[][2], int *player, int *nextPlayer, int *playerNum, int *nextPlayerNum, int *AI_option_1,
          int *AI_option_2, int *scoreP1, int *scoreP2, int currentTurn, FILE *file) { // Turn 관련 함수
    int bonus = 0, isSamePlayer = 0, row = 0, col = 0, sleepTime = 0;

    isSamePlayer = (*player == *nextPlayer);

    if (*player == P) {
        printTurn(*player, *playerNum, isSamePlayer);
        inputPlayer(board, &row, &col); // 플레이어로 부터 좌표를 입력받는다.
    } else if (*player == AI) {
        sleepTime = (isSamePlayer)?(500000):(1000000);
        usleep(sleepTime); // 모드에 따라 우선 쉬어준다.
        printTurn(*player, *playerNum, isSamePlayer);
        AI_function(&row, &col, board, lines,  *AI_option_1, *playerNum, *nextPlayerNum, currentTurn); // 인공지능으로부터 좌표를 입력받는다.
        printf("The selected position by AI %c is %d %d\n", (isSamePlayer)?(*playerNum + 48):(1), row, col);
    }

    board[row][col] = *playerNum; // 입력받은 좌표를 기입한다.
    print_record(row, col, file); // 파일로 출력한다.
    bonus = checkScore(board, *playerNum); // 보드에 점수를 체크하고, 점수를 얻었다면 추가 턴을 얻는다.
    view_board(board, scoreP1, scoreP2); // 보드 출력

    if (bonus == 0) { // 추가 턴을 받지 못했다면, 플레이어들을 스왑해서 턴을 바꿔준다.
        swap(playerNum, nextPlayerNum);
        swap(player, nextPlayer);
        swap(AI_option_1, AI_option_2);
    }
}

void init_lines(int lines[][2]) { // lines 배열이 0 ~ 39의 자연수를 line에 해당하는 좌표로 일대일 대응시킬 수 있도록 초기화 해준다.
    int row = 0, col = 0, idx = 0;
    for (row = 0; row < ROW; row++) {
        for (col = ((row % 2) == 0); col < COL; col += 2) {
            lines[idx][0] = row;
            lines[idx++][1] = col;
        }
    }
}

void init_board(int board[][COL]) { // board를 모두 0으로 초기화한다.
    int row, col = 0;
    for (row = 0; row < ROW; row++) {
        for (col = 0; col < COL; col++) {
            board[row][col] = 0;
        }
    }
}

void view_board(const int board[][9], int *scoreP1, int *scoreP2) { // 보드를 출력한다.
    int row = 0, col = 0, type = 0, cur = 0, i = 0, isLineHori = 0, isLineVert = 0;
    int letters[5] = {'1', };

    printf("\n");

    // 점수를 받아온다.
    *scoreP1 = getScore(board, P1);
    *scoreP2 = getScore(board, P2);

    // 점수 출력
    printLine();
    printf("%6d       vs%9d\n", *scoreP1, *scoreP2);
    printLine();

    // 칼럼 숫자를 출력한다.
    for (row = 0; row < ROW; row++) {
        printf("%3d", row);
    }
    printf("\n");

    for (row = 0; row < ROW; row++) {
        printf("%-2d", row); // 로우 숫자 출력.
        for (col = 0; col < COL; col++) {
            cur = board[row][col];
            type = checkType(row, col);

            // 상황에 맞게 "+" 또는 "|" 또는 " " 또는 "     " 또는 " --- "를 출력한다.
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

int setDifficulty(int *AI_option, int playerNum){ // 인공지능의 난이도를 결정하는 함수이다.
    // 옵션에 따라 안내 멘트가 달라진다.
    printf("Which difficulty do you want");
    if(playerNum > 0){
        printf(" for AI %d?\n", playerNum);
    } else {
        printf("?\n");
    }
    menu(AI_option, AI); // 입력 값을 받는다.

    return 1;
}

void setGame(int mode, int *player1, int *player2, int *AI_option_1, int *AI_option_2) {
    *player1 = (mode == 1 || mode == 2)?(P):(AI);
    *player2 = (mode == 1)?(P):(AI);

    if (mode == 2) { // PvA 모드라면 AI 2의 난이도를 셋팅한다.
        setDifficulty(AI_option_2, 0);
    } else if (mode == 3) { // AVA 모드라면 AI 1, 2의 난이도를 셋팅한다.

        if (setDifficulty(AI_option_1, 1) && *AI_option_1 == 4) {return;} // 만약 옵션 값이 4라면 바로 리턴하여 메인 메뉴로 돌아갈 수 있게 한다.
        if (setDifficulty(AI_option_2, 2) && *AI_option_2 == 4) {return;} // 마찬가지.
    }
}

int getScore(const int board[][COL], int playerNum) { // board의 box를 읽어서 playerNum과 같다면 score을 하나 더하고, 반환한다.
    int row = 0, col = 0, score = 0;
    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            score += (board[row][col] == playerNum);
        }
    }

    return score;
}

int checkScore(int board[][COL], int playerNum) { // 점수를 얻었다면, 해당 칸에 playerNum을 기입하고, 추가턴 여부를 반환한다.
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

int checkType(int row, int col) { // 해당 row, col의 좌표가 무엇인지 반환한다. LINE_HORI: 가로줄, LINE_VERT: 세로줄 이다.
    return (isOdd(col))?
           ((isOdd(row))?(CELL):(LINE_HORI)):
           ((isOdd(row))?(LINE_VERT):(CORNER));
}

int calcValue(const int board[][COL], int playerNum, int nextPlayerNum) { // AI_hard에서 노드 가치를 매길 때 사용한다.
    int row = 0, col = 0, score = 0;
    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            score = score + (board[row][col] == playerNum) - (board[row][col] == nextPlayerNum); // 플레이어 숫자와 같다면 1을 더하고, 아니면 1을 뺀다.
        }
    }

    return score;
}

void inputPlayer(const int board[][COL], int* row, int* col){ // 플레이어로 부터 좌표를 입력받는다.
    while (1) {
        printf("Select the position you want to draw.\n");
        scanf("%d %d", row, col);

        if (isValid(board, *row, *col)) { // 만약 올바른 좌표라면 탈출한다.
            break;
        }

        // 아니라면, 잘못된 이유를 출력한다.
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

void printResult(int scoreP1, int scoreP2, int player1, int player2, int player1_num, int player2_num){ // 결과를 출력한다.
    if (scoreP1 == scoreP2) { // 동점일 경우.
        printf("DRAW!\n");
    } else {
        if (scoreP1 < scoreP2) { // 플레이어 2가 더 높을 경우 1과 2를 스왑한다.
            swap(&player1, &player2);
            swap(&player1_num, &player2_num);
        }
        // 플레이어 1의 승리에 대해 출력한다.
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

void printTurn(int player, int playerNum, int isNumNeeded) { // 턴 정보 출력 함수
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

int getRand(int start, int end) { // start 이상 end 미만의 랜덤 값을 반환한다.
    return rand() % (end - start) + start;
}

void getRandPoint(const int board[][COL], int *pRow, int *pCol) { // Valid한 값이 나올 때 까지 랜덤 좌표를 기입한다.
    do {
        *pRow = getRand(0, ROW);
        *pCol = getRand(0, COL);
    } while(isValid(board, *pRow, *pCol) == 0);
}

void openBoundary(const int board[][COL], int *pRow, int *pCol, int row, int col) { // 열린 선이 있다면, 체크한다. countBoundary==3 인지 체크하는 과정이 선행되어야 한다.
    int i = 0, j = 0, tmpRow = 0, tmpCol = 0;
    for (i = -1; i < 2; i++) {
        for (j = -1; j < 2; j++) {
            // i, j: row, col 주변 칸들.
            tmpRow = row + i;
            tmpCol = col + j;
            if (isLine(tmpRow, tmpCol) && (board[tmpRow][tmpCol] == 0)) { // i, j가 line이고, 비어 있다면,
                *pRow = tmpRow;
                *pCol = tmpCol;
            }
        }
    }
}

void AI_function(int *px, int *py, const int board[][9], int lines[][2], int AI_option, int player1_num, int player2_num, int turn_num) {
    // AI 난이도에 맞게 AI 함수들을 호출한다.
    if (AI_option == EASY) {
        AI_easy(board, px, py);
    } else if (AI_option == NORMAL) {
        AI_normal(board, py, px);
    } else if (AI_option == HARD) {
        AI_hard(board, lines, px, py, player1_num, player2_num, turn_num);
    }

}

void AI_easy(const int board[][COL], int *pRow, int *pCol) { // getRandPoint를 통해 Valid한 랜덤 좌표를 기입.
    getRandPoint(board, pRow, pCol);
}

void AI_normal(const int board[][COL], int *pCol, int *pRow) {
    int row = 0, col = 0;

    *pRow = 0;
    *pCol = 0;

    for (row = 1; row < ROW; row += 2) {
        for (col = 1; col < COL; col += 2) {
            if (countBoundary(board, row, col) == 3) { // 만약 박스 주변의 선이 3개라면,
                openBoundary(board, pRow, pCol, row, col); // 빈 선의 좌표를 기입.
                break;
            }
        }
    }

    if ((*pRow == 0) && (*pCol == 0)) { // 박스 주변에 선이 3개인 곳이 없다면,
        getRandPoint(board, pRow, pCol); // valid한 랜덤 좌표를 기입.
    }
}

void AI_hard(const int board[][COL], int lines[][2], int *px, int *py, int player1_num, int player2_num, int turn_num){
    int depth = 0;

    // 현재 턴의 진행에 따라 depth를 조절한다.
    if(turn_num < 15){
        depth = DEPTH - 1;
    } else if (turn_num < 25) {
        depth = DEPTH;
    } else {
        depth = DEPTH + 1;
    }

    // alpha_beta_minimax를 실행한다.
    alpha_beta_minimax(board, lines, px, py, player1_num, player2_num, 1, 0, depth, -999, 999);

}

int alpha_beta_minimax(const int board[][COL], int lines[][2], int *pRow, int *pCol, int player1_Num, int player2_Num, int isPlayersTurn, int prevScore, int depth, int alpha, int beta) {

    // alpha_beta_minimax의 이론적/자세한 이야기는 보고서를 참조해주세요.
    // 아주 가끔씩 Segmentation Fault가 발생할 수 있습니다.
    // isPlayersNum: AI의 턴 일때 True.

    int row = 0, col = 0, extraTurn = 0, currentScore = 0, countLine = 0, lenArr = 0, childOptimalValue = 0, optimalValue = 0, i = 0, j = 0, best = 1000;
    int simulBoard[ROW][COL] = {0};
    int childList[MAX_TURN][2] = {0};

    if(depth <= 0){ // depth가 0 이하라면 prevScore 반환.
        return prevScore;
    } else {
        best = (isPlayersTurn)?(-1000):(1000); // best한 점수.

        for (i = 0; i < MAX_TURN; i++){
            // lines는 좌표가 대응되는 배열. init_lines 참고.
            row = lines[i][0];
            col = lines[i][1];

            if(isValid(board, row, col)){ // Valid한 line에 대해서
                copyBoard(board, simulBoard); // 우선 시뮬레이션 보드로 복사합시다.

                // 현재 플레이어에 맞게 시뮬레이션 보드에 기입합니다.
                if(isPlayersTurn){
                    simulBoard[row][col] = player1_Num;
                    extraTurn = checkScore(simulBoard, player1_Num);
                } else {
                    simulBoard[row][col] = player2_Num;
                    extraTurn = checkScore(simulBoard, player2_Num);
                }

                // 시뮬레이션 점수를 계산합니다.
                currentScore = calcValue(simulBoard, player1_Num, player2_Num);

                for(j = 0; j < MAX_TURN; j++){ // 현재 가능한 시뮬레이션 보드 선 개수 체크
                    if(isValid(simulBoard, lines[j][0], lines[j][1])){
                        ++countLine;
                    }
                }

                if(countLine == 0){ // 더이상 가능한 선이 없다면
                    childList[lenArr][0] = currentScore; // childList에 일단 push.
                    childList[lenArr++][1] = -1;
                } else { // 아니라면,

                    childOptimalValue = alpha_beta_minimax(simulBoard, lines, pRow, pCol, player1_Num, player2_Num, ((extraTurn && isPlayersTurn) || ((extraTurn == 0) && (isPlayersTurn == 0))), currentScore, depth - 1, alpha, beta);
                    // 재귀 호출을 통해 child 노드에서 opt 한 값을 구한다.
                    if(isPlayersTurn){
                        best = max(best, childOptimalValue);
                        alpha = max(alpha, best);
                    } else {
                        best = min(best, childOptimalValue);
                        beta = min(beta, best);
                    }

                    childList[lenArr][0] = childOptimalValue; // childLsit에 push 하고,
                    childList[lenArr++][1] = i;

                    if (beta <= alpha){ // 알파컷/베타컷을 진행한다.
                        break;
                    }
                }
            }
        }

        // 각 상황에서 최적의 값을 찾고, 그 때의 좌표를 기입한다.
        optimalValue = childList[0][0];
        *pRow = lines[childList[0][1]][0];
        *pCol = lines[childList[0][1]][1];
        for (i = 0; i < lenArr; i++){
            if(isPlayersTurn){
                if(childList[i][0] > optimalValue){
                    optimalValue = childList[i][0];
                    *pRow = lines[childList[i][1]][0];
                    *pCol = lines[childList[i][1]][1];
                }
            } else {
                if((childList[i][0] < optimalValue)){
                    optimalValue = childList[i][0];
                    *pRow = lines[childList[i][1]][0];
                    *pCol = lines[childList[i][1]][1];
                }
            }

        }

        // 만약 해당 좌표가 올바르지 않다면, 랜덤 좌표 기입.
        if(isValid(board, *pRow, *pCol) == 0){
            getRandPoint(board, pRow, pCol);
        }

        // 현재 최적의 점수를 반환.
        return optimalValue;
    }
}

void copyBoard(const int src[][COL], int dst[][COL]) { // 2차원 보드를 src에서 dst로 복사한다.
    int row = 0, col = 0;
    for (row = 0; row < ROW; row++) {
        for (col = 0; col < COL; col++) {
            dst[row][col] = src[row][col];
        }
    }
}

void print_record(int row, int col, FILE *file) { // 파일로 출력한다.
    fprintf(file, " %d %d\n", row, col);
}

void printLine() { // "="으로 이루어진 선 출력.
    printf("======================\n");
}

void printMenu() { // 메인메뉴 출력
    printLine();
    printf("1. Game: Player vs Player\n");
    printf("2. Game: Player vs AI\n");
    printf("3. Game: AI vs AI\n");
    printf("4. Quit\n");
    printLine();
    printf("Select menu number: ");
}

void printAIMenu() { // AI 난이도 선택 메뉴 출력
    printLine();
    printf("1. Easy\n");
    printf("2. Normal\n");
    printf("3. Hard\n");
    printf("4. Quit\n");
    printLine();
    printf("Select menu number: ");
}

void getMenu(int *menu) { // 메인메뉴 입력
    printMenu();
    scanf("%d", menu);
}

void getAIMenu(int *menu) { // AI 난이도 입력
    printAIMenu();
    scanf("%d", menu);
}

int inputMenu(int *mode, int option){  // 사용자 입력을 정제하고, valid한지 체크한다.
    if (*mode == 1 || *mode == 2 || *mode == 3 || *mode == 4) {
        *mode = (option == AI && *mode <= 3)?(100*(*mode)):(*mode);
        if(!(*mode == 4 && option == P)) {printf("\n");}
        return 0;
    } else {
        printf("Not valid input. Please try again.\n\n");

        return 1;
    }
}

void menu(int *mode, int option){ // 컨디션에 맞게 메뉴를 출력한다.
    do {
        if(option == AI){
            getAIMenu(mode);
        } else {
            getMenu(mode);
        }
    } while (inputMenu(mode, option));
}

void boundary(const int board[][9], int bound[], int row, int col) { // row, col을 기준으로 위, 아래, 왼쪽, 오른쪽의 값을 bound에 넣어준다.
    bound[0] = board[row - 1][col];
    bound[1] = board[row + 1][col];
    bound[2] = board[row][col - 1];
    bound[3] = board[row][col + 1];
}

int countBoundary(const int board[][9], int row, int col) { // bound 배열을 받아와서, 몇개가 채워져 있는지 그 개수를 반환한다.
    int i = 0, count = 0;
    int bound[4] = {0};
    boundary(board, bound, row, col);
    for (i = 0; i < 4; i++) {
        count += (bound[i] > 0);
    }

    return count;
}

int isOdd(int x) { // x가 홀수인지 반환.
    return (x % 2 == 1) ? (1) : (0);
}

int isValid(const int board[][COL], int row, int col) { // row, col이 입력 가능한 곳인지 여부를 반환한다.
    // 선이어야하며, 보드에 비어있어야하고, 보드 범위 안이어야 함.
    return ((isLine(row, col)) && (board[row][col] == 0) && (row < ROW) && (col <COL));
}

int isLine(int row, int col) { // row, col이 선인지 반환한다.
    return ((checkType(row, col) == LINE_VERT) || (checkType(row, col) == LINE_HORI));
}

int max(int a, int b){ // 둘 중 큰 값을 반환한다.
    return (a>b)?(a):(b);
}

int min(int a, int b){ // 둘 중 작은 값을 반환한다.
    return (a<b)?(a):(b);
}

void swap(int *x, int *y) { // 두 값을 바꿔준다.
    int tmp = *x;
    *x = *y;
    *y = tmp;
}