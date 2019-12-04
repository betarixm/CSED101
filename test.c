#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LEN_FILENAME 31
#define MAX_DW 15
#define MAX_DH 15

#define RED 0
#define GREEN 1
#define BLUE 2

typedef struct {
    int w; // x 좌표. width에 대응.
    int h; // y 좌표. height에 대응.
} POINT; // 좌표 1개의 정보

typedef struct {
    POINT d; // delta. 채널이 움직여야 하는 양.
    POINT ref; // reference 좌표. 채널의 기준 시작점.
    int isOptimal; // 현재 평가값이 최적 값인지를 나타내는 bool.
    double value; // 현재 평가값
    double optimal_value; // 최적 평가값
    long long sum; // 단순 합계
    long long sum_cross; // 기준 채널과의 크로스 값 합계.
    long long sum_target_square; // 이 채널의 제곱 값 합계.
} CHANNEL; // 채널 1개의 정보

typedef struct {
    CHANNEL ch[3]; // 채널 3개에 대한 정보
    int ***pixel; // 채널 픽셀 정보
    int width; // 가로 길이
    int height; // 세로 길이
} IMG; // 이미지 정보

void SSD_NCC(IMG *img, char fileName[], int targets[], int isSSD);

void calc(IMG *img, int src, int target, int isSSD);

int loadImage(FILE **inputImage, char *fileName);

int readImage(FILE **inputImage, IMG *img);

void writeImage(IMG *img, char *resultFileName, const int *targets);

void result(IMG *img, char fileName[], char resultFileName[], int targets[], int isSSD);

void menu(int *mode, const char fileName[]);

void printMenu(const char *fileName);

void printLine();

int free_pixel(IMG* img);

int swap(int *a, int *b);

int main() {
    IMG img;
    FILE *inputImgFile;
    int mode = 0;
    char fileName[LEN_FILENAME] = "";
    int targets[3] = {RED, GREEN, BLUE}; // targets[2]를 기준 채널으로 삼는다.
    img.pixel = NULL;

    do {
        menu(&mode, fileName); // 사용자로부터 입력을 받는다.
        if (mode == 1) {
            loadImage(&inputImgFile, fileName) && readImage(&inputImgFile, &img); // image를 불러오는데 성공하면 readImage 한다.
        } else if (mode == 2 || mode == 3) {
            SSD_NCC(&img, fileName, targets, 3 - mode); // SSD / NCC 호출
        } else if (mode == 4) {
            free_pixel(&img); // 종료 시 free.
        }
    } while (mode != 4);

    return 0;
}

void SSD_NCC(IMG *img, char fileName[], int targets[], int isSSD) {
    char resultFileName[100] = "";
    printf("\n");
    printLine();
    calc(img, targets[2], targets[0], isSSD); // targets[2]를 기준으로 targets[0]에 대한 최적값 계산
    calc(img, targets[2], targets[1], isSSD); // targets[2]를 기준으로 targets[1]에 대한 최적값 계슨
    result(img, fileName, resultFileName, targets, isSSD); // result 출력.
    printLine();
    printf("\n");
}

