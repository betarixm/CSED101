#include <stdio.h>
#include <math.h>

int convert(int amount, int ex_rate){
    return (int)ceil((double)((int)((double)amount/ex_rate*pow(10,3)))/10);
}

int main(void){
    printf("%lf %d\n", ((double)11000111/10000), convert(11000111,10000));
    return 0;
}