#include <stdio.h> // 표준입출력 라이브러리 include
#include <math.h>  // 수학 관련 함수를 이용하기 위해 math.h를 include

int exch(int target, int *amount); // target 단위의 달러 개수를 반환해주고, 반환한 만큼 amount를 감소시키는 함수

int main(){ // main에 프로그램 구현

    int amount = 0; // 입력받는 원 화의 양
    int ex_rate = 0; // 입력받는 환율
    int MUL = 100; // int 형을 이용하여 소수 둘째자리까지 표현하고, ceil 함수가 소수 셋째자리에서 올림할 수 있도록 하는 보정 상수

    // 원화 입력
    printf("Enter the amount of exchange: ");
    scanf("%d", &amount);

    // 환율 입력
    printf("Enter today's exchange rate: ");
    scanf("%d", &ex_rate);

    // 올림 수행
    // 소수 셋째자리에서 올림할 때 사용하는 코드: amount = (int)ceil((double)((int)((double)amount/ex_rate*pow(10,3)))/10);
    amount = (int)ceil((double)amount/ex_rate*100); // ceil 함수의 정상적인 작동을 위해 double으로 캐스팅. 그 후 amount를 달러로 변환. 소수 둘째 자리 까지 표현하기 위해 100을 곱한다.

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
    int d = (target<100); // target 단위에 따라 '$' 또는 'cent'를 출력하기 위한 식별자.
    printf("%d %c%c%c%c:   %d\n", target/(100-99*d), 36 + d*63, 1 + d*100, 1 + d*109, 1 + d*115, *amount/target); // target 단위의 달러 개수 출력. 아스키코드를 이용해 target에 따라 '$' 또는 'cent'를 출력한다.
    *amount = *amount - target * (*amount/target); // 변환한 만큼 amount 차감
    return 0;
}