void calc(IMG *img, int src, int target, int isSSD) {
    int pixel_src = 0, pixel_target = 0, height = 0, width = 0, h = 0, w = 0;
    long long sum_src_square = 0;
    POINT src_ref, target_ref, d; // 기준, 타겟의 기준점과 delta iterator 설정.
    CHANNEL* ch = &(img->ch[target]); // 타겟의 채널 정보

    img->ch[src].d = (POINT){0,0};
    img->ch[src].ref = (POINT){0, 0};

    for (d.h = -MAX_DH; d.h <= MAX_DH; d.h++) { // delta w가 -15 ~ 15
        for (d.w = -MAX_DW; d.w <= MAX_DW; d.w++) { // delta h가 -15 ~ 15
            src_ref = (POINT) {d.w * (d.w > 0), d.h * (d.h < 0) * (-1)}; // delta에 따른 기준 채널 기준점 세팅
            target_ref = (POINT) {d.w * (d.w < 0) * (-1), d.h * (d.h > 0)}; // delta에 따른 타겟 채널 기준점 세팅

            height = (img->height) - (abs(d.h)); // delta에 따른 가로 길이
            width = (img->width) - (abs(d.w)); // delta에 따른 세로 길이

            // 합계 초기화
            ch->sum = 0;
            ch->sum_cross = 0;
            ch->sum_target_square = 0;
            sum_src_square = 0;

            for (h = 0; h < height; h++) { // 세로 좌표 안에서
                for (w = 0; w < width; w++) { // 가로 좌표 안에서
                    pixel_src = img->pixel[src][src_ref.h + h][src_ref.w + w]; // 기준 채널 픽셀 값
                    pixel_target = img->pixel[target][target_ref.h + h][target_ref.w + w]; // 타겟 채널 픽셀 값
                    if (isSSD) { // SSD 모드라면
                        ch->sum += (long long) pow(pixel_src - pixel_target, 2); // 단순 합계 구하기
                    } else { // NCC 모드라면
                        ch->sum_cross += (pixel_src * pixel_target); // 기준 * 타겟 크로스 합계
                        sum_src_square += (long long) pow(pixel_src, 2); // 기준 채널 제곱 값 합계
                        ch->sum_target_square += (long long) pow(pixel_target, 2); // 타겟 채널 제곱 값 합계
                    }
                }
            }

            ch->value = (isSSD) ?
                        ((double) ch->sum / (width * height)) : // SSD일 때 평가값
                        ((double) ch->sum_cross /(sqrt((double)sum_src_square) *sqrt((double)ch->sum_target_square))); // NCC일 때 평가값
            ch->optimal_value = (d.w == -MAX_DW && d.h == -MAX_DH) ? (ch->value) : (ch->optimal_value); // 맨 처음에는 평가값이 바로 최적값이 된다
            ch->isOptimal = (isSSD) ? (ch->value < ch->optimal_value) : (ch->value > ch->optimal_value); // SSD / NCC에 따른 optimal bool.

            if (ch->isOptimal) { // optimal한 상황이라면
                ch->optimal_value = ch->value; // optimal_value 업데이트
                ch->d = d; // 타겟 채널 delta 값 업데이트
                ch->ref = target_ref; // 타겟 채널 reference 좌표 업데이트
            }
        }
    }
}

int loadImage(FILE **inputImage, char *fileName) {
    printf("이미지 이름: ");
    scanf("%s", fileName);
    *inputImage = fopen(fileName, "r"); // 입력 문자열으로 파일 열기.
    if (*inputImage == NULL)  { // 만약 파일을 불러오는데 실패했다면
        printf("이미지를 불러오는데 실패했습니다.\n\n");
        strcpy(fileName, "");
        return 0;
    }
    return 1;
}

int readImage(FILE **inputImage, IMG *img) {
    int i = 0, j = 0, k = 0;
    free_pixel(img); // 우선 pixel을 free한다.
    fscanf(*inputImage, "%*c%*c %d %d %*d", &(img->width), &(img->height)); // width, height 입력

    for (i = 0; i < 3; i++){ // 동적할당; 채널 3개
        img->pixel = (int***) malloc(3 * sizeof(int**));
    }

    for (i = 0; i < 3; i++) { // 동적할당; height
        img->pixel[i] = (int **) malloc((img->height) * sizeof(int *));
        for (j = 0; j < (img->height); j++) { // 동적할당; width
            img->pixel[i][j] = (int *) malloc((img->width) * sizeof(int));
        }
    }

    for (i = 0; i < (img->height); i++) {
        for (j = 0; j < (img->width); j++) {
            for (k = 0; k < 3; k++) {
                fscanf(*inputImage, "%d", &(img->pixel[k][i][j])); // 픽셀값 R,G,B 순서대로 입력.
            }
        }
    }

    fclose(*inputImage); // 파일 스트림 close.
    printf("이미지 읽기를 완료했습니다.\n\n");
    return 0;
}

