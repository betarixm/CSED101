#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define SIZE 20

void print_matrix(int arr[][SIZE], int size);

int main(){
    int arr[SIZE][SIZE] = {0};
    int size = 0, row = 0, col = 0;
    int isUpper = 0;
    int isLower = 0;
    printf("Enter the size of square: ");
    scanf("%d", &size);

    for(row = 0; row < size; row++){
        for(col = 0; col < size; col++){
            isUpper = (col < (size - (row + 1)));
            isLower = (col > (size - (row + 1)));
            arr[row][col] = arr[row][col] + isUpper - isLower;
        }
    }

    print_matrix(arr, size);
}

void print_matrix(int arr[][SIZE], int size){
    int row = 0, col = 0;

    for(row = 0; row < size; row++){
        for(col = 0; col < size; col++){
            printf("%3d", arr[row][col]);
        }
        printf("\n");
    }
}