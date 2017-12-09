#include <stdio.h>

int main(){
	long var=0x12345678;
	char *pt;

	pt=(char *)&var;
	printf("v1=%x v2=%x v3=%x v4=%x \n", pt[0], pt[1], pt[2], pt[3]);
	
	return 0;
}
