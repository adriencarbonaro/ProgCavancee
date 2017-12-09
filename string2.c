#include <string.h>
#include <stdio.h>

int main()
{
	char string1[]="DUPOND";
	char string2[]="DUPONA";
	int i=0;
	for(i=0;i<strlen(string1);i++)
	{
		if(string1[i] != string2[i])
		{
			printf("%d\n", i);
		}

	}
	return 0;
}
