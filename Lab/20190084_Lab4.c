#include <stdio.h>

int main(void){
	FILE* input;
	FILE* output;

	int ID = 0, mid = 0, fnl = 0, count = 0, sum = 0;
	double  score = 0;
	char grade = 'A';
	
	input = fopen("score.txt", "r");
	output = fopen("report.txt", "w");
	
	fprintf(output, "=========================\n");
	fprintf(output, "  학번         총점 (학점)\n");
	fprintf(output, "=========================\n");

	while(EOF != fscanf(input, "%d %d %d", &ID, &mid, &fnl)){
		score = (double)(mid*0.4) + (double)(fnl*0.6);
		grade = grade + (score  < 90) + (score < 80) + (score < 70) + 2 * (score < 60);
		count++;
		sum += score;

		fprintf(output, "%-13d %.2f (%c)\n\n", ID, score, grade);
	}

}