void writeImage(IMG *img, char *resultFileName, const int *targets) {
    FILE *resultFile; // resultFile 파일포인터.
    POINT calibration; // delta 보정 상수.
    CHANNEL* ch = (img->ch); // 편의를 위해 img->ch 따로 지정.
    int i = 0, j = 0, size = 3, w = 0, h = 0, width = 0, height = 0;
    int w_pos[3] = {targets[0], targets[1], targets[2]};
    int h_pos[3] = {targets[0], targets[1], targets[2]};

    for (i = 1; i < size; i++) {
        for (j = 0; j < size - i; j++) {
            (ch[w_pos[j]].d.w > ch[w_pos[j + 1]].d.w) && swap(&w_pos[j], &w_pos[j + 1]); // 각 타겟을 delta w를 기준으로 오름차순 정렬
            (ch[h_pos[j]].d.h > ch[h_pos[j + 1]].d.h) && swap(&h_pos[j], &h_pos[j + 1]); // 각 타겟을 delta h를 기준으로 오름차순 정렬
        }
    }

    // w_pos[0] = 왼쪽에 있는 타겟. w_pos[1] = 가운데에 있는 타겟. w_pos[2] = 오른쪽에 있는 타겟.
    // h_pos[0] = 아래쪽에 있는 타겟. h_pos[1] = 가운데에 있는 타겟. h_pos[2] = 위쪽에 있는 타겟.

    calibration = (POINT) {ch[w_pos[0]].d.w * -1, ch[h_pos[0]].d.h * -1}; // 가장 작은 delta 값의 -1 곱한 값.

    for (i = 0; i < 3; i++) { // 보정 값을 더해서 모든 delta 값을 0 이상의 수로 만든다.
        ch[w_pos[i]].d.w += calibration.w;
        ch[h_pos[i]].d.h += calibration.h;
    }

    for (i = 0; i < 3; i++) { // 이와 같은 상황에서, 각 채널의 기준 시작점은 아래와 같이 세팅하 수 있다.
        ch[w_pos[i]].ref.w = ch[w_pos[2]].d.w - ch[w_pos[i]].d.w;
        ch[h_pos[i]].ref.h = ch[h_pos[i]].d.h;
    }

    width = img->width - ch[w_pos[2]].d.w; // 출력 이미지 가로 길이
    height = img->height - ch[h_pos[2]].d.h; // 출력 이미지 세로 길이

    resultFile = fopen(resultFileName, "w"); // 출력 이미지 파일 오픈

    fprintf(resultFile, "P3 %d %d 255\n", width, height); // 파일 시그니처 작성

    for (h = 0; h < height; h++) {
        for (w = 0; w < width; w++) {
            for (i = 0; i < 3; i++) {
                fprintf(resultFile, "%d ", img->pixel[i][ch[i].ref.h + h][ch[i].ref.w + w]); // RGB 순서대로 파일 작성
            }
        }
    }

    fclose(resultFile); // 파일 포인터 닫기
}

void result(IMG *img, char fileName[], char resultFileName[], int targets[], int isSSD) {
    char mode[4] = "NCC";
    CHANNEL* ch = &(*img->ch);
    isSSD && strcpy(mode, "SSD"); // SSD 모드라면 NCC로 mode 문자열 변경
    printf("%s - R:[%d, %d] G:[%d, %d]\n", mode, ch[RED].d.w, ch[RED].d.h, ch[GREEN].d.w,
           ch[GREEN].d.h);
    sprintf(resultFileName, "%s_%s_R%d_%d_G%d_%d.ppm", strtok(fileName, "."), mode, ch[RED].d.w,
            ch[RED].d.h,
            ch[GREEN].d.w, ch[GREEN].d.h); // sprintf로 파일 이름 생성
    strcat(fileName, ".ppm"); // strtok 하면서 잘린 .ppm을 다시 붙여준다.
    writeImage(img, resultFileName, targets); // 이미지 파일 생성
    printf("결과 이미지 파일: %s\n", resultFileName);
}

void menu(int *mode, const char fileName[]) {
    while (1) {
        printMenu(fileName); // 메뉴 출력
        scanf("%d", mode);

        if (!(*mode == 1 || *mode == 2 || *mode == 3 || *mode == 4)) { // 1, 2, 3, 4 중 하나가 아니면
            printf("1, 2, 3, 4 중에서 입력해주세요.\n");
            getchar();
            continue;
        }

        if ((*mode == 2 || *mode == 3) && (strcmp(fileName, "") == 0)) { // 2 , 3 인데 fileName이 비어있다면
            printf("이미지를 불러온 후에 이미지를 정합할 수 있습니다.\n");
            continue;
        }

        break; // 정상적인 입력일 때 종료.
    }
}

void printMenu(const char *fileName) {
    printLine();
    if (strcmp(fileName, "") == 0) {
        printf("[1] 이미지 불러오기\n");
    } else {
        printf("[1] 이미지 변경 - 현재 이미지: %s\n", fileName);
    }
    printf("[2] 이미지 정합(SSD)\n[3] 이미지 정합(NCC)\n[4] 종료\n");
    printLine();
    printf("메뉴 선택> ");
}

void printLine() {
    printf("===================\n");
}

int free_pixel(IMG* img){
    int i = 0, j = 0;
    if (img->pixel == NULL){
        return 0;
    }
    for (i = 0; i < 3; i++){
        for(j = 0; j < img->height; j++){ // width 크기 free
            free(img->pixel[i][j]);
        }
        free(img->pixel[i]); // height 크기 free
    }
    free(img->pixel); // 채널 3개 크기 free
    return 1;
}

int swap(int *a, int *b) { // int 값 2개 스왑.
    int tmp = *a;
    *a = *b;
    *b = tmp;
    return 1;
}