#include <stdio.h>

int main(void){
    int num1 = 0;
    int num2 = 0;

    printf("Input two integers: ");
    scanf("%d %d", &num1, &num2);

    printf("%d + %d = %d\n", num1, num2, num1 + num2);
    printf("%d - %d = %d\n", num1, num2, num1 - num2);
    printf("%d * %d = %d\n", num1, num2, num1 * num2);
    printf("%d / %d = %d\n", num1, num2, num1 / num2);
    printf("%d %% %d = %d\n", num1, num2, num1 % num2);

    return 0;
}