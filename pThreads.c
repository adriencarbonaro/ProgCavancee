//-------------------------------------------------------------------//
// pThread.c - 6 nov. 2017
//-------------------------------------------------------------------//
// Création d'un thread fils dans le thread père
// On affiche le resultat de la fonction pthreadFunc qui affiche i
// Puis on finit le père et le programme
//-------------------------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// thread fils
pthread_t pthread1;

// fonction associée au thread fils
void *pthreadFunc(void *par){
    int i = (int)par;
    printf("i= %d\n", i);
    return NULL;
}

// main = thread père
int main(void)
{
    // ret pour vérifier qu'il n'y a pas d'erreur
    int ret, i=2;
    
    // on crée le thread fils
    ret = pthread_create(&pthread1, NULL, pthreadFunc, (void *) i);
    
    // on check les erreurs
    if(ret != 0)
    {
        printf(stderr, "%s", strerror(ret));
    }

    // On empèche le père de "mourir" pour que le fils aie le temps d'afficher son résultat à l'écran
    sleep(10);
}