#include <stdio.h>

// int myArray[3]; GLOBAL

int main()
{
	int myArray[3]; // en LOCAL
	int i=0;
	for(i=0;i<3;i++)
	{
		printf("%d\n", myArray[i]);
	} 
return 0;
}
