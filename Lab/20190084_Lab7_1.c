// macOS CLion에서 작성됨.

#include <stdio.h>
#define SIZE 10

void partial_print(const int *arr_start, int size);
int partial_sum(int *arr_start, int *arr_end);

int main(){
    int arr[SIZE] = {0};
    int i = 0, startIdx = 0, endIdx = 0;
    printf("Enter 10 elements of array: ");

    for(i = 0; i < SIZE; i++){
        scanf("%d", arr + i);
    }

    printf("Numbers that you enter are: ");
    partial_print(arr, SIZE);
    printf("\n");

    printf("Enter the starting and ending index: ");
    scanf("%d %d", &startIdx, &endIdx);
    printf("\n");

    printf("Sum from array[%d] to array[%d] is %d\n", startIdx, endIdx, partial_sum(arr+startIdx, arr+endIdx));
    printf("Partial array output: ");
    partial_print(arr + startIdx, (endIdx - startIdx)+1);

    return 0;
}

void partial_print(const int *arr_start, int size){
    int i = 0;
    for (i = 0; i < size; i++){
        printf("%d ", *(arr_start + i));
    }
}

int partial_sum(int *arr_start, int *arr_end){
    int* i = 0;
    int result = 0;
    for(i = arr_start; i <= arr_end; i++){
        result += *i;
    }

    return result;
}