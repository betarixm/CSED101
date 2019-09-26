#include <stdio.h> // 표준입출력 라이브러리 include
#include <math.h>  // 수학 관련 함수를 이용하기 위해 math.h를 include

float calc_Vs(float temp); // 섭씨 온도에 따른 음속을 반환
float calc_distance(float t, float Vs); // 시간과 음속을 입력받아 물체와의 거리를 반환
void get_data(float *time1, float *time2); // 사용자로부터 time1과 time2를 입력받음

int main(void) {

    float degree = 0; // 섭씨 온도를 입력받는 변수
    float vs = 0; // 음속을 저장하는 변수

    float time1 = 0; // 1번째 시간 측정을 입력받는 변수
    float time2 = 0; // 2번째 시간 측정을 입력받는 변수

    // 섭씨 온도 입력
    printf("Input the temperature in Celsius: ");
    scanf("%f", &degree);

    // 음속 계산 및 출력
    vs = calc_Vs(degree);
    printf("Speed of sound is %.2f m/s\n", vs); // 소수 둘째자리까지 출력하기 위해 %.2f 사용

    // 시간 입력받고 거리 출력
    get_data(&time1, &time2);
    printf("The distance is %.2f m\n", calc_distance((time2 + time1) / 2, vs)); // 소수 둘째자리까지 출력하기 위해 %.2f 사용

    return 0;

}

float calc_Vs(float temp) { // 섭씨 온도에 따른 음속을 반환
    return (331.3 + 0.606 * temp);
}


float calc_distance(float t, float Vs) { // 시간과 음속을 입력받아 물체와의 거리를 반환
    return (t * Vs) / 2;
}

void get_data(float *time1, float *time2) { // 사용자로부터 time1과 time2를 입력받음
    printf("Input the first measured time (seconds): ");
    scanf("%f", time1); // time1은 포인터 변수이므로 &time1과 같이 작성하지 않는다.
    printf("Input the second measured time (seconds): ");
    scanf("%f", time2); // time2는 포인터 변수이므로 &time2와 같이 작성하지 않는다.
}