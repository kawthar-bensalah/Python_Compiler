%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#include "projet.tab.h"
int dernier=0,courant=0,courant2=0,nb1=0,nb2=0,e=0,nb_saut=0;
extern YYSTYPE yylval;
int colone=0;
int ligne=1;
FILE *f;
extern FILE* yyout;
int x=0;
char ch;
%}

lettreMaj [A-Z]
lettre [a-z]
chiffre [0-9]
Entier ([0-9]+)|"(-"[0-9]+")"
idf {lettreMaj}({lettre}|{chiffre})*
saut "\n"+                     
blanc [ ]+
tabul [\t]*
comment "#"(.*)

%%

"int" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng; return INT;}}
"if" { if(x==1) {e=0; nb1++; fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng; return IF;}}
"else" { if(x==1) {e=1;nb1++; fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng; return ELSE;}}
"elif" { if(x==1) {e=1;nb1++; fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng; return ELIF;}}
"=" {if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng; return eg;}}
"+" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng; return add;}}               // yyval.nom=strdup(strdup(yytext)) 
"-" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng; return sub;}}
"*" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return mul;}}
"/" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else { colone+=yyleng ;return divs;}}
"," { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return virg;}}
";" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return pvirg;}}
">" {  if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return sup;}}
"<" {  if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else { colone+=yyleng ;return inf;}}
">=" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return supeg;}}
"<=" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else { colone+=yyleng ;return infeg;}}
"!" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return no;}}
"==" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return id;}}
"!=" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return dif;}}
"&" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return and;}}
"|" {if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return or;}}
"(" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return paro;}}
")" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return parf;}}
":" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return pp;}}
"[" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return acco;}}
"]" { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {colone+=yyleng ;return accf;}}

{Entier} { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} 
                else {
                    colone+=yyleng;
 
                    char en[10], en1[10]="";
                    strcpy(en,strdup(yytext)); int i=1, j=0;
                    if(en[0]=='(') while(en[i]!=')') { en1[j]=en[i]; j++;i++;}
                        else strcpy(en1,strdup(en));

                    if(atoi(strdup(en1))<-32768||atoi(strdup(en1))>32767) {
	                    printf(" lexical error  ->  ligne: %d colone: %d  \"%s\"  <nombre entré a  depassé>\n",ligne,colone,strdup(yytext));
                        exit(0);
                    }
                    else 
	                    { colone+=yyleng; strcpy(yylval.chaine,strdup(strdup(en1))); return Entier;}
                }
}

{idf} {
 if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {  
             if (yyleng>8) { 
                      printf(" lexical error  ->  ligne: %d colone: %d  \"%s\"  <nombre de caractere depassé>\n",ligne,colone,strdup(yytext));
                      exit(0);
             }
             else {colone+=yyleng;strcpy(yylval.chaine,strdup(strdup(yytext))); return idf;}}}


{saut} {
if(x==1){ 
if((nb2<nb1) )
{ 
 for(int i=0;i<(nb1-nb2);i++)
{
   fprintf(yyout,"\n)"); 
} 
 nb2=0; nb1=0;
}
nb2=0; nb1=0;  courant=0;
fprintf(yyout,"%s",yytext);
}
else {colone=0; ligne=ligne+yyleng; return ST; }
}

{saut}+{tabul} {
if(x==1) {
dernier=courant;
courant=yyleng;
//fprintf(yyout,"%d   %d ",dernier,courant);
if(courant<dernier) 
{
   for(int i=0;i<(dernier-courant);i++)
{
   fprintf(yyout,"\n)");  nb2++;
}
}

fprintf(yyout,"%s", strdup(yytext));
}
else {
//printf("Stab => ligne : %d\n",ligne);
nb_saut=0;
char *y="";
y=yytext;
int i=0;
while(i<strlen(y)) {
if(y[i]=='\n')
nb_saut++;
i++; 
}
colone=(strlen(y)-nb_saut)*4;
ligne=ligne+nb_saut;
 i=0;
courant2 =yyleng-nb_saut; return tabul;
}

}



{blanc} {if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else {   colone+=yyleng;}}

{comment} {if(x!=1) { colone+=yyleng; }}

. { if(x==1) {fprintf(yyout,"%s", strdup(yytext));} else { colone+=yyleng;  printf("Erreur lexicale : %s  ->  Ligne : %d Colonne : %d \n",strdup(yytext),ligne,colone);exit(0);}}

%%





