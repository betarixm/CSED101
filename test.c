#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct ele
{
    int data;
    struct ele* next;
} Element;
typedef struct set
{
    char set_name[21];
    int set_size;
    Element* ele_head;
    struct set* next;
} Set;

void setfunc(Set* head, FILE* infile);
void showfunc(Set* head, FILE* infile);
int is_element(Set* head, FILE* infile);
void addfunc(Set* head, FILE* infile);
void addprofunc(Set* ntemp, int nn);
void popfunc(Set* head, FILE* infile);
void clearfunc(Set* head, FILE* infile);
void freeele(Element* etemp);
void quitfunc(Set* head);
void unionfunc(Set* head, FILE* infile);
int check_addSet(Set* head, Set** atemp, Set** btemp, Set** ctemp, char* icn, FILE* infile);
int main(int argc, char** argv)
{
    if (argc < 3)
    {
        Set* hset = (Set*)malloc(sizeof(Set));
        hset->set_size = 0;
        hset->next = NULL;
        char cmd[11] = " ";
        while (strcmp(cmd, "quit") != 0)
        {
            fprintf(stdout, ">> ");
            fscanf(stdin, "%s", cmd);
            if (strcmp(cmd, "set") == 0)
            {
                setfunc(hset, stdin);
            }
            else if (strcmp(cmd, "show") == 0)
            {
                showfunc(hset, stdin);
            }
            else if (strcmp(cmd, "is_element") == 0)
            {
                int isen = is_element(hset, stdin);
                if (isen != -1)
                {
                    fprintf(stdout, "%d\n", isen);
                }
            }
            else if (strcmp(cmd, "add") == 0)
            {
                addfunc(hset, stdin);
            }
            else if (strcmp(cmd, "pop") == 0)
            {
                popfunc(hset, stdin);
            }
            else if (strcmp(cmd, "clear") == 0)
            {
                clearfunc(hset, stdin);
            }
            else if (strcmp(cmd, "union") == 0)
            {
                unionfunc(hset, stdin);
            }
        }
        if (strcmp(cmd, "quit") == 0)
        {
            if (hset->set_size > 0)
            {
                quitfunc(hset->next);
            }
            free(hset);
        }
    }
}


int check_addSet(Set* head, Set** atemp, Set** btemp, Set** ctemp, char* icn, FILE* infile)
{
    char a_name[21] = { 0 };
    char b_name[21] = { 0 };
    char c_name[21] = { 0 };

    fscanf(infile, "%s", a_name);
    fscanf(infile, "%s", b_name);
    fscanf(infile, "%s", c_name);
    *atemp = head;
    while (*atemp != NULL)
    {
        if (strcmp((*atemp)->set_name, a_name) == 0)
        {
            break;
        }
        *atemp = (*atemp)->next;
    }
    if (*atemp == NULL)
    {
        return 0;
    }
    *btemp = head;
    while (*btemp != NULL)
    {
        if (strcmp((*btemp)->set_name, b_name) == 0)
        {
            break;
        }
        *btemp = (*btemp)->next;
    }
    if (*btemp == NULL)
    {
        return 0;
    }
    *ctemp = head;
    while ((*ctemp)->next != NULL)
    {
        if (strcmp((*ctemp)->set_name, c_name) == 0)
        {
            return 0;
        }
        *ctemp = (*ctemp)->next;
    }
    return 1;

}

void unionfunc(Set* head, FILE* infile)
{
    char icn[21];
    Set* atemp = head;
    Set* btemp = head;
    Set* ctemp = head;
    int i, j;
    if (check_addSet(head, &atemp, &btemp, &ctemp, icn, infile))
    {
        Element* aetemp = atemp->ele_head->next;
        Element* betemp = btemp->ele_head->next;
        Set* cset = (Set*)malloc(sizeof(Set));
        ctemp->next = cset;
        cset->next = NULL;
        strcpy(cset->set_name, icn);
        Element* ehead = (Element*)malloc(sizeof(Element));
        ehead->data = 0;
        ehead->next = NULL;
        cset->ele_head = ehead;
        for (i = 0; i < atemp->set_size; i++)
        {
            addprofunc(cset, aetemp->data);
            aetemp = aetemp->next;
        }
        for (j = 0; j < btemp->set_size; j++)
        {
            addprofunc(cset, betemp->data);
            betemp = betemp->next;
        }
        cset->set_size = ehead->data;
        head->set_size++;
    }
}

