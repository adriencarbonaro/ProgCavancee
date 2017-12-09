#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ChainList{
	char* val;
	struct ChainList *next;
	struct ChainList *prev;
};

void addWord(struct ChainList *liste){
    char* mot;
    printf("Mot : ");
    scanf("%s", mot);
    // Création de la première case
    liste -> val = mot;
    liste -> next = NULL;
    liste -> prev = NULL;
}

void debugTree(struct ChainList *liste){
    printf("Valeur : %s \n", liste->val);
}

void choix(){
    int choix;
    struct ChainList *start;
    start = (struct ChainList *)malloc(sizeof(struct ChainList));

    printf("1. Ajouter un mot\n2.Afficher mots");
    scanf("%d", &choix);

    switch(choix){
        case 1: 
            addWord(start);
        break;
        case 2: debugTree(start);
        
        break;
    }
}

int main()
{
    while(q=0){
        choix();
    }


	return 0;
}