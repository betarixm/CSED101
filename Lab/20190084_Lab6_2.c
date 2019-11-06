// macOS, Clion에서 작성.

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#define MAX 6

void bubble_sort(int a[], int size);
void swap(int *x, int *y);
void print_array(int a[], int size);
void input_array(int a[], int size);

int main()
{
    int ary[MAX];

    printf("Input integer numbers to be sorted: ");
    input_array(ary, MAX);

    printf(" Unsorted: ");
    print_array(ary, MAX);

    printf("----------------------------------\n");
    bubble_sort(ary, MAX);
    printf("----------------------------------\n");

    printf("   Sorted: ");
    print_array(ary, MAX);

    return 0;
}

void bubble_sort(int a[], int size){
    int step = 0, j = 0;
    int target = 0;

    for(step = 1; step < (size); ++step){
        for(j = (size - 1); j > target; j--){
            if(a[j] < a[j-1]){
                swap(&a[j], &a[j-1]);
            }
        }

        printf("[step %2d]: ", step);
        print_array(a, size);
    }
}

void swap(int *x, int *y){
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

void print_array(int a[], int size){
    int i = 0;
    while(i < size){
        printf("%4d", a[i++]);
    }
    printf("\n");
}

void input_array(int a[], int size){
    int i = 0;
    while(i < size){
        scanf("%d", &a[i++]);
    }
}

