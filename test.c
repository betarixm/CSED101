#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

float ncc(int** color1, int** color2, int x, int y, int width, int height);
float ssd(int** color1, int** color2, int x, int y, int width, int height);
void find_min(int** color1, int** color2, int* min_x, int* min_y, int width, int height, int mode);
void find_Range(int Red_x, int Red_y, int Green_x, int Green_y,int height, int width, int *top, int *bottom, int *right, int *left);


int main()
{
    FILE* file=NULL, * result;
    int mode, width, height, p, i, j, Red_x, Red_y, Green_x, Green_y, min_x=-15, min_y=-15, top=0, bottom=0, right=0, left=0;// p는 픽셀을 의미한다
    float min=0;
    char imagename[30], type[10], finalfile[30];
    int** R, ** G, **B;//Red의 2차원 배열

    while (1)
    {
        printf("===================\n[1] 이미지 불러오기\n[2] 이미지 정합(SSD)\n[3] 이미지 정합(NCC)\n[4] 종료\n===================\n메뉴 선택> ");
        scanf("%d", &mode);
        if (mode > 5)
        {
            printf("Wrong!!! Select again\n");
        }
        else if (mode != 1)
        {
            printf("Wrong!!! Select again\n");
        }
        else if (mode == 4)//quit선택하면 끝낸다
        {
            return 0;
        }
        if (mode == 1)
        {

            while (file == NULL)//파일이 존재하지 않을 경우 다시 입력하라고 한다.
            {
                printf("이미지 이름: ");
                scanf("%s", imagename);//error뜨면 null문자 여부 확인
                file = fopen(imagename, "r");
                if (file == NULL)
                {
                    printf("There isn't any file that has name %s\n", imagename);//그런 파일이 존재하지 않다고 출력한다 만약 파일이 열렸다면 file == NULL이 아니므로 while문이 끝난다
                }
            }

            fscanf(file, "%s", type);
            fscanf(file, "%d%d%d", &width, &height, &p);
            R = (int**)malloc(sizeof(int*) * height);
            B = (int**)malloc(sizeof(int*) * height);
            G = (int**)malloc(sizeof(int*) * height);
            for (i = 0; i < height; i++)//동적할당하기
            {
                *(R + i) = (int*)malloc(sizeof(int) * width);
                *(G + i) = (int*)malloc(sizeof(int) * width);
                *(B + i) = (int*)malloc(sizeof(int) * width);
            }
            for (i = 0; i < height; i++)
            {
                for (j = 0; j < width; j++)//앞의 4자리 제외하고 RGB의 픽셀값들을 할당하는 2중 for문 i가
                {
                    fscanf(file, "%d%d%d", &R[i][j], &G[i][j], &B[i][j]);
                }
            }
            printf("이미지 읽기를 완료했습니다.\n");//1선택 후 픽셀값을 동적할당한 뒤 메뉴선택창을 띄운다
            while (1)
            {

                printf("=================== \n[1] 이미지 변경 - 현재 이미지 : %s\n[2] 이미지 정합(SSD)\n[3] 이미지 정합(NCC)\n[4] 종료\n=================== \n메뉴 선택 > ", imagename);
                scanf("%d", &mode);
                if (mode == 2)
                {

                    find_min(B, R, &min_x, &min_y, width, height, mode);
                    Red_x = min_x;
                    Red_y = min_y;

                    find_min(B, G, &min_x, &min_y, width, height, mode);
                    Green_x = min_x;
                    Green_y = min_y;

                    find_Range(Red_x, Red_y, Green_x, Green_y, height, width, &top, &bottom, &right, &left);
                    printf("\n\n===================\nSSD - R : [%d, %d] G : [%d, %d]\n결과 이미지 파일: %c%c%c%c%c_SSD_R%d_%d_G_%d_%d.ppm\n===================", Red_x, Red_y, Green_x, Green_y, imagename[0], imagename[1], imagename[2], imagename[3], imagename[4], Red_x, Red_y, Green_x, Green_y);
                    sprintf(finalfile, "%c%c%c%c%c_SSD_R%d_%d_G_%d_%d.ppm", imagename[0], imagename[1], imagename[2], imagename[3], imagename[4], Red_x, Red_y, Green_x, Green_y);
                    result = fopen(finalfile, "w");
                    fprintf(result, "P3 %d %d 255", width, height);//P3 width height 255를 입력한다 width와 height는 변화된 값
                    for (i = top; i < bottom; i++)
                    {
                        for (j = left; j < right; j++)
                        {
                            fprintf(result, "%d%d%d", R[i + Red_y][j - Red_x], G[i + Green_y][j - Green_x], B[i][j]);
                        }
                    }
                    fclose(result);
                }
                else if (mode == 3)
                {
                    find_min(B, R, &min_x, &min_y, width, height, mode);
                    Red_x = min_x;
                    Red_y = min_y;

                    find_min(B, G, &min_x, &min_y, width, height, mode);
                    Green_x = min_x;
                    Green_y = min_y;

                    find_Range(Red_x, Red_y, Green_x, Green_y, height, width, &top, &bottom, &right, &left);
                    printf("\n\n===================\nSSD - R : [%d, %d] G : [%d, %d]\n결과 이미지 파일: %c%c%c%c%c_NCC_R%d_%d_G_%d_%d.ppm\n===================", Red_x, Red_y, Green_x, Green_y, imagename[0], imagename[1], imagename[2], imagename[3], imagename[4], Red_x, Red_y, Green_x, Green_y);
                    sprintf(finalfile, "%c%c%c%c%c_NCC_R%d_%d_G_%d_%d.ppm", imagename[0], imagename[1], imagename[2], imagename[3], imagename[4], Red_x, Red_y, Green_x, Green_y);
                    result = fopen(finalfile, "w");
                    fprintf(result, "P3 %d %d 255", width, height);//P3 width height 255를 입력한다 width와 height는 변화된 값
                    for (i = top; i < bottom; i++)
                    {
                        for (j = left; j < right; j++)
                        {
                            fprintf(result, "%d%d%d", R[i + Red_y][j - Red_x], G[i + Green_y][j - Green_x], B[i][j]);
                        }
                    }
                    fclose(result);

                }
                else if (mode == 4)
                {
                    free(R);
                    free(B);
                    free(G);
                    return 0;
                }
            }
        }
    }
}

