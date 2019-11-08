#include <stdio.h>
#include "calc.h"

int main(){
    int a = 0, b = 0;

    printf("Enter two integers: ");
    scanf("%d %d", &a, &b);

    printf("Results:\n");
    printf("%d + %d = %d\n", a, b, add(a, b));
    printf("%d - %d = %d\n", a, b, sub(a, b));
    printf("%d * %d = %d\n", a, b, mul(a, b));
    printf("%d / %d = %.2f\n", a, b, div(a, b));

    return 0;

}