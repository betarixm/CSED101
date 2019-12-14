#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

void addSet(Set *baseSet, FILE *fp) { // 세트 추가

    int i = 0, setSize = 0, tmp = 0;
    char setName[21] = "";
    Set *newSet;

    fscanf(fp, "%s", setName); // 세트 이름 입력
    fscanf(fp, "%d", &(setSize)); // 세트 사이즈 입력

    newSet = appendSet(baseSet, setName); // 빈 세트 추가
    if (setSize < 0) return; // 입력된 세트 사이즈가 0보다 작은 경우 함수 종료
    if (newSet != 0 && 0 <= setSize) { // 세트 추가가 성공하고, 세트 사이즈가 0 이상이라면
        for (i = 0; i < setSize; i++) { // 주어진 세트 사이즈 만큼 입력받음
            fscanf(fp, "%d", &tmp); // 데이터 입력
            addElement(newSet, tmp); // 엘리먼트 오름차순으로 추가
        }
    }
}

void showSet(Set *baseSet, char *setName) { // 세트 출력
    Set *setTarget = baseSet->next; // baseSet의 next는 첫 set를 가리킴.
    int isPrint;
    while (setTarget != NULL) {
        isPrint = (strcmp(setName, "") == 0) ? (1) : (strcmp(setName, setTarget->set_name) == 0); // 입력된 세트의 이름이 비어있거나, 현자 타겟 세트와 이름과 같은 경우
        (isPrint) && (showElements(setTarget)); // showElements로 엘리먼트들 출력
        setTarget = setTarget->next; // 타겟을 다음 세트로 변경
    }
}

int showElements(Set *set) { // 엘리먼트 출력
    Element *eleTarget = set->ele_head; // 첫 엘리먼트
    printf("%s : ", set->set_name); // 세트 이름 출력
    while (eleTarget != NULL) { // 엘리먼트가 null이 아닐 때 까지
        printf("%d ", eleTarget->data); // 엘리먼트를 출력하고
        eleTarget = eleTarget->next; // 다음 엘리먼트로 커서 변경
    }
    printf("\n");

    return 1;
}

int isElement(Set *set, int target) { // 엘리먼트 포함 여부
    Element *e = set->ele_head; // 첫 엘리먼트
    while (e != NULL) { // 엘리먼트가 null이 아닐 때까지
        if (e->data == target) { // 만약 데이터가 엘리먼트의 데이터와 같다면
            return 1; // 1 반환. (함수 종료)
        }
        e = e->next; // 다음 엘리먼트로 변경
    }
    return 0; // 0 반환.
}

int addElement(Set *set, int data) {
    Element *prevEle = set->ele_head; // 이전 엘리먼트 저장.
    Element *tmp;
    if (!isElement(set, data)) { // 이미 존재하는 엘리먼트가 아닐 경우
        tmp = (Element *) malloc(sizeof(Element)); // 새로운 엘리먼트 동적 할당
        tmp->data = data; // 새로운 엘리먼트에 데이터 저장
        if (set->set_size == 0 || data < set->ele_head->data) { // 만약 세트 사이즈가 0이거나, 첫 엘리먼트보다 삽입하고자 하는 데이터가 작은 경우
            tmp->next = set->ele_head; // tmp를 헤드로 설정한다.
            set->ele_head = tmp;
        } else {
            // 가장 마지막 엘리먼트를 찾는다.
            while (prevEle->next != NULL && (prevEle->next->data < data)) {
                prevEle = prevEle->next;
            }
            // 가장 마지막 엘리먼트의 다음에 tmp 엘리먼트를 삽입한다.
            tmp->next = prevEle->next;
            prevEle->next = tmp;
        }
        (set->set_size)++; // 세트 사이즈 증가
    }
    return 1;
}

int popElement(Set *set, int data) {
    Element *cur = set->ele_head;
    Element *tmp;

    if(cur == NULL) return 0; // 빈 세트일 경우 0 반환
    if (cur->data == data) { // 맨 처음 엘리먼트가 찾는 값과 같은 경우
        // 헤드 엘리먼트를 삭제하고 다음 엘리먼트 연결
        tmp = set->ele_head;
        set->ele_head = tmp->next;
        free(tmp); // 엘리먼트 할당 해제
        (set->set_size)--; // 세트 사이즈 줄이기
    } else {
        while (cur != NULL && cur->next != NULL) { // 모든 엘리먼트 안에서 탐색
            if (cur->next->data == data) { // 같은 원소의 이전 원소를 찾았다면
                // 이전 원소를 그 다다음 원소와 연결
                tmp = cur->next;
                cur->next = tmp->next;
                free(tmp); // 삭제할 엘리먼트 할당 해제
                (set->set_size)--; // 세트 사이즈 줄이기
                break;
            }
            cur = cur->next; // 커서 변경
        }
    }
    return 1;
}

