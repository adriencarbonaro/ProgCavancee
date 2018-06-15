#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------//
// Structure ChainList donnant un maillon de la liste chainée //
//------------------------------------------------------------//
typedef struct ChainList{
	int val;
	struct ChainList *next;
	struct ChainList *prev;
}ChainList_t;

//-----------------------//
// fonction de base main //
//-----------------------//
int main()
{
	// pt  = pointeur actuel        //
	// pt2 = pointeur case suivante //
	ChainList_t *start, *pt_current, *pt_after;
	int i;

	// Création de la première case //
	start = (ChainList_t *)malloc(sizeof(ChainList_t));
	start -> val = 1;
	start -> next = NULL;
	start -> prev = NULL;

	// On sauvegarde le pointeur de départ //
	pt_current = start;

	// On crée toutes les cases suivantes //
	for(i=2;i<11;i++)
	{
		pt_after = (ChainList_t *)malloc(sizeof(ChainList_t));
		pt_current -> next = pt_after;
		pt_after -> prev = pt_current;
		pt_after -> val = i;
		pt_current = pt_after;
	}

	// On se replace au début de la chaine //
	pt_current = start;

	// On affiche tant qu'il y a des cases //
	while(pt_current != NULL)
	{
		printf("Valeur : %d \n", pt_current->val);
		pt_current = pt_current->next;
	}

	// On se place à la fin (car pt etait plus loin que la dernière case) //
  pt_current = pt_after;

	// On affiche dans l'autre sens //
	while(pt_current != NULL)
	{
		printf("Valeur : %d \n", pt_current->val);
		pt_current = pt_current->prev;
	}

	return 0;
}
