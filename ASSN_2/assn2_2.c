#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

void loadFile(int *len, long long int *block_0, long long int *block_1, long long int *block_2); // 문제에서 요구하는 파일을 열어서 8글자씩 block_0, block_1, block_2에 저장한다.
char *getTrialAddr(int n, long long int *block_0, long long int *block_1, long long int *block_2); // 파일의 block_0, block_1, block_2에서 n번째 글자의 주소를 찾아서 반환한다.
void getTrialInfo(int idx_trial, int *trial_prev, int *trial_0, int *trial_1, int *trial_2, long long int *block_0, long long int *block_1, long long int *block_2); // 원하는 trial로부터 -1, 0, 1, 2번째의 trial들을 trial_prev, trial_0, trial_1, trial_2에 저장한다.
int encode(int c, int prevPoint); // 글자와, 전 점수를 받아서 점수로 변환해준다.
void encodeToPoints(int *trial_prev, int *trial_0, int *trial_1, int *trial_2); // trial_prev,trial_0, trial_1, trial_2에 담긴 정보를 점수로 변경해준다.

int main(void) {
    long long int block_0 = 0, block_1 = 0, block_2 = 0; // 텍스트 파일을 읽어와서 저장할 공간들
    int len = 0, idx_trial = 0, idx_frame = 0, result = 0; // len: 파일의 텍스트 길이, idx_trial: 현재 처리 중인 trial의 인덱스, idx_frame: 현재 처리 중인 frame의 인덱스, result: 점수 최종 합
    int trial_0 = 0, trial_1 = 0, trial_2 = 2, trial_prev = 0; // trial_n: 현재 처리할 trial의 인덱스로부터 n번째 떨어진 번째 글자 정보. trial_prev: 현재 처리할 trial의 이전 인덱스 정보.
    char isStrike = 0, isSpare = 0, isNotLast = 0; // isStrike: 현재 프레임이 스트라이크인가? isSpare: 현재 프레임이 스페어인가? isNotLast: 현재 프레임이 마지막 프레임이 아닌가?

    loadFile(&len, &block_0, &block_1, &block_2); // File을 읽어서 block_n에 저장.

    while (idx_trial <= len) { // trial의 인덱스가 파일의 글자 수 이하일 때 까지 반복
        getTrialInfo(idx_trial, &trial_prev, &trial_0, &trial_1, &trial_2, &block_0, &block_1, &block_2); //trial_prev, trial_0, trial_1, trial_2에 현재 인덱스를 기준으로 하는 글자를 1개씩 저장.

        isStrike = (trial_0 == 'S'); // 현재 trial 인덱스의 trial이 'S'이면 현재 프레임은 스트라이크.
        isSpare = (trial_1 == 'P'); // (현재 trial 인덱스 + 1)의 trial이 'P'이면 현재 프레임은 스페어.
        isNotLast = (idx_frame != 9); // 현재 프레임 인덱스가 9가 아니면 현재 프레임은 마지막 프레임이 아님.

        encodeToPoints(&trial_prev, &trial_0, &trial_1, &trial_2); // trial_prev, trial_0, trial_1, trial_2에 담긴 글자 정보를 점수로 변환해서 저장.

        // trial_0: 어떤 경우이든 result에 더해져야 한다.
        // trial_1: 스트라이크가 아니거나, 마지막 라운드가 아닌 상황이면 result에 더해져야 한다. 스트라이크일 때 이 값은 1번째 추가 점수이다.
        // trail_2: 스트라이크나 스페어일 때, 마지막 프레임이 아니면 result에 더해져야한다. 스페어 일때 이 값은 1번째 추가 점수이며, 스트라이크일 때 이 값은 2번째 추가 점수이다.
        result += trial_0 + ((!isStrike) || isNotLast) * trial_1 + ((isStrike || isSpare) && isNotLast) * trial_2;

        idx_frame += 1 * isNotLast; // 마지막 프레임이 아니면 프레임 인덱스를 더한다.
        idx_trial += 2 - isStrike; // idx_trial을 기본 2개씩 더해야하지만, strike인 경우에는 한 프레임의 trial이 1이므로 1을 idx_trial에 더한다.
    }

    printf("TOTAL SCORE: %d\n", result); // 최종 점수를 출력한다.

    return 0;

}

