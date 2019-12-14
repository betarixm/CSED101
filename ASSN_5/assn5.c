#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "set.h"

int main(int argc, char *argv[]) {
    char cmd[100] = ""; // 명령어를 받을 배열
    Set baseSet; // 헤드 세트를 지시할 세트. baseSet.next에 첫번째 세트의 주소가 할당되고, baseSet의 나머지 정보는 사용하지 않는다.
    int c = 0; // 글자 하나를 저장
    char setName_1[31] = ""; // 세트 이름 입력 1
    char setName_2[31] = ""; // 세트 이름 입력 2
    char resultSetName[31] = ""; // 세트 이름 입력 3
    int target = 0;
    initSet(&baseSet); // baseSet 초기화
    FILE *fp = NULL; // 파일 포인터 선언

    (argc >= 2 && argv[1] != NULL) && (fp = fopen(argv[1], "r")); // 파일명이 존재할 경우, 그 파일명으로 파일을 열어서 fp에 파일 포인터 할당.
    if (fp == NULL) fp = stdin; // 파일이 없었거나, 여는데 실패한 경우 stdin으로 세팅.

    while (1) {
        (fp == stdin) && (printf(">> ")); // 입력이 stdin이라면 입력 대기 표시.
        fscanf(fp, "%s", cmd); // 입력.

        if (strcmp(cmd, "set") == 0) { // 집합 선언
            addSet(&baseSet, fp);
        } else if (strcmp(cmd, "show") == 0) { // 집합 출력
            c = fgetc(fp); // 입력 버퍼에서 한 글자 가져옴.
            if (c == 32) fscanf(fp, "%s", setName_1); // 그 글자가 띄어쓰기라면, 세트 이름을 입력 받음
            else
                strcpy(setName_1, ""); // 아니라면, 세트 이름을 비움
            showSet(&baseSet, setName_1);
        } else if (strcmp(cmd, "is_element") == 0) { // 요소 포함 여부
            fscanf(fp, "%s %d", setName_1, &target); // 세트와 타겟 데이터 입력
            printResult(executer(isElement, &baseSet, setName_1, target)); // executer를 통해 isElement 실행
        } else if (strcmp(cmd, "add") == 0) { // 요소 추가
            fscanf(fp, "%s %d", setName_1, &target); // 세트와 타겟 데이터 입력
            executer(addElement, &baseSet, setName_1, target); // executer를 통해 addElement를 실행
        } else if (strcmp(cmd, "pop") == 0) { // 요소 삭제
            fscanf(fp, "%s %d", setName_1, &target); // 세트와 타겟 데이터 입력
            executer(popElement, &baseSet, setName_1, target); // executer를 통해 popElement를 실행
        } else if (strcmp(cmd, "clear") == 0) { // 세트 삭제
            fscanf(fp, "%s", setName_1); // 세트 이름 입력
            clearSetIn(&baseSet, setName_1); // clearSetIn 호출
        } else if (strcmp(cmd, "union") == 0) { // 합집합
            fscanf(fp, "%s %s %s", setName_1, setName_2, resultSetName); // 세트 이름 3개 입력
            setUnion(&baseSet, setName_1, setName_2, resultSetName);
        } else if (strcmp(cmd, "intersection") == 0) { // 교집합
            fscanf(fp, "%s %s %s", setName_1, setName_2, resultSetName); // 세트 이름 3개 입력
            setDiffInter(&baseSet, setName_1, setName_2, resultSetName, 0);
        } else if (strcmp(cmd, "difference") == 0) { // 차집합
            fscanf(fp, "%s %s %s", setName_1, setName_2, resultSetName); // 세트 이름 3개 입력
            setDiffInter(&baseSet, setName_1, setName_2, resultSetName, 1);
        } else if (strcmp(cmd, "is_disjoint") == 0) { // 서로소 여부
            fscanf(fp, "%s %s", setName_1, setName_2); // 세트 이름 2개 입력
            printResult(isSubset_Disjoint(&baseSet, setName_1, setName_2, 0));
        } else if (strcmp(cmd, "is_subset") == 0) { // 포함 여부
            fscanf(fp, "%s %s", setName_1, setName_2); // 세트 이름 2개 입력
            printResult(isSubset_Disjoint(&baseSet, setName_1, setName_2, 1));
        } else if (strcmp(cmd, "quit") == 0) { // 나가기
            clearAllSet(&baseSet); // 동적 할당된 메모리 모두 free
            break;
        }

        while ((strcmp(cmd, "show") != 0) && ((c = fgetc(fp)) != '\n' && c != EOF)); // 입력 버퍼에서 줄바꿈 비우기
        strcpy(cmd, ""); // 명령어 초기화

        if ((fp != stdin) && feof(fp)) { // 파일이 eof라면 파일 스트림을 닫고, stdin으로 변경.
            fclose(fp);
            fp = stdin;
        }

    }

    return 0;
}