int clearSet(Set *baseSet, Set *set) {
    Set *cur = baseSet->next;
    Set *tmp;
    if (set == baseSet->next) { // 삭제하고자 하는 세트가 첫 세트일 경우
        // 첫 세트를 두번째 세트에 연결
        tmp = baseSet->next;
        baseSet->next = tmp->next;
        clearElements(tmp->ele_head); // 기존 첫 세트 엘리먼트 할당 해제
        free(tmp); // 기존 첫 세트 삭제
        return 1;
    } else {
        while (cur->next != NULL) { // 모든 세트 탐색
            if (cur->next == set) { // 삭제하고자 하는 세트를 찾았을 경우
                // 삭제하고자 하는 이전 세트와 그 다다음 세트 연결
                tmp = cur->next;
                cur->next = tmp->next;
                clearElements(tmp->ele_head); // 삭제하고자 하는 세트의 엘리먼트 할당 해제
                free(tmp); // 삭제하고자 하는 세트 할당 해제
                return 1;
            }
            cur = cur->next;
        }
    }
    return 0;
}

void clearElements(Element *baseEle) { // 모든 엘리먼트 할당 해제
    Element *cur = baseEle;
    Element *tmp;
    while (cur != NULL) { // 모든 엘리먼트에 대해서
        tmp = cur; // 현재 커서 임시 저장
        cur = cur->next; // 다음 엘리먼트로 커서 이동
        free(tmp); // 할당 해제
    }
}

int clearSetIn(Set *baseSet, char *setName) { // 이름으로 세트 찾아서 clear
    Set *set = getSetByName(baseSet, setName); // 이름으로 세트 검색
    if (set == 0) return 0; // 만약 찾은 결과가 0이라면 0 반환
    return clearSet(baseSet, set); // 아니라면 해당 세트를 clear 시킴.
}

void clearAllSet(Set *baseSet) { // 모든 세트 할당 해제
    Set *set = baseSet->next, *tmp;
    while (set != NULL) { // 모든 세트에 대해서
        tmp = set->next; // 다음 세트 미리 저장
        clearElements(set->ele_head); // 현재 세트 요소 할당 해제
        free(set); // 현재 세트 할당 해제
        set = tmp; // 다음 세트로 커서 이동
    }
}

void initSet(Set *set) { // 세트 초기화
    set->set_size = 0;
    set->next = NULL;
    set->ele_head = NULL;
}

int executer(int (*f)(Set *, int), Set *baseSet, char *setName, int target) { // 이름으로 세트 찾아서 함수 실행
    Set *set = getSetByName(baseSet, setName); // 이름으로 세트 찾기
    return (set == 0) ? (-1) : f(set,target); // 찾은 세트 결과가 없다면 -1을 반환, 아니라면 함수 포인터로 함수 실행
}

Set *getSetByName(Set *baseSet, char *setName) { // 이름으로 함수 찾기
    Set *cur = baseSet->next;
    while (cur != NULL) { // 모든 세트들에 대해서
        if (strcmp(cur->set_name, setName) == 0) { // 현재 세트의 이름이 입력받은 이름과 같다면
            return cur; // 현재 세트의 주소 반환.
        }
        cur = cur->next;
    }
    return 0; // 찾는데 실패한 경우 0 반환.
}

Set *appendSet(Set *baseSet, char *setName) { // 빈 세트 추가

    Set *newSet, *prevSet = baseSet;
    if (getSetByName(baseSet, setName) != 0) { // 이미 존재하는 이름이라면
        return 0; // 0 반환
    } else {
        newSet = (Set *) malloc(sizeof(Set)); // 새로운 세트 할당
        // 현재 가장 마지막 세트 탐색
        while (prevSet->next != NULL) {
            prevSet = prevSet->next;
        }
        prevSet->next = newSet; // 현재 가장 마지막 세트의 뒤에 새로운 세트 연결.
        initSet(newSet); // 새로운 세트 초기화
        strcpy(newSet->set_name, setName); // 새로운 세트 이름 입력
        return newSet; // 새로운 세트의 주소 반환
    }
}

