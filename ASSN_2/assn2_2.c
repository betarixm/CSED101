#define _CRT_SECURE_NO_WARNINGS
#define L(x) *(l(x, &l1, &l2, &l3, &l4, sizeof(l1)))

#include <stdio.h>

void fileLoader(int *len, long long int *l1, long long int *l2, long long int *l3, long long int *l4, int size);

char *l(int n, long long int *b1, long long int *b2, long long int *b3, long long int *b4, int size);

int encode(char c, int prevPoint);

int main(void) {
    long long int l1 = 0, l2 = 0, l3 = 0, l4 = 0;
    int len = 0, esp = 0, rsp = 0, res = 0;

    fileLoader(&len, &l1, &l2, &l3, &l4, sizeof(l1));

    while (esp <= len) {
        char isStrke = (L(esp) == 'S');
        char isSplit = (L(esp + 1) == 'P');
        char notLast = (rsp != 9);

        int p0 = encode(L(esp), L(esp - 1));
        int p1 = encode(L(esp + 1), p0);
        int p2 = encode(L(esp + 2), p1);

        if (isStrke) {
            res += (p0 + notLast * (p1 + p2));
        } else if (isSplit) {
            res += (p0 + p1 + notLast * (p2));
        } else {
            res += (p0 + p1);
        }

        rsp += 1 * notLast;
        esp += 2 - isStrke;
    }

    printf("TOTAL SCORE: %d\n", res);

    return 0;

}

void fileLoader(int *len, long long int *l1, long long int *l2, long long int *l3, long long int *l4, int size) {
    FILE *fp = fopen("/Users/beta/Desktop/POSTECH/2019/CSED101/ASSN/ASSN_2/Example2.txt", "r");
    while (EOF != fscanf(fp, "%c", l(*len, l1, l2, l3, l4, size))) {
        printf("%c", *(l(*len, l1, l2, l3, l4, size)));
        *len = *len + 1;
    }
    fclose(fp);
    printf("\n");
}

int encode(char c, int prevPoint) {
    return (c == '-') ? (0) : ((c == 'S') ? (10) : ((c == 'P') ? (10 - prevPoint) : ((c == 0) ? (0) : (c - 48))));
}

char *l(int n, long long int *b1, long long int *b2, long long int *b3, long long int *b4, int size) {
    char *p1 = (char *) b1;
    char *p2 = (char *) b2;
    char *p3 = (char *) b3;
    char *p4 = (char *) b4;

    if (0 <= n && n < size * 1) return (p1 + n);
    if (size * 1 <= n && n < size * 2) return (p2 + (n - size * 1));
    if (size * 2 <= n && n < size * 3) return (p3 + (n - size * 2));
    if (size * 3 <= n && n < size * 4) return (p4 + (n - size * 3));
}