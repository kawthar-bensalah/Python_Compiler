%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "quad.h"
extern FILE* yyin ;
extern FILE* yyout;
extern FILE* f;
extern int ligne;
extern int colone;
int cpt=0,cond=0;
extern int courant2;
extern int nb_saut;
extern int x;
int nTemp=1, op=0, Qc, sauvBZ, sauvBR; char tempC[12]=""; char tempQc[12]=""; 
struct pile *p1, *p2;
int yyerror( );
int yylex();
%}


%union
{
char chaine[10];
struct {int type ; char* res;}NT;
}

/*************************Déclaration de priorités et de types ***********************************/

%type<NT> expr 
%type<NT> valeur
%type<chaine> paro
%type<chaine> parf
%left add sub 
%left mul divs
%left paro parf


/**************************Déclaration des entités lexicales *************************************/
%token INT IF ELSE ELIF eg add sub mul divs virg pvirg sup inf supeg infeg acco accf no id dif and or paro parf pp tabul ST 
%token <chaine>Entier <chaine>idf


/*******************************L'axiome de la grammaire *****************************************/
%start S
/***************************************La grammaire *********************************************/
%%
S : Code;

Code: Inst 
    | ST Inst
    ;


//liste déclaration
Decl : variable ST | variable ST Inst;

//déclaration variables
variable: INT listeidf;

listeidf: idf {ajoutVar($1);}
        | idf virg listeidf {ajoutVar($1);}
        | aff 
        | aff virg listeidf;

//affectation 
aff: idf eg expr {Qc = quad ("=",$3.res,"",$1); ajoutVar($1);};

valeur: Entier { $$.res=$1;}
      | idf { $$.res=strdup($1);}
      ;

/////////////////////////////////////////////////////////////

//instructions 
Inst: suitexp | Decl | if;

//suite instructions
suitexp: aff ST | aff ST Inst;


//expression arithmétique
expr: expr add expr 
    { sprintf(tempC,"t%d",nTemp);
      $$.res=strdup(tempC);nTemp++;
      tempC[0]='\0';
      Qc = quad ("+",$1.res,$3.res,$$.res);
    }
   |expr sub expr
    { sprintf(tempC,"t%d",nTemp);
      $$.res=strdup(tempC);nTemp++;
      tempC[0]='\0';
      Qc = quad ("-",$1.res,$3.res,$$.res);
    }
   |expr mul expr 
    { sprintf(tempC,"t%d",nTemp);
      $$.res=strdup(tempC);nTemp++;
      tempC[0]='\0';
      Qc = quad ("*",$1.res,$3.res,$$.res);
    }
   |expr divs expr
    { sprintf(tempC,"t%d",nTemp);
      $$.res=strdup(tempC);nTemp++;
      tempC[0]='\0';
      Qc = quad ("/",$1.res,$3.res,$$.res);
    }
    | valeur
    | paro expr parf {$$.res=strdup($2.res);}
    ; 

//////////////////////////////////////////////////////////////

//expression de condition
cdt: C | paro C parf | no paro C parf;

C: expr opC expr { //debut du If
                   Qc = quad (decode(op),"",$1.res,$3.res);
                   if((Qc-1)==1) p1=empiler(p1,-1); 
                   else p1 = empiler(p1,Qc-1); 
                   Qc++;};

opC: sup {op=1;}| supeg {op=2;}| inf {op=3;}| infeg {op=4;}| id {op=5;}| dif{op=6;};

//instruction if  

if : I | I Inst;

I : instIf { //le cas du If avec Else
             Qc = quad ("BR","","",""); 
             p2 = empiler(p2, Qc-1);
             Qc++;
             sauvBZ = sommet_pile(p1); //printf(" p1 vide : %d\n sommet p1 :%d\n",pile_vide(p1), sommet_pile(p1));
             if(sauvBZ==-1) sauvBZ=1;
             p1 = depiler(p1); //printf("%d ",sauvBZ);
             sprintf(tempQc,"%d",Qc); 
             strcpy(q[sauvBZ].op1,tempQc);} else 
 
   | instIf {//le cas du If sans Else 
             Qc++; 
             sauvBZ = sommet_pile(p1); 
             p1 = depiler(p1);
             sprintf(tempQc,"%d",Qc);
             strcpy(q[sauvBZ].op1,tempQc);
            };

