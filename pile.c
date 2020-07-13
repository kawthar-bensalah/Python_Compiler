#include <stdio.h>
#include <malloc.h>
#include "quad.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           PROJET DE COMPILATION DU LANGAGE MINI PYTHON				       //
//						             Réalisé par :					               //
//					     BENSALAH KAWTHAR     et     MERZOUK MOHAMED 			       	       //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int x;
struct pile *tete;

struct pile *init_pile (struct pile *tete){
    return(NULL);
}

int pile_vide (struct pile *tete){
 if(tete==NULL) return 1;
 else return 0;
}

int sommet_pile (struct pile *tete){
    return tete->info;
}

struct pile *empiler (struct pile *p, int x){
    struct pile *p1;
    p1=malloc(sizeof(struct pile));
    p1->info=x;
    p1->svt=p;
    p=p1;
    return p;
}

struct pile *depiler(struct pile *p){
    struct pile *p1;
    x=p->info;
    p1=p;
    p=p->svt;
    free(p1);
    return p;
}
/*
int main(){
    struct pile *p1;

    p1 = empiler(p1,1);
    printf("%d\n",sommet_pile(p1));
    p1 = depiler(p1);
    printf("%d\n",pile_vide(p1));

return 0;
}*/
