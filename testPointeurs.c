#include <stdio.h>

// Fonction permettant de changer la valeur de a en memoire grâce au pointeur
void changeVar(int* val, int newA){
    *val = newA;
}

int main(){
    int v = 3;
    int* a;
    a = &v;

    printf("a = %d\n", *a);
    printf("addresse de a = %p\n", a);
    
    changeVar(a, 5);

    printf("a = %d\n", *a);
    printf("v = %d\n", v);
}
