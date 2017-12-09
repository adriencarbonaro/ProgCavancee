//-------------------------------------------------------------------//
// pThread3.c - 6 nov. 2017
//-------------------------------------------------------------------//
// Création de 10 threads fils dans le thread père
// rangés dans un tableau de threads
//-------------------------------------------------------------------//
// Ajout de la fonction join qui permet d'attendre
// la fin des threads pour finir le père
//-------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int var = 0;

// Threads fils
pthread_t pthread[10];

// Fonction associée au threads fils
void *pthreadFunc(void *par){
    /* Pour enlever le warning */
    (void) par;
    int j;
    int i = (int)par;
    printf("i= %d\n", i);
    
    for(j=0; j<1000000; j++){
        var+=1;
    }
    return NULL;
}

int main(void)
{
    pthread_t thread1;
    int i;

    printf("Avant la création du thread.\n");

    for(i=0; i<10; i++){
        if (pthread_create(&pthread[i], NULL, pthreadFunc, (void *)i)) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }


    for(i=0; i<10; i++){
        // Cette fonction permet d'attendre la fin des threads fils pour finir le père
        // Elle permet aussi aux fils de s'ordonner pendant leur affichage
        if (pthread_join(pthread[i], NULL)) {
            perror("pthread_join");
            return EXIT_FAILURE;
        }
    } 

    printf("Après la création du thread.\n");
    printf("var = %d\n", var);

    return EXIT_SUCCESS;
}
