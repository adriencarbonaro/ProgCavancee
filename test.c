#include <stdio.h>

int operation(int menu)
{
	int a,b;
	printf("Chiffre A : ");
	scanf("%d", &a);
	fflush(stdin);
	printf("\nChriffre B : ");
	scanf("%d", &b);

	if(menu == 1) return a+b;
	if(menu == 2) return a-b;
	if(menu == 3) return a*b;
	if(menu == 4) return a/b;

}

int main(int argc, char const *argv[])
{
	int menu = 0;

	printf("Bonjour choisissez une option :\n");

	printf("1. +\n");
	printf("2. -\n");
	printf("3. *\n");
	printf("4. /\n");

	scanf("%d", &menu);
	fflush(stdin);

	int res = operation(menu);
	printf("Resultat : %d", res);
	printf("\n");
	return 0;
}