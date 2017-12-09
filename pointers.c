#include <stdio.h>

int main(){
	int val1 = 5;
	int *pt1;

	pt1=&val1;
	printf("%d", *pt1);
	return 0;
}
