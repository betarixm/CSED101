#define L(x) *(l(x, &l1, &l2, &l3, &l4, sizeof(l1)))
#define R(x) *(r(x, &r1, &r2, &r3))
#include <stdio.h>

char* l(int n, long long int *b1, long long int *b2, long long int *b3, long long int *b4, int size);
char* r(int n, long long int *b1, long long int *b2, long long int *b3);
int encode(char c, int prevPoint);

int main(void){

    long long int l1 = 0, l2 = 0, l3 = 0, l4 = 0;
    long long int r1 = 0, r2 = 0, r3 = 0;

    int i = 0;
    int len = 0;

    int esp = 0;
    int rsp = 0;

    int res = 0;

    FILE *fp;

    fp = fopen("/Users/beta/Desktop/POSTECH/2019/CSED101/ASSN/ASSN_2/Example1.txt", "r");
    while(EOF != fscanf(fp, "%c", &L(len))){printf("%c",L(len));len++;}
    printf("\n");

    while(esp<=len){
        char isStrike = (L(esp)=='S');
        char isSplit = (L(esp+1)=='P');
        char notLast = (rsp!=9);

        int p0=encode(L(esp), L(esp-1));
        int p1=encode(L(esp+1), p0);
        int p2=encode(L(esp+2), p1);

        if (isStrike){
            R(rsp) += (p0 + notLast*(p1 + p2));
        } else if (isSplit) {
            R(rsp) += (p0 + p1 + notLast*(p2));
        } else {
            R(rsp) += (p0 + p1);
        }

        esp = esp + 2 - isStrike;
        rsp = rsp + 1*(notLast);
    }

    for(i = 0; i<10; i++){
        res = res + R(i);
    }

    printf("TOTAL SCORE: %d\n",res);
}

int encode(char c, int prevPoint){
    return (c=='-')?(0):((c=='S')?(10):((c=='P')?(10-prevPoint):((c==0)?(0):(c-48))));
}

char* l(int n, long long int *b1, long long int *b2, long long int *b3, long long int *b4, int size){
    char* p1 = (char*) b1;
    char* p2 = (char*) b2;
    char* p3 = (char*) b3;
    char* p4 = (char*) b4;

    if(0 <= n && n < size*1) return (p1 + n);
    if(size*1 <= n && n < size*2) return (p2 + (n - size*1));
    if(size*2 <= n && n < size*3) return (p3 + (n - size*2));
    if(size*3 <= n && n < size*4) return (p4 + (n - size*3));
}

char* r(int n, long long int *b1, long long int *b2, long long int *b3){
    char* p1 = (char*) b1;
    char* p2 = (char*) b2;
    char* p3 = (char*) b3;

    if(0 <= n && n < 8) return p1 + n;
    if(8 <= n && n < 16) return p2 + (n - 8);
    if(16 <= n && n < 24) return p3 + (n - 16);
}