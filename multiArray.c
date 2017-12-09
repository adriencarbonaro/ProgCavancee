#include <stdio.h>
#include <string.h>

int main()
{
	int matrix[3][2];
	int i=0; int j=0;
	for(i=0;i<6;i++)
	{
		matrix[0][i]=i;
	}

	/*printf("%d\n", matrix[0][0]);
	printf("%d\n", matrix[0][1]);
	printf("%d\n", matrix[1][0]);
	printf("%d\n", matrix[1][1]);
	printf("%d\n", matrix[2][0]);
	printf("%d\n", matrix[2][1]);
	*/

	for(i=0;i<3;i++)
	{
		for(j=0;j<2;j++)
		{
			printf("%d\n", matrix[i][j]);
		}
	}
	return 0;
}
