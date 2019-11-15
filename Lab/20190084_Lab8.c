#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

void print_array(int *arr, int size);
void input_array(int *arr, int size);
void Find_minmax(int *arr, int size, int *max, int *min);

int main(){
    int *arr;
    int max = 0, min = 0, size = 0, i = 0;

    printf("Enter array size: ");
    scanf("%d", &size);

    arr = (int *)malloc(size * sizeof(int));

    input_array(arr, size);
    print_array(arr, size);
    Find_minmax(arr, size, &max, &min);

    printf("Maximum is %d\n", max);
    printf("Minimum is %d\n", min);

    printf("\nEnter changed array size: ");
    scanf("%d", &size);

    arr = (int *)realloc(arr, size * sizeof(int));

    input_array(arr, size);
    print_array(arr, size);
    Find_minmax(arr, size, &max, &min);

    printf("Maximum is %d\n", max);
    printf("Minimum is %d\n", min);

    free(arr);

    return 0;

}

void Find_minmax(int *arr, int size, int *max, int *min){
    int i = 0;

    *max = arr[0];
    *min = arr[0];

    for(i = 0; i < size; i++){
        *max = (*max < arr[i])?(arr[i]):(*max);
        *min = (*min > arr[i])?(arr[i]):(*min);
    }
}

void print_array(int *arr, int size){
    int i = 0;
    printf("Entered numbers are\n");
    for(i = 0; i < size; i++){
        printf("%d  ", arr[i]);
    }
    printf("\n");
}

void input_array(int *arr, int size){
    int i = 0;
    for(i = 0; i < size; i++){
        scanf("%d", &arr[i]);
    }
}