#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

int main(int argc, char* argv[]){
    char cmd[100] = "";
    Set baseSet;
    char c = 0;
    char setName[30] = "";
    int target = 0;
    initSet(&baseSet);
    FILE* fp = fopen(argv[1], "r");

    if(fp == NULL){
        fp = stdin;
    }

    while(1){
        strcpy(setName, "");
        (fp == stdin) && (printf(">> "));
        fscanf(fp, "%s", cmd);
        if(strcmp(cmd, "set") == 0){
            addSet(&baseSet, fp);
        } else if(strcmp(cmd, "show") == 0){
            if((fp != stdin && fgetc(fp)=='\n') || (fp == stdin && getchar() == '\n')) strcpy(setName, "");
            else fscanf(fp, "%s", setName);
            showSet(&baseSet, setName);

        } else if(strcmp(cmd, "is_element") == 0) {
            fscanf(fp, "%s %d", setName, &target);
            printf("%d\n", isElementIn(&baseSet, setName, target));
        } else if(strcmp(cmd, "add") == 0) {
            fscanf(fp, "%s %d", setName, &target);
            addElementIn(&baseSet, setName, target);
        } else if(strcmp(cmd, "pop") == 0){
            fscanf(fp, "%s %d", setName, &target);
            popElementIn(&baseSet, setName, target);
        } else if(strcmp(cmd, "clear") == 0) {
            fscanf(fp, "%s", setName);
            clearSet(&baseSet, setName);
        } else if (strcmp(cmd, "quit") == 0) {
            break;
        }
        while((strcmp(cmd, "show") != 0) && ((fp == stdin) && (getchar() != '\n')));
        if(fp != stdin && fscanf(fp, "%c", &c) == EOF){
            fp = stdin;
        }
    }

    return 0;
}