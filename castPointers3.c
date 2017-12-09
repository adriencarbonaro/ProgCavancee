#include <stdio.h>

int main(){
	float var=1.0;
	float var2=-1.0;
	float var3=3.0;

	char *pt;
	char *pt2;
	char *pt3;

	pt=(char *)&var;
	pt2=(char *)&var2;
	pt3=(char *)&var3;

	printf("v1=%x v2=%x v3=%x v4=%x \n", pt[0], pt[1], pt[2], pt[3]);
	printf("v1=%x v2=%x v3=%x v4=%x \n", pt2[0], pt2[1], pt2[2], pt2[3]);
	printf("v1=%x v2=%x v3=%x v4=%x \n", pt3[0], pt3[1], pt3[2], pt3[3]);
	return 0;
}