float ssd(int** color1, int** color2, int x, int y, int width, int height)//color1는 항상 blue로 고정한다고 생각한다 즉 x y좌표를 blue에서 생각한다
{
    int i, j, sum = 0;

    if (x >= 0)
    {
        if (y >= 0)
        {

            for (i = 0; i < height - y; i++)//x y가 둘다 양인경우의 ssd값 계산
            {
                for (j = 0; j < width - x; j++)
                {
                    sum += (color1[i][j + x] - color2[i + y][j]) * (color1[i][j + x] - color2[i + y][j]);
                }
            }
        }
        else //즉 y가 음수인 경우
        {
            for (i = 0; i < height + y; i++)//x y가 둘다 양인경우의 ssd값 계산
            {
                for (j = 0; j < width - x; j++)
                {
                    sum += (color1[i - y][j + x] - color2[i][j]) * (color1[i - y][j + x] - color2[i][j]);
                }
            }
        }
    }
    else // x가 음수인 경우
    {
        if (y >= 0)
        {

            for (i = 0; i < height - y; i++)//y가 양인경우
            {
                for (j = 0; j < width + x; j++)
                {
                    sum += (color1[i][j] - color2[i + y][j-x]) * (color1[i][j] - color2[i + y][j - x]);
                }
            }
        }
        else //즉 y가 음수인 경우
        {
            for (i = 0; i < height + y; i++)//x y가 둘다 양인경우의 ssd값 계산
            {
                for (j = 0; j < width + x; j++)
                {
                    sum += (color1[i - y][j] - color2[i][j-x]) * (color1[i - y][j] - color2[i][j - x]);
                }
            }

        }
    }
    return (float)(sum / ((width-x) *(height - y)));
}