Element *appendElement(Set* set, Element **target, int data){ // 가장 뒤에 엘리먼트 추가
    Element *tmp = (Element*)malloc(sizeof(Element)); // 새로운 엘리먼트 할당
    *tmp = (Element){data, NULL}; // 새로운 엘리먼트에 데잍 ㅓ할당
    if(*target == NULL){ // 기존의 가장 마지막 엘리먼트가 비었다면
        set->ele_head = tmp; // 새로운 엘리먼트를 헤드에 추가
    } else {
        (*target)->next = tmp; // 가장 마지막 엘리먼트 뒤에 새로운 엘리먼트 연결
    }
    *target = tmp; // 가장 마지막 엘리먼트를 새로운 엘리먼트로 변경
    set->set_size++; // 세트 사이즈 증가
    return tmp; // 가장 마지막 엘리먼트 주소 반환
}

Set *setUnion(Set *baseSet, char *targetName_1, char *targetName_2, char *resultName) { // 합집합
    Set *targetSet_1 = getSetByName(baseSet, targetName_1); // 이름으로 세트 1 탐색
    Set *targetSet_2 = getSetByName(baseSet, targetName_2), *resultSet; // 이름으로 세트 2 탐색
    Element *ele1, *ele2, *resultEle = NULL, **minEle;
    if ((targetSet_1 == 0) || (targetSet_2 == 0)) return 0; // 세트 1이나 세트 2가 존재하지 않는다면 바로 종료
    ele1 = targetSet_1->ele_head; // 세트 1의 첫 엘리먼트
    ele2 = targetSet_2->ele_head; // 세트 2의 첫 엘리먼트
    resultSet = appendSet(baseSet, resultName); // 결과 세트 추가
    if (resultSet == 0) return 0; // 결과 세트 추가에 실패한 경우 바로 종료

    // 세트 1과 세트 2에서 최소인 원소의 데이터를 결과 세트에 추가하고, 그 세트의 엘리먼트 포인터를 진행시키는 방식으로
    // 오름차순을 이용한 합집합 구하기를 실현할 수 있다.
    while (ele1 != NULL || ele2 != NULL) { // 엘리먼트 1, 2가 둘 다 NULL이 아닐 동안
        minEle = minElement(&ele1, &ele2); // NULL이 아니면서 최소인 요소 찾기
        appendElement(resultSet, &resultEle, (*minEle)->data); // 해당 요소의 데이터를 결과 세트에 추가
        if((ele1 != NULL && ele2 != NULL) && (ele1->data == ele2->data)){ // 두 엘리먼트가 NULL이 아니고 데이터가 같다면
            // 엘리먼트 둘 다 다음 엘리먼트로 진행
            ele1 = ele1->next;
            ele2 = ele2->next;
        } else {
            (*minEle) = (*minEle)->next; // 작은 엘리먼트만 그 다음 엘리먼트로 변경
        }
    }

    return resultSet; // 결과 세트 주소 반환
}

