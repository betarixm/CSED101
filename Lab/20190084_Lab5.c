#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int seqSearch(int arr[], int target, int size);

int main(void){
	int i = 0;
	int target = 0;
	int idx = 0;
	int input[10] = { 0 };

	printf("Input 10 integer values: ");
	
	for (i = 0; i < 10; i++){
		scanf("%d", &input[i]);
	}
	
	printf("Entered number is        ");

	for (i = 0; i < 10; i++){
		printf("%d ", input[i]);
	}

	printf("\n");
	
	printf("Input an integer that you want to find: ");
	scanf("%d", &target);

	idx = seqSearch(input, target, 10);

	if (resultIdx == -1){
		printf("Not Found\n");
	} else {
		printf("The index of %d is %d\n", target, idx);
	}

	return 0;
}

int seqSearch(int arr[], int target, int size){
	int i = 0;
	for(i = 0; i < size; i++){
		if(arr[i] == target)
			return i;
	}

	return -1;
}
	
