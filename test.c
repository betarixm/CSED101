#include <stdio.h>
#include <stdlib.h>

void Find_minmax(int *a, int size, int* max, int* min);

int main(){
    int size, i, max, min;
    int *a;

    printf("Enter array size: ");
    scanf("%d", &size);

    a = (int*)malloc(size*sizeof(int));

    for(i = 0; i < size; i++){
        scanf("%d", a+i);
    }

    printf("Entered numbers are\n");
    for(i = 0; i <size; i++){
        printf("%d ", *(a+i));
    }

    max = *a;
    min = *a;

    Find_minmax(a, size, &max, &min);
    printf("\nMaximum is %d\n", max);
    printf("Minimum is %d\n", min);

    printf("Enter changed array size: ");
    scanf("%d", &size);

    a = (int*)realloc(a, size*sizeof(int));

    for(i = 0; i < size; i++){
        scanf("%d", a+i);
    }

    printf("Entered numbers are\n");
    for(i = 0; i <size; i++){
        printf("%d ", *(a+i));
    }

    max = *a;
    min = *a;

    Find_minmax(a, size, &max, &min);
    printf("\nMaximum is %d\n", max);
    printf("Minimum is %d\n", min);

    free(a);
}

void Find_minmax(int *a, int size, int* max, int* min){
    int i;
    for(i = 0; i < size; i++){
        if(*(a+i) > *max){
            *max = *(a+i);
        }
        if(*(a+i) < *min){
            *min = *(a+i);
        }
    }
}