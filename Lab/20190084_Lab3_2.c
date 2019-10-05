#include <stdio.h>

int main(void){
	//Using Nested-For
	int i, j;
	
	for(i = 1; i<10; i++){
		for(j = 2; j<10; j++){
			printf("%d * %d = %2d ", j, i, i*j);
		}
		printf("\n");
	}

	printf("\n");

	/////////////
	int a = 1;
	int b = 1;

	for (i=0; i<72; i++){
		int d = (i>0 && i%8==0);
		
		a = a + d;
		b = b - d*(b-1);

		printf("%c", 1+10*d);
		b = b + 1;
		printf("%d * %d = %2d ", i, b, a, a*b);
	}
}