char *getTrialAddr(int n, long long int *block_0, long long int *block_1, long long int *block_2) {
    // long long int가 8바이트 일때를 기준으로 작성된 주석입니다.
    int size = sizeof(*block_0); // 블록의 사이즈를 기준으로 주소를 조회하기에, 사이즈 값을 미리 받아둔다.

    // char* 를 이용하면 변수를 1 바이트씩 읽고 쓸 수 있으며, block 1개당 8글자씩 저장한다.
    char *addr_block_0 = (char *) block_0; // block_0의 가장 낮은 주소를 저장
    char *addr_block_1 = (char *) block_1; // block_1의 가장 낮은 주소를 저장
    char *addr_block_2 = (char *) block_2; // block_2의 가장 낮은 주소를 저장

    if (0 <= n && n < size * 1) return (addr_block_0 + n); // 0 ~ 7번째 글자의 주소 반환
    if (size * 1 <= n && n < size * 2) return (addr_block_1 + (n - size * 1)); // 8 ~ 15번째 글자의 주소 반환
    if (size * 2 <= n && n < size * 3) return (addr_block_2 + (n - size * 2)); // 16 ~ 23번째 글자의 주소 반환

    return addr_block_0; // block_0의 가장 낮은 주소를 반환. 해당 코드에서 -1번째 주소를 조회할 때의 예외 처리.
}

void loadFile(int *len, long long int *block_0, long long int *block_1, long long int *block_2) {
    FILE *fp = fopen("Example.txt", "r"); // 파일 스트림을 연다.
    while (EOF != fscanf(fp, "%c", getTrialAddr(*len, block_0, block_1, block_2))) { // EOF가 아닐 때 까지 1글자씩 len을 인덱스로 해서 저장한다.
        printf("%c", *(getTrialAddr(*len, block_0, block_1, block_2))); // 실행 시 읽은 파일을 출력
        *len = *len + 1; // len은 글자 길이가 되어야 하므로 1개 증가시킨다.
    }
    fclose(fp); // 파일 스트림을 닫는다.
    printf("\n");
}

void getTrialInfo(int idx_trial, int *trial_prev, int *trial_0, int *trial_1, int *trial_2, long long int *block_0, long long int *block_1, long long int *block_2) {
    *trial_prev = (*getTrialAddr(idx_trial - 1, block_0, block_1, block_2)); // (idx_trial - 1)번째 글자를 trial_prev에 저장.
    *trial_0 = (*getTrialAddr(idx_trial, block_0, block_1, block_2));        // (idx_trial + 0)번째 글자를 trial_0에 저장.
    *trial_1 = (*getTrialAddr(idx_trial + 1, block_0, block_1, block_2));    // (idx_trial + 1)번째 글자를 trial_1에 저장.
    *trial_2 = (*getTrialAddr(idx_trial + 2, block_0, block_1, block_2));    // (idx_trial + 2)번째 글자를 trail_2에 저장.
}

int encode(int c, int prevPoint) {
    // c가 '-'이면 0점, 'S'이면 10점, 'P'이면 10에서 이전 점수를 뺀 값, 나머지이면 아스키 코드를 고려하여 48을 뺀 값, 즉 원래 숫자가 의미하는 점수를 반환한다.
    return (c == '-') ? (0) : ((c == 'S') ? (10) : ((c == 'P') ? (10 - prevPoint) : ((c == 0) ? (0) : (c - 48))));
}

void encodeToPoints(int *trial_prev, int *trial_0, int *trial_1, int *trial_2){
    *trial_prev = encode(*trial_prev, 0); // trial_prev의 글자를 점수로 encode.
    *trial_0 = encode(*trial_0, *trial_prev); // trial_0의 글자를 점수로 encode.
    *trial_1 = encode(*trial_1, *trial_0); // trial_1의 글자를 점수로 encode.
    *trial_2 = encode(*trial_2, *trial_1); // trial_2의 글자를 점수로 encode.
}