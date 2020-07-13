


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           PROJET DE COMPILATION DU LANGAGE MINI PYTHON				       //
//						             Réalisé par :					               //
//					     BENSALAH KAWTHAR     et     MERZOUK MOHAMED 			       	       //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct quadruplet quadruplet;
struct quadruplet {char* opr;
				   char* op1;
				   char* op2;
				   char* res;};
				 
struct quadruplet q[70];
int quad (char* a,char* b,char* c,char* d);
void afficherQuad();
char *decode (int i);

char varsTab [50][10];
void ajoutVar (char *var);
void afficheVarsTab();
void generationCode();

struct pile{
    int info;
    struct pile *svt;
};
int sommet_pile (struct pile *tete);
struct pile *empiler (struct pile *p, int x);
struct pile *depiler (struct pile *p);
int pile_vide (struct pile *tete);

void afficherQuadPr();
void optimisation();
void SuppQuads();
void MAJ();
