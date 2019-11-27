#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void print_init();
void print_menu(char imagename[30]);
void array_allocation(int ***R, int ***G, int ***B, int width, int height);
void rgb_array(int **R, int **G, int **B, int width, int height, FILE* infile);
void rgb_array_2(int ***R, int ***G, int ***B, int width, int height, FILE* infile);

int main() {
    int menu = 0;
    int **R = 0, **G = 0, **B = 0;
    int i, width = 0, height = 0, three = 0, color = 0;
    char p = '0';
    char imagename[30] = { 0 };
    FILE* infile;

    while (1) {

        print_init();
        printf("메뉴 선택 > ");
        scanf("%d", &menu);

        if (menu == 1) {
            printf("이미지 이름 : ");
            scanf("%s", imagename);
            infile = fopen(imagename, "r");
            if (infile == NULL) printf("유효하지 않은 파일입니다. 메인 메뉴로 돌아갑니다.\n");
            else {
                printf("이미지 읽기를 완료했습니다.\n");
                fscanf(infile, "%c%d %d %d %d", &p, &three, &width, &height, &color);
                array_allocation(&R, &G, &B, width, height);
                // rgb_array(R, G, B, width, height, infile);
                rgb_array_2(&R, &G, &B, width, height, infile);
                break;
            }

        }
        else if (menu == 4) break;
        else printf("유효하지 않은 입력입니다. 다시 입력해주세요\n");
    }
    while (1) {
        if (menu == 4) break;
        print_menu(imagename);
        printf("메뉴 선택 > ");
        scanf("%d", &menu);
        if (menu == 1) {
            free(R);
            free(G);
            free(B);
            printf("이미지 이름 : ");
            scanf("%s", imagename);
            infile = fopen(imagename, "r");
            if (infile == NULL) printf("유효하지 않은 파일입니다. 메인 메뉴로 돌아갑니다.\n");
            else printf("이미지 변경을 완료했습니다.\n");
        }
        else if (menu == 2) continue;
        else if (menu == 3) continue;
        else if (menu == 4) break;
        else printf("유효하지 않은 입력입니다. 다시 입력해주세요\n");
    }
    printf("사진 정합 프로그램 종료\n");
    free(R);
    free(G);
    free(B);
    return 0;
}

void print_init() {
    printf("===================\n");
    printf("[1] 이미지 불러오기\n");
    printf("[2] 이미지 정합(SSD)\n");
    printf("[3] 이미지 정합(NCC)\n");
    printf("[4] 종료\n");
    printf("===================\n");
}

void print_menu(char imagename[30]) {
    printf("===================\n");
    printf("[1] 이미지 변경 - 현재 이미지: %s\n", imagename);
    printf("[2] 이미지 정합(SSD)\n");
    printf("[3] 이미지 정합(NCC)\n");
    printf("[4] 종료\n");
    printf("===================\n");
}

void array_allocation(int ***R, int ***G, int ***B, int width, int height) {

    int i;
    *R = (int**)malloc(sizeof(int*) * height);
    *G = (int**)malloc(sizeof(int*) * height);
    *B = (int**)malloc(sizeof(int*) * height);

    for (i = 0; i < height; i++) {
        (*R)[i] = (int*)malloc(sizeof(int) * width);
        (*G)[i] = (int*)malloc(sizeof(int) * width);
        (*B)[i] = (int*)malloc(sizeof(int) * width);
    }

}

void rgb_array(int **R, int **G, int **B, int width, int height, FILE* infile) {

    int row = 0, col = 0;

    for(row = 0; row < height; row++){
        for(col = 0; col < width; col++){
            fscanf(infile, "%d", &R[row][col]);
            fscanf(infile, "%d", &G[row][col]);
            fscanf(infile, "%d", &B[row][col]);
        }
    }
}

void rgb_array_2(int ***R, int ***G, int ***B, int width, int height, FILE* infile) {

    int row = 0, col = 0;

    for(row = 0; row < height; row++){
        for(col = 0; col < width; col++){
            fscanf(infile, "%d", &(*R)[row][col]);
            fscanf(infile, "%d", &(*G)[row][col]);
            fscanf(infile, "%d", &(*B)[row][col]);
        }
    }
}