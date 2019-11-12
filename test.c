#include <stdio.h>

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
#define MAX_SIMUL 20

void init_lines(int lines[][2]){
    int row = 0, col = 0, idx = 0;
    for(row = 0; row < ROW; row++){
        for(col = ((row%2) == 0); col < COL; col+=2){
            lines[idx][0] = row;
            lines[idx][1] = col;
            idx++;
        }
    }
}

int main(){
    int lines[MAX_TURN][2] = {0};
    int i = 0;
    init_lines(lines);

    for (i = 0; i < MAX_TURN; i++){
        printf("%d %d\n", lines[i][0], lines[i][1]);
    }

    return 0;
}
