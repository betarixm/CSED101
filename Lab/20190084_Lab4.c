#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void){
	FILE* input;
	FILE* output;

	int ID = 0, mid = 0, fnl = 0, count = 0;
	double  score = 0, sum = 0;
	char grade = 0;

	input = fopen("score.txt", "r");
	output = fopen("report.txt", "w");

	fprintf(output, "=========================\n");
	fprintf(output, "  학번        총점(학점)\n");
	fprintf(output, "=========================\n");

	while(EOF != fscanf(input, "%d %d %d", &ID, &mid, &fnl)){
		score = (double)(mid*0.4) + (double)(fnl*0.6);
		grade = 'A' + (score  < 90) + (score < 80) + (score < 70) + 2 * (score < 60);
		count++;
		sum += score;

		fprintf(output, "%-12d %.1f (%c)\n", ID, score, grade);
	}

    fprintf(output, "=========================\n");
	fprintf(output, "  평균        %.1f\n", sum/count);
    fprintf(output, "=========================\n");

    fclose(input);
    fclose(output);

}
