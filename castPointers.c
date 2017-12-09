#include <stdio.h>

int main(){
	int var=0x12345678;
	char *pt;

	pt=(char *)&var;
	printf("v1=%x v2=%x \n", pt[0], pt[1]);
	
	return 0;
}
