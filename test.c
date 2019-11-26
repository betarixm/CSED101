#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>

void print_init();
void scan_init(char imagename[20], int* menu);
void print_board(char imagename[20]);
void scan_board(char imagename[20], int* menu);
//void call_image(int* red, int* green, int* blue);

int main() {
    int s;
    int menu = 0;
    FILE* infile;
    char imagename[20];
    int** R;
    int i, check = 0, order = 0, height = 0, width = 0;
    print_init();
    scan_init(imagename, &menu);

    while (1){
        if (menu == 4) break;
        print_board(imagename);
        scan_board(imagename, &menu);
        if (menu == 4) break;
        infile = fopen("test1.ppm", "r");
        while (fscanf(infile, "%d", &check) != '\n') {
            ++order;
            if (order == 2) check = width;
            else if (order == 3) check = height;
        }
        //if (menu == 2) SSD(imagename);
        //if (menu == 3) NCC(imagename);
    }
    printf("사진 정합 프로그램을 종료합니다.\n");
    return 0;
}

void print_init() {
    printf("===================\n");
    printf("[1] 이미지 불러오기\n");
    printf("[2] 이미지 정합(SSD)\n");
    printf("[3] 이미지 정합(NCC)\n");
    printf("[4] 종료\n");
    printf("===================\n");
    return;
}

void scan_init(char imagename[20], int* menu) {
    int init = 0;
    while (init != 1 && init != 4) {
        printf("메뉴 선택> ");
        scanf("%d", &init);
        if (init != 1 && init != 4) printf("Invalid Input! Select Again\n");
    }
    if (init == 1) {
        printf("이미지 이름: ");
        scanf("%s", imagename);
        printf("이미지 읽기를 완료했습니니다.\n");
    }
    *menu = init;
    return;
}

void print_board(char imagename[20]) {
    printf("===================\n");
    printf("[1] 이미지 변경 - 현재 이미지: %s \n",imagename);
    printf("[2] 이미지 정합(SSD)\n");
    printf("[3] 이미지 정합(NCC)\n");
    printf("[4] 종료\n");
    printf("===================\n");
    return;
}

void scan_board(char imagename[20], int* menu) {
    int smenu = 0;
    while (1) {
        printf("메뉴 선택> ");
        scanf("%d", &smenu);
        if (smenu != 1 && smenu != 2 && smenu != 3 && smenu != 4) printf("Invalid Input! Select Again\n");
        else break;
    }
    if (smenu == 1) {
        printf("이미지 이름: ");
        scanf("%s", imagename);
        printf("이미지 변경을 완료했습니니다.\n");
    }
    *menu = smenu;
    return;
}

/*void allocation() {
	R = (int**)malloc(sizeof(int*) * height);
	for (i = 0; i < height; i++)
		*(R + i) = (int*)malloc(sizeof(int) * width);
}*/




