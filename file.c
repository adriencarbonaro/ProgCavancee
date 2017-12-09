#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

int* initTab(int* tab, int size){
    int i=0;
    tab=(int*)malloc((sizeof(int))*size);
    for(i=0; i<size; i++){
        tab[i] = 3;
    }
    return tab;
}

int main(){
    int* tabNum;
    int i=0;
    int size = 100;
    
    tabNum = initTab(tabNum, size);

    for(i=0; i<size; i++){
        printf("tab %d = %d\n", i, tabNum[i]);
    }

    return 0;
}