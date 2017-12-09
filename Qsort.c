#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE_TABLEAU 100

void swap(int array[], int a, int b) {
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}
    
void quickSort(int* array, int start, int end) {
    int left = start-1;
    int right = end+1;
    const int bias = array[start];
    /* if array length is null, nothing to do */
    if(start >= end) return;
    /* else, we read the array, one time from right
    to left and one time from left to right to search
    for element which are on the wrong place.
    Once found, we swap them. When right index
    reaches left index we stop.*/
    
    while(1) {
        do right--; while(array[right] > bias);
        do left++; while(array[left] < bias);
        if(left < right) swap(array, left, right);
        else break;
    }
    
    /* Now, all elements lesser than bias are on the
    left side and all elements greater than bias are
    on the right side. Thus, we have 2 groups to
    sort. We launch quicksort on these 2 groups!
    That’s what we call recursivity ! */
    quickSort(array, start, right);
    quickSort(array, right+1, end);
}

int* initTab(int* tab, int size){
    int i=0;
    tab=(int*)malloc((sizeof(int))*size);
    for(i=0; i<size; i++){
        tab[i] = rand()%TAILLE_TABLEAU;
    }
    return tab;
}

int main(){
    int* tabNum;
    int i=0;
    int size = TAILLE_TABLEAU;
    srand(time(NULL));
    
    tabNum = initTab(tabNum, size);

   
    quickSort(tabNum, 0, size);

    for(i=0; i<size; i++){
        printf("tab %d = %d\n", i, tabNum[i]);
    }

    return 0;
}

