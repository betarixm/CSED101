#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define LEN_FILENAME 30
#define MAX_DW 15
#define MAX_DH 15

#define RED 0
#define GREEN 1
#define BLUE 2

typedef struct {
    int w;
    int h;
} POINT;

typedef struct {
    POINT d;
    POINT ref;
} OPTIMAL;

typedef struct {
    int width;
    int height;
    int **channels[3];
    OPTIMAL opt[3];
} IMG;

typedef struct {
    int pixel;
    int isOptimal;
    double value;
    double optimal_value;
    long long sum;
    long long sum_cross;
    long long sum_target_square;
    long long sum_src_square;
} CHANNEL;

void setOptimal(IMG *img, int target, int dW, int dH, int w, int h);

void menu(int *mode, const char fileName[]);

int loadImage(FILE **inputImage, char *fileName);

int readImage(FILE **inputImage, IMG *img);

void printLine();

void printMenu(const char *fileName);

void calc(IMG *img, int src, int target, int isSSD);

void SSD_NCC(IMG *img, char fileName[], int targets[], int isSSD);

void result(IMG *img, char fileName[], char resultFileName[], int targets[], int isSSD);

void resultIMG(IMG *img, char resultFileName[], const int targets[]);

int max(int a, int b);

int min(int a, int b);

void swap(int *a, int *b);

int main() {
    IMG img;
    FILE *inputImgFile;
    int mode = 0, i = 0;
    char fileName[LEN_FILENAME] = "";
    int targets[3] = {RED, GREEN, BLUE}; // targets[2]를 기준 채널으로 삼는다.

    for (i = 0; i < 3; i++) {
        img.channels[i] = (int **) malloc(sizeof(int *));
    }

    do {
        menu(&mode, fileName);
        if (mode == 1) {
            loadImage(&inputImgFile, fileName) && readImage(&inputImgFile, &img);
        } else if (mode == 2 || mode == 3) {
            SSD_NCC(&img, fileName, targets, 3 - mode);
        } else if (mode == 4) {
            for (i = 0; i < 3; i++) {
                free(img.channels[i]);
            }
        }
    } while (mode != 4);
}

int max(int a, int b) {
    return (a > b) ? (a) : (b);
}

int min(int a, int b) {
    return (a < b) ? (a) : (b);
}

void setOptimal(IMG *img, int target, int dW, int dH, int w, int h) {
    img->opt[target].d.w = dW;
    img->opt[target].d.h = dH;
    img->opt[target].ref.w = w;
    img->opt[target].ref.h = h;
}

void menu(int *mode, const char fileName[]) {
    while (1) {
        printMenu(fileName);
        scanf("%d", mode);

        if (!(*mode == 1 || *mode == 2 || *mode == 3 || *mode == 4)) {
            printf("1, 2, 3, 4 중에서 입력해주세요.\n");
            getchar();
            continue;
        }

        if ((*mode == 2 || *mode == 3) && (strcmp(fileName, "") == 0)) {
            printf("이미지를 불러온 후에 이미지를 정합할 수 있습니다.\n");
            continue;
        }
        break;
    }
}

int loadImage(FILE **inputImage, char *fileName) {
    printf("이미지 이름: ");
    scanf("%s", fileName);
    *inputImage = fopen(fileName, "r");
    if (*inputImage != NULL) {
        return 1;
    } else {
        printf("이미지를 불러오는데 실패했습니다.\n\n");
        strcpy(fileName, "");
        return 0;
    }
}

int readImage(FILE **inputImage, IMG *img) {
    int i = 0, j = 0, k = 0;
    for (i = 0; i < 3; i++) {
        free(img->channels[i]);
    }

    fscanf(*inputImage, "%*c%*c %d %d %*d", &(img->width), &(img->height));
    for (i = 0; i < 3; i++) {
        img->channels[i] = (int **) malloc((img->height) * sizeof(int *));
        for (j = 0; j < (img->height); j++) {
            img->channels[i][j] = (int *) malloc((img->width) * sizeof(int));
        }
    }

    for (i = 0; i < (img->height); i++) {
        for (j = 0; j < (img->width); j++) {
            for (k = 0; k < 3; k++) {
                fscanf(*inputImage, "%d", &(img->channels[k][i][j]));
            }
        }
    }

    fclose(*inputImage);

    printf("이미지 읽기를 완료했습니다.\n\n");
    return 0;
}

void printLine() {
    printf("===================\n");
}

void printMenu(const char *fileName) {
    printLine();
    if (strcmp(fileName, "") == 0) {
        printf("[1] 이미지 불러오기\n");
    } else {
        printf("[1] 이미지 변경 - 현재 이미지: %s\n", fileName);
    }

    printf("[2] 이미지 정합(SSD)\n");
    printf("[3] 이미지 정합(NCC)\n");
    printf("[4] 종료\n");
    printLine();
    printf("메뉴 선택> ");
}

