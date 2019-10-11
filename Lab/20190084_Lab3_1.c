#include <stdio.h>

float calc_average(int mid, int fnl);
char calc_grade(float avg);

int main(void){
    int mid = 0;
    int fnl = 0;
    float avg = 0;

    printf("Enter your midterm score: ");
    scanf("%d", &mid);

    printf("Enter your final score: ");
    scanf("%d", &fnl);

    avg = calc_average(mid, fnl);
    printf("Average: %.1f\n", avg);
    printf("Grade: %c\n", calc_grade(avg));

}

float calc_average(int mid, int fnl){
    return ((float)(mid+fnl))/2;
}

char calc_grade(float avg){
    return ((avg >= 90) ? 'A' :
           ((avg >= 80) ? 'B' :
           ((avg >= 70) ? 'C' :
           ((avg >= 60) ? 'D' : 'F'))));
}
