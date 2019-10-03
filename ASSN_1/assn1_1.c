#include <stdio.h> // 표준입출력 라이브러리 include
#include <math.h>  // 수학 관련 함수를 이용하기 위해 math.h를 include

int exchange(int target, int *amount); // target 단위의 달러 개수를 반환해주고, 반환한 만큼 amount를 감소시키는 함수
int exchange_cent(int target, int *amount); // exchange와 같은 역할을 하되, cent 단위로 printf 하기 위해 선언
int exch(int target, int *amount);

int main(){ // main에 프로그램 구현

    int amount = 0; // 입력받는 원 화의 양
    int ex_rate = 0; // 입력받는 환율
    int MUL = 100; // int 형을 이용하여 소수 둘째자리까지 표현하고, ceil 함수가 소수 셋째자리에서 올림할 수 있도록 하는 보정 상수

    // 원 화 입력
    printf("Enter the amount of exchange: ");
    scanf("%d", &amount);

    // 환율 입력
    printf("Enter today's exchange rate: ");
    scanf("%d", &ex_rate);

    // 올림 수행
    amount = (int)ceil((double)((int)((double)amount/ex_rate*pow(10,3)))/10); // ceil 함수의 정상적인 작동을 위해 double으로 캐스팅. 그 후 amount를 달러로 변환.

    exch(20*MUL, &amount); // 20달러 단위로 변환
    exch(10*MUL, &amount); // 10달러 단위로 변환
    exch(5*MUL, &amount); // 5달러 단위로 변환
    exch(1*MUL, &amount); // 1달러 단위로 변환
    exch((int)(0.5*MUL), &amount); // 50센트 단위로 변환
    exch((int)(0.1*MUL), &amount); // 10센트 단위로 변환
    exch((int)(0.01*MUL), &amount); // 1센트 단위로 변환

    printf("Written by %c%c%c%c%c%c%c%c%c%c%c%c\n", 71, 119, 111, 110, 32, 77, 105, 110, 32, 74, 97, 101); // 아스키코드로 이름 출력
    
    return 0;
}

int exch(int target, int *amount){ // target 단위의 달러 개수를 반환해주고, 반환한 만큼 amount를 감소시키는 함수
    int d = (target<100);

    printf("%d %c%c%c%c:   %d\n", target/(100-99*d), 36 + d*63, 1 + d*100, 1 + d*109, 1 + d*115, *amount/target); // target 단위의 달러 개수 출력
    *amount = *amount - target * (*amount/target); // 변환한 만큼 amount 차감
    return 0;
}