Set *setDiffInter(Set *baseSet, char *srcName, char *targetName, char *resultName, int isDiff) { // 차집합, 교집
    Set *srcSet = getSetByName(baseSet, srcName), *targetSet = getSetByName(baseSet, targetName), *resultSet; // 원본 세트와 타겟 세트 이름으로 탐색
    Element *srcEle, *targetEle, *resultEle = NULL;
    int isFit; // isFit이 0이면 결과 세트에 원소를 추가하지 않고, 아니면 결과 세트에 원소를 추가한다.
    if ((srcSet == 0) || (targetSet == 0)) return 0; // 만약 타겟 세트나 원본 세트를 찾는데 실패했다면 바로 종료
    targetEle = targetSet->ele_head;
    resultSet = appendSet(baseSet, resultName); // 결과 세트 추가
    if(resultSet == 0) return 0; // 결과 세트 추가에 실패한 경우 바로 종료
    for (srcEle = srcSet->ele_head; srcEle != NULL; srcEle = srcEle->next) { // 원본 세트의 원소들을 읽어나가며
        if(targetEle == NULL){ // 타겟 엘리먼트가 없는 경우 (타겟의 더 진행할 요소가 없는 경우)
            // 차집합을 구하는 경우, 이때 원소 집합의 원소를 추가해야하므로 isFit == 1이 되어야 하고,
            // 교집합을 구하는 경우, 이때 원소 집합의 원소를 추가하면 안되므로 isFit == 0이 되어야 한다.
            // 즉, 더 탐색할 타겟 엘리먼트가 없는 경우, isFit은 isDiff와 같은 값을 가져야 한다.
            isFit = isDiff;
        } else {
            // 현재 원본 엘리먼트보다 첫번째로 크거나 같은 타겟 엘리먼트를 구한다.
            // for문 안에서 타겟 엘리먼트는 따로 초기화 되지 않기 때문에, 오름차순을 적절히 이용하였다고 볼 수 있다.
            while ((targetEle->next != NULL) && targetEle->data < srcEle->data) targetEle = targetEle->next;
            // 차집합을 구하는 경우라면 원본 엘리먼트와 타겟 엘리먼트가 다를 때 원본 원소가 추가되어야하고,
            // 교집합을 구하는 경우라면 원본 엘리먼트와 타겟 엘리먼트가 같을 때 원본 원소가 추가되어야한다.
            isFit = ((isDiff) ? (srcEle->data != targetEle->data) : (srcEle->data == targetEle->data));
        }
        (isFit) && (appendElement(resultSet, &resultEle, srcEle->data)); // Fit한 경우에 결과 세트에 원소를 추가한다.
    }

    return resultSet; // 결과 세트의 주소 반환
}

int isSubset_Disjoint(Set *baseSet, char *setName_1, char *setName_2, int isSub) { // 포함, 서로소 여부
    int tmp = 0;
    char setName[31] = "";
    makeUniqName(baseSet, setName); // 현재까지 존재하지 않는 유일한 이름 생성
    // 포함 여부: 세트 1에서 세트 2을 뺀 차집합이 공집합인 경우, 세트 1은 세트 2에 포함된다고 할 수 있다.
    // 서로소 여부: 세트 1과 세트 2의 교집합이 공집합인 경우, 서로 서로소라고 할 수 있다.
    // 포함 여부를 구하는 경우에는 차집합을, 서로소 여부를 구하는 경우에는 교집합을 구한다.
    Set *resultSet = setDiffInter(baseSet, setName_1, setName_2, setName, isSub);
    if (resultSet == 0) return -1; // 결과 세트 생성에 실패한 경우 -1을 반환한다.
    tmp = (resultSet->set_size == 0); // 아니라면 임시로 결과 세트의 사이즈가 0과 같은지 확인한다.
    clearSet(baseSet, resultSet); // 결과 세트를 삭제한다. (임시로 만든 세트이므로)
    return tmp; // 결과 반환
}

void makeUniqName(Set *baseSet, char *setName) { // 유일한 이름 생성
    int i = 0;
    do {
        i = 0;
        // 정상적인 입력으로는 setName[20]에 널 문자가 아닌 문자를 입력할 수 없으므로,
        // 유일한 이름이 생성될 확률이 높다.
        while (i < 21 && (setName[i++] = (char) (rand() % 90 + 33))); // 인덱스 0부터 20까지 랜덤한 문자로 채우기
    } while (getSetByName(baseSet, setName) != 0); // 이미 있는 이름이 아닐때 까지 반복
}

Element ** minElement(Element **a, Element **b) { // NULL이 아니면서 최소인 엘리먼트 반환
    // NULL인 경우 구조체 참조를 할 수 없다는 점을 염두하자.
    if (*a == NULL && *b != NULL) return b; // a만 NULL인 경우 b 반환
    else if (*a != NULL && *b == NULL) return a; // b만 NULL인 경우 a 반환
    else if (*a == NULL && *b == NULL) return NULL; // a, b 둘 다 NULL인 경우, NULL 반환
    else return ((*a)->data <= (*b)->data) ? (a) : (b); // a, b 둘 다 NULL이 아닌 경우, 작은 엘리먼트를 반환.
}

void printResult(int result){ // 결과 출력: 많은 함수들이 실패시 -1을 반환하기 때문에, 이 경우를 커버하기 위함.
    if(result != -1){ // 결과가 -1이 아닐 경우
        printf("%d\n", result); // 결과를 출력한다.
    }
}