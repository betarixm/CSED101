#include <stdio.h>
#include <math.h>

float calc_Vs(float temp);
float calc_distance(float t, float Vs);
void get_data(float *time1, float *time2);

int main(void) {

    float degree = 0;
    float vs = 0;

    float time1 = 0;
    float time2 = 0;

    printf("Input the temperature in Celsius: ");
    scanf("%f", &degree);

    vs = calc_Vs(degree);
    printf("Speed of sound is %.2f m/s\n", vs);

    get_data(&time1, &time2);
    printf("The distance is %.2f m\n", calc_distance((time2 + time1) / 2, vs));

    return 0;

}

float calc_Vs(float temp) { // 음속을 계산
    return (331.3 + 0.606 * temp);
}


float calc_distance(float t, float Vs) { // 물체와의 거리를 구하여 반환
    return (t * Vs) / 2;
}

void get_data(float *time1, float *time2) {
    printf("Input the first measured time (seconds): ");
    scanf("%f", time1);
    printf("Input the second measured time (seconds): ");
    scanf("%f", time2);
}