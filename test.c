#include <stdio.h>

void print_matrix(int ary[][3], int n);

int main(){
    int i = 0;
    int j = 0;
    int arr[2][3] = {{1, 2, 3}, {4, 5, 6}};

    for (i = 0; i < 2; i++){
        for(j = 0; j < 3; j++){
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void print_matrix(int ary[][3], int n){

}