if2 : I2 B;
B : Instiff | ;
I2 :  instIf A;
Instiff : suitexpif | if2 | Decll;
suitexpif : aff  t Instiff | aff ST;
Decll : variable t Instiff | variable ST;

StTab : ST  | tabul;
instIf : IF paro cdt parf pp {cpt++;} t Instiff parf StTab {cpt--; ligne--;}

t: tabul {if(courant2!=cpt) {/*printf("taille = %d cpt= %d \n",courant2,cpt);*/ yyerror("erreur tabulation"); cond=1;} } ;

//{printf("taille = %d cpt= %d \n",taille,cpt); if(taille !=cpt) yyerror("tabul");};

else : ELSE pp {cpt++;} t Instiff parf StTab{cpt--; ligne--;
                                            sauvBR = sommet_pile(p2);
                                            p2 = depiler(p2); 
                                            sprintf(tempQc,"%d",Qc+1); 
                                            strcpy(q[sauvBR].op1,tempQc);}
     | elif { //le Elif avec Else
             sauvBR = sommet_pile(p2);
             p2 = depiler(p2); 
             sprintf(tempQc,"%d",Qc+1);
             strcpy(q[sauvBR].op1,tempQc);

             Qc = quad ("BR","","","");
             p2 = empiler(p2, Qc-1);
             Qc++;

             sauvBZ = sommet_pile(p1);
             p1 = depiler(p1);
             sprintf(tempQc,"%d",Qc);
             strcpy(q[sauvBZ].op1,tempQc);
             } else

     | elif { //le Elif sans Else
             sauvBR = sommet_pile(p2);
             p2 = depiler(p2); 
             sprintf(tempQc,"%d",Qc+1);
             strcpy(q[sauvBR].op1,tempQc);

             Qc++;

             sauvBZ = sommet_pile(p1);
             p1 = depiler(p1);
             sprintf(tempQc,"%d",Qc);
             strcpy(q[sauvBZ].op1,tempQc);
             };

A : { //le cas du If avec Else
        Qc = quad ("BR","","",""); 
        p2 = empiler(p2, Qc-1);
        Qc++;
             
        sauvBZ = sommet_pile(p1);
        p1 = depiler(p1); 
        sprintf(tempQc,"%d",Qc);
        strcpy(q[sauvBZ].op1,tempQc);} else 

    | {//le cas du If sans Else 
       //Qc++;
       sauvBZ = sommet_pile(p1); 
       p1 = depiler(p1);
       sprintf(tempQc,"%d",Qc+1);
       strcpy(q[sauvBZ].op1,tempQc);
       };

elif : ELIF paro cdt parf pp {cpt++;} t Instiff parf StTab{cpt--; ligne--;};

%%

int yyerror(char* msg){
printf(" %s  ->  Ligne: %d \n",msg,ligne);
exit(0);
return 1;
}                                   
/************************************Le code principal*******************************************/
    
int yywrap(void)
{
return 1;
}
                                
int main(){

x=1;
FILE *in1=fopen("input.txt","r");  
FILE *in2=fopen("input2.txt","w");
char buf;
buf=fgetc(in1);
while(buf!=EOF){
    if(buf=='#') { fputc('\n',in2); while(buf!='\n') buf=fgetc(in1); }
        else fputc(buf,in2); buf=fgetc(in1);
}
fclose(in1);
fclose(in2);
yyin=fopen("input2.txt","r");
yyout=fopen("Output.txt","w");
yylex();
fclose(yyin);
fclose(yyout);

yyin=fopen("Output.txt","r");
x=2;
yyparse();
afficherQuad();
optimisation();
MAJ();
SuppQuads();
afficherQuadPr();
generationCode();
fclose(yyin);

return 0;
}