void calc(IMG *img, int src, int target, int isSSD) {
    POINT ref_src, ref_target, d;
    CHANNEL ch;
    int pixel_src = 0, pixel_target = 0, height = 0, width = 0, h = 0, w = 0;

    for (d.w = -MAX_DW; d.w <= MAX_DW; d.w++) {
        for (d.h = -MAX_DH; d.h <= MAX_DH; d.h++) {
            ref_src = (POINT){d.w * (d.w > 0), d.h * (d.h < 0) * (-1)};
            ref_target = (POINT){d.w * (d.w < 0) * (-1), d.h * (d.h > 0)};

            height = (img->height) - (abs(d.h));
            width = (img->width) - (abs(d.w));

            ch.sum = 0;
            ch.sum_cross = 0;
            ch.sum_target_square = 0;
            ch.sum_src_square = 0;

            for (h = 0; h < height; h++) {
                for (w = 0; w < width; w++) {
                    pixel_src = img->channels[src][ref_src.h + h][ref_src.w + w];
                    pixel_target = img->channels[target][ref_target.h + h][ref_target.w + w];
                    if (isSSD) {
                        ch.sum += (long long) pow(pixel_src - pixel_target, 2);
                    } else {
                        ch.sum_cross += (pixel_src * pixel_target);
                        ch.sum_src_square += (long long) pow(pixel_src, 2);
                        ch.sum_target_square += (long long) pow(pixel_target, 2);
                    }
                }
            }

            ch.value = (isSSD) ? ((double) ch.sum / (img->width * img->height)) : ((double) ch.sum_cross /
                                                                                   (sqrt(ch.sum_src_square) *
                                                                                    sqrt(ch.sum_target_square)));
            ch.optimal_value = (d.w == -MAX_DW && d.h == -MAX_DH) ? (ch.value) : (ch.optimal_value);
            ch.isOptimal = (isSSD) ? (ch.value < ch.optimal_value) : (ch.value > ch.optimal_value);
            // setOptimal(img, src, 0, 0, 0, 0);
            img->opt[src] = (OPTIMAL){(POINT){0, 0}, (POINT){0, 0}};
            if (ch.isOptimal) {
                ch.optimal_value = ch.value;
                img->opt[target] = (OPTIMAL){d, ref_target};
            }

        }
    }
}

void SSD_NCC(IMG *img, char fileName[], int targets[], int isSSD) {
    clock_t t;
    t = clock();
    char resultFileName[100] = "";
    printf("\n");
    printLine();
    calc(img, targets[2], targets[0], isSSD);
    calc(img, targets[2], targets[1], isSSD);
    result(img, fileName, resultFileName, targets, isSSD);
    printLine();
    t = clock() - t;
    printf("Elapsed Time: %.2f\n", ((double) t) / CLOCKS_PER_SEC);
    printf("\n");
}


void result(IMG *img, char fileName[], char resultFileName[], int targets[], int isSSD) {
    char mode[4] = "NCC";
    isSSD && strcpy(mode, "SSD");

    printf("%s - R:[%d, %d] G:[%d, %d]\n", mode, img->opt[RED].d.w, img->opt[RED].d.h, img->opt[GREEN].d.w,
           img->opt[GREEN].d.h);
    sprintf(resultFileName, "%s_%s_R%d_%d_G%d_%d.ppm", strtok(fileName, "."), mode, img->opt[RED].d.w,
            img->opt[RED].d.h,
            img->opt[GREEN].d.w, img->opt[GREEN].d.h);
    strcat(fileName, ".ppm");
    resultIMG(img, resultFileName, targets);
    printf("결과 이미지 파일: %s\n", resultFileName);
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}


void resultIMG(IMG *img, char resultFileName[], const int targets[]) {
    FILE *resultFile;
    POINT calibration;
    int i = 0, j = 0, size = 3, w = 0, h = 0, width = 0, height = 0;
    int w_pos[3] = {targets[0], targets[1], targets[2]};
    int h_pos[3] = {targets[0], targets[1], targets[2]};

    for (i = 1; i < size; i++) {
        for (j = 0; j < size - i; j++) {
            if (img->opt[w_pos[j]].d.w > img->opt[w_pos[j + 1]].d.w) {
                swap(&w_pos[j], &w_pos[j + 1]);
            }
            if (img->opt[h_pos[j]].d.h > img->opt[h_pos[j + 1]].d.h) {
                swap(&h_pos[j], &h_pos[j + 1]);
            }
        }
    }

    calibration.w = img->opt[w_pos[0]].d.w * -1;
    calibration.h = img->opt[h_pos[0]].d.h * -1;

    for (i = 0; i < 3; i++) {
        img->opt[w_pos[i]].d.w += calibration.w;
        img->opt[h_pos[i]].d.h += calibration.h;
    }

    for (i = 0; i < 3; i++) {
        img->opt[w_pos[i]].ref.w = img->opt[w_pos[2]].d.w - img->opt[w_pos[i]].d.w;
        img->opt[h_pos[i]].ref.h = img->opt[h_pos[i]].d.h;
    }

    width = img->width - img->opt[w_pos[2]].d.w;
    height = img->height - img->opt[h_pos[2]].d.h;

    resultFile = fopen(resultFileName, "w");

    fprintf(resultFile, "P3 %d %d 255\n", width, height);

    for (h = 0; h < height; h++) {
        for (w = 0; w < width; w++) {
            for (i = 0; i < 3; i++) {
                fprintf(resultFile, "%d ", img->channels[i][img->opt[i].ref.h + h][img->opt[i].ref.w + w]);
            }
        }
    }

    fclose(resultFile);
}