//-------------------------------------------------------------------//
// pThreads2.c - 6 nov. 2017
//-------------------------------------------------------------------//
// Création de 10 threads fils dans le thread père
// rangés dans un tableau de threads
//-------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// thread fils
pthread_t pthread[10];

// fonction associée au thread fils
void *pthreadFunc(void *par){
    /* Pour enlever le warning */
    (void) par;
    int i = (int)par;
    printf("i= %d\n", i);
    return NULL;
}

// main = thread père
int main(void)
{
    // ret pour vérifier qu'il n'y a pas d'erreur
    int ret, i;
    
    for(i=0; i<10; i++){
        // on crée le thread fils
        ret = pthread_create(&pthread[i], NULL, pthreadFunc, (void *) i);

        // on check les erreurs
        if(ret != 0)
        {
            perror("pthread create");
        }


    } 

    // On empèche le père de "mourir" pour que le fils aie le temps d'afficher son résultat à l'écran
    sleep(1);
}