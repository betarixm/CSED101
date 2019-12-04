#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

int main(int argc, char *argv[]) {
    char cmd[100] = "";
    Set baseSet;
    char c = 0;
    char setName_1[31] = "";
    char setName_2[31] = "";
    char resultSetName[31] = "";
    int target = 0;
    initSet(&baseSet);
    FILE *fp = fopen(argv[1], "r");

    if (fp == NULL) {
        fp = stdin;
    }

    while (1) {
        strcpy(setName_1, "");
        (fp == stdin) && (printf(">> "));
        fscanf(fp, "%s", cmd);
        if (strcmp(cmd, "set") == 0) {
            addSet(&baseSet, fp);
        } else if (strcmp(cmd, "show") == 0) {
            if ((fp != stdin && fgetc(fp) == '\n') || (fp == stdin && getchar() == '\n')) strcpy(setName_1, "");
            else fscanf(fp, "%s", setName_1);

            showSet(&baseSet, setName_1);
        } else if (strcmp(cmd, "is_element") == 0) {
            fscanf(fp, "%s %d", setName_1, &target);
            printf("%d\n", isElementIn(&baseSet, setName_1, target));
        } else if (strcmp(cmd, "add") == 0) {
            fscanf(fp, "%s %d", setName_1, &target);
            addElementIn(&baseSet, setName_1, target);
        } else if (strcmp(cmd, "pop") == 0) {
            fscanf(fp, "%s %d", setName_1, &target);
            popElementIn(&baseSet, setName_1, target);
        } else if (strcmp(cmd, "clear") == 0) {
            fscanf(fp, "%s", setName_1);
            clearSet(&baseSet, setName_1);
        } else if (strcmp(cmd, "union") == 0) {
            fscanf(fp, "%s %s %s", setName_1, setName_2, resultSetName);
            setUnion(&baseSet, setName_1, setName_2, resultSetName);
        } else if (strcmp(cmd, "intersection") == 0) {
            fscanf(fp, "%s %s %s", setName_1, setName_2, resultSetName);
            setInter(&baseSet, setName_1, setName_2, resultSetName);
        } else if (strcmp(cmd, "difference") == 0){
            fscanf(fp, "%s %s %s", setName_1, setName_2, resultSetName);
            setDiff(&baseSet, setName_1, setName_2, resultSetName);

        } else if (strcmp(cmd, "quit") == 0) {
            break;
        }
        while ((strcmp(cmd, "show") != 0) && ((fp == stdin) && (getchar() != '\n')));
        if (fp != stdin && fscanf(fp, "%c", &c) == EOF) {
            fp = stdin;
        }
    }

    return 0;
}