float ncc(int** color1, int** color2, int x, int y, int width, int height)//color1을 blue color2를 Red Green으로 생각한다
{
    int i, j, sum1 = 0, sum2 = 0, sum3 = 0;

    if (x >= 0)
    {
        if (y >= 0)
        {

            for (i = 0; i < height - y; i++)//x y가 둘다 양인경우의 ncc값 계산
            {
                for (j = 0; j < width - x; j++)
                {
                    sum1 += color1[i][j+x] * color2[i+y][j];
                    sum2 += color1[i][j+x] * color1[i][j+x];
                    sum3 += color2[i+y][j] * color2[i+y][j];
                }
            }
        }
        else //즉 y가 음수인 경우
        {
            for (i = 0; i < height + y; i++)//x y가 둘다 양인경우의 ssd값 계산
            {
                for (j = 0; j < width - x; j++)
                {
                    sum1 += color1[i-y][j+x] * color2[i][j];
                    sum2 += color1[i-y][j+x] * color1[i-y][j+x];
                    sum3 += color2[i][j] * color2[i][j];
                }
            }
        }
    }
    else // x가 음수인 경우
    {
        if (y >= 0)
        {

            for (i = 0; i < height - y; i++)//y가 양인경우
            {
                for (j = 0; j < width + x; j++)
                {
                    sum1 += color1[i][j] * color2[i+y][j-x];
                    sum2 += color1[i][j] * color1[i][j];
                    sum3 += color2[i+y][j-x] * color2[i+y][j-x];
                }
            }
        }
        else //즉 y가 음수인 경우
        {
            for (i = 0; i < height + y; i++)//x y가 둘다 양인경우의 ssd값 계산
            {
                for (j = 0; j < width + x; j++)
                {
                    sum1 += color1[i-y][j] * color2[i][j-x];
                    sum2 += color1[i-y][j] * color1[i-y][j];
                    sum3 += color2[i][j-x] * color2[i][j-x];
                }
            }
        }
    }
    return (float)sum1 / sqrt(sum2 * sum3);
}
void find_min(int** color1, int** color2, int* min_x, int* min_y, int width, int height, int mode)
{
    int i, j;
    float min;


    if (mode == 2)
    {
        min = ssd(color1, color2, -15, -15, width, height);
        for (i = -15; i <= 15; i++)
        {
            for (j = -15; j <= 15; j++)
            {
                printf("\nSSD :%f i :%d j: %d", ssd(color1, color2, i, j, width, height), i, j);
                if (min > ssd(color1, color2, i, j, width, height))
                {
                    min = ssd(color1, color2, i, j, width, height);
                    printf("\nMin :%f", min);
                    *min_x = j;
                    *min_y = i;
                    printf("%d %d", *min_x, *min_y);

                }
            }
        }
    }
    if (mode == 3)
    {
        min = ncc(color1, color2, -15, -15, width, height);
        for (i = -15; i <= 15; i++)
        {
            for (j = -15; j <= 15; j++)
            {
                if (min > ncc(color1, color2, i, j, width, height))
                {
                    min = ncc(color1, color2, i, j, width, height);
                    *min_x = j;
                    *min_y = i;
                }
            }
        }
    }

}
void find_Range(int Red_x, int Red_y, int Green_x, int Green_y,int height, int width, int *top, int *bottom, int *right, int *left)//ssd ncc가 최소가 되는 xy값을 통해 blue를 기준으로 범위를 정해주는 함수
{
    int a, b;
    a = Red_y;
    b = Green_y;

    if (a >= 0)
    {
        if (b >= 0)
        {
            if (a > b)
            {
                *top = 0;
                *bottom = height - a;
            }
            else
            {
                *top = 0;
                *bottom = height - b;
            }
        }
        else //b가 0 이하 a 양수인 경우
        {

            *top = -b;
            *bottom = height - a;

        }
    }
    else//a가 음수인 경우
    {
        if (b >= 0)
        {
            *top = -a;
            *bottom = height - b;
        }
        else//둘다 음수인 경우
        {
            if (a < b)
            {
                *top =  -a;
                *bottom = height + b;
            }
            else
            {
                *top = -b;
                *bottom = height + a;
            }
        }
    }//top bottom 계산 완료
    a = Red_x;
    b = Green_x;
    if (a >= 0)
    {
        if (b >= 0)
        {
            if (a > b)
            {
                *left = a;
                *right = 0;
            }
            else
            {
                *right = b;
                *left = 0;
            }
        }
        else //b가 0 이하, a 양수인 경우
        {

            *top = a;
            *left = width + b;
        }
    }
    else//a가 음수인 경우
    {
        if (b >= 0)
        {
            *right = b;
            *left = width + a;
        }
        else//둘다 음수인 경우
        {
            if (a < b)
            {
                *left = 0;
                *right = width + a;
            }
            else
            {
                *left = 0;
                *right = width + b;
            }
        }
    }
}