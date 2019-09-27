#include <stdio.h>
#include <math.h>

void triangle(int a, int b, float* perimeter, float* area);
void print(float perimeter, float area);

int main(void){

    int a = 0;
    int b = 0;

    float perimeter = 0;
    float area = 0;

    printf("Enter two integer sides: ");
    scanf("%d %d", &a, &b);

    triangle(a, b, &perimeter, &area);
    print(perimeter, area);

    return 0;
}

void triangle(int a, int b, float* perimeter, float* area){
    *perimeter = sqrt(pow(a, 2) + pow(b, 2)) + a + b;
    *area = ((float)(a*b))/2;
}
void print(float perimeter, float area){
    printf("Perimeter: %f\n", perimeter);
    printf("Area: %f\n", area);
}