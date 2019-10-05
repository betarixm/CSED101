#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void season_print(int month);

int main(void){
    int random;
    int i=0;
    srand(time(NULL));

    for(i=0; i<3; i++){

        random = rand()%12 + 1;
        season_print(random);
    }

    return 0;
}

void season_print(int month){
    int r = month % 12;

    if (3 <= r && r < 6){
        printf("%2d월, 봄\n", month);
    } else if (6 <= r && r < 9){
        printf("%2d월, 여름\n", month);
    } else if (9 <= r && r < 12){
        printf("%2d월, 가을\n", month);
    } else if (0 <= r && r < 3){
        printf("%2d월, 겨울\n", month);
    }
}