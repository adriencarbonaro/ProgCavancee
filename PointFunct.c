#include <stdio.h>
#include <stdlib.h>

struct object{
        struct object *next;
        void (*display)(struct object *object);
};

struct rectangle{
        struct object *next;
        void (*display)(struct rectangle *pt);
        int left, top, width, height;
};

struct circle{
        struct object *next;
        void (*display)(struct circle *pt);
        int radius;
};

struct triangle{
        struct object *next;
        void (*display)(struct triangle *pt);
        int x1,y1,x2,y2,x3,y3;
};

void displayTriangle(struct triangle *pt){
        printf("TRIANGLE\n");
        printf("x1 = %d\n",pt->x1);
        printf("y1 = %d\n",pt->y1);
        printf("x2 = %d\n",pt->x2);
        printf("y2 = %d\n",pt->y2);
        printf("x3 = %d\n",pt->x3);
        printf("y3 = %d\n\n",pt->y3);
}
    
void displayCircle(struct circle *pt){
        printf("CERCLE\n");
        printf("radius = %d\n\n",pt->radius);
}

void displayRectangle(struct rectangle *pt){
        printf("RECTANGLE\n");
        printf("left = %d\n",pt->left);
        printf("top = %d\n",pt->top);
        printf("width = %d\n",pt->width);
        printf("height = %d\n\n",pt->height);
}

struct circle *createCircle(int newRadius){
    struct circle *c;
    c=(struct circle *)malloc(sizeof(struct circle));
    c->next=NULL;
    c->radius=newRadius;
    c->display=displayCircle;
    return c;
}

struct rectangle *createRectangle(int newLeft, int newTop, int newWidth, int newHeight){
    struct rectangle *r;
    r=(struct rectangle *)malloc(sizeof(struct rectangle));
    r->next=NULL;
    r->left=newLeft;
    r->top=newTop;
    r->width=newWidth;
    r->height=newHeight;
    r->display=displayRectangle;
    return r;
}

struct triangle *createTriangle(int newx1, int newy1, int newx2, int newy2, int newx3, int newy3){
    struct triangle *t;
    t=(struct triangle *)malloc(sizeof(struct triangle));
    t->next=NULL;
    t->x1=newx1;
    t->y1=newy1;
    t->x2=newx2;
    t->y2=newy2;
    t->x3=newx3;
    t->y3=newy3;
    t->display=displayTriangle;
    return t;
}

void addObject(struct object *newObj, struct object* current){
        // On parcourt la liste pour arriver au dernier élément
        while(current->next != NULL){
                current=current->next;
        }
        current->next = newObj;
}

void displayList(struct object* current){
        current = current->next;
        while(current!=NULL){
                current->display(current);
                current=current->next;
        }
}

int main(){
        struct object *start, *pt;
        struct rectangle *r;
        struct circle *c, *c2;
        struct triangle *t;

        start = (struct object *)malloc(sizeof(struct object));
        start->next = NULL;
        pt=start;
        
        r = createRectangle(1,2,3,4);
        c = createCircle(10);
        t = createTriangle(5,6,7,8,9,10);
        c2 = createCircle(55);

        addObject((struct object*)c, pt);
        addObject((struct object*)t, pt);
        addObject((struct object*)c2, pt);
        addObject((struct object*)r, pt);
        displayList(pt);
}