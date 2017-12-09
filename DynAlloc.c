#include <stdio.h>
#include <malloc.h>
#include <string.h>

int main(){

	char *pt = malloc(32*sizeof(char));

	strcpy(pt, "Run to the hills");

	printf("phrase : %s \n", pt);
	
	free(pt);



	return 0;
}
