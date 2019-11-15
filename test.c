// macOS, Clion에서 작성.

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define SIZE 20

void print_matrix(int** arr, int size);

int main(){
    int **arr;
    int size = 0, row = 0, col = 0;
    int isUpper = 0;
    int isLower = 0;
    printf("Enter the size of square: ");
    scanf("%d", &size);

    arr = (int **)malloc(size * sizeof(int *));

    if (arr == NULL){
        printf("ERROR DURING GET ADDR!\n");
    }

    for(row = 0; row < size; row++){
        arr[row] = (int*)calloc(size, sizeof(int));
        if(arr[row] == NULL){
            printf("ERROR DURING GET ADDR 2!\n");
        }
    }

    for(row = 0; row < size; row++){
        for(col = 0; col < size; col++){
            isUpper = (col < (size - (row + 1)));
            isLower = (col > (size - (row + 1)));
            arr[row][col] = arr[row][col] + isUpper - isLower;
        }
    }

    print_matrix(arr, size);
}

void print_matrix(int** arr, int size){
    int row = 0, col = 0;

    for(row = 0; row < size; row++){
        for(col = 0; col < size; col++){
            printf("%3d", arr[row][col]);
        }
        printf("\n");
    }
}