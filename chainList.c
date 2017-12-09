#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------//
// Structure ChainList donnant un maillon de la liste chainée //
//------------------------------------------------------------//
struct ChainList{
	int val;
	struct ChainList *next;
	struct ChainList *prev;
};

//-----------------------//
// fonction de base main //
//-----------------------//
int main()
{
	// pt  = pointeur actuel        //
	// pt2 = pointeur case suivante //
	struct ChainList *start, *pt, *pt2;
	int i;

	// Création de la première case //
	start = (struct ChainList *)malloc(sizeof(struct ChainList));
	start -> val = 1;
	start -> next = NULL;
	start -> prev = NULL;

	// On sauvegarde le pointeur de départ //
	pt = start;

	// On crée toutes les cases suivantes //
	for(i=2;i<11;i++)
	{
		pt2 = (struct ChainList *)malloc(sizeof(struct ChainList));
		pt -> next = pt2;
		pt2 -> prev = pt;
		pt2 -> val = i;
		pt = pt2;
	}

	// On se replace au début de la chaine //
	pt = start;

	// On affiche tant qu'il y a des cases //
	while(pt != NULL)
	{
		printf("Valeur : %d \n", pt->val);
		pt = pt->next;
	}

	// On se place à la fin (car pt etait plus loin que la dernière case) //
  pt = pt2;

	// On affiche dans l'autre sens //
	while(pt != NULL)
	{
		printf("Valeur : %d \n", pt->val);
		pt = pt->prev;
	}

	return 0;
}
