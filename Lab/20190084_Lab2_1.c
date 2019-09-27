#include <stdio.h>

int add(int num1, int num2);
float average(int num1, int num2);
void printResult(int x, int y, int sum, float avg);

int main(void){
    int num1 = 0;
    int num2 = 0;
	int sum = 0;
	float avg = 0;

    printf("Enter two integers: ");
    scanf("%d %d", &num1, &num2);
    
	sum = add(num1, num2);
	avg = average(num1, num2);

	printResult(num1, num2, sum, avg);

	return 0;

}

int add(int num1, int num2){
	return num1 + num2;
}

float average(int num1, int num2){
	return ((float)num1 + (float)num2) / 2;
}

void printResult(int x, int y, int sum, float avg) {
    printf("The sum of %d and %d is %d\n", x, y, sum);
    printf("The average of numbers is %.1f\n", avg);
}