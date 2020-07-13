#include "quad.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           PROJET DE COMPILATION DU LANGAGE MINI PYTHON				       //
//						             Réalisé par :					               //
//					     BENSALAH KAWTHAR     et     MERZOUK MOHAMED 			       	       //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Fonction d'jout d'un quadruplet 
int indq=0;
int quad(char* o,char*o1,char*o2,char*r){
    q[indq].opr=strdup(o);
    q[indq].op1=strdup(o1);
    q[indq].op2=strdup(o2);
    q[indq].res=strdup(r);
    indq++;
    return indq;
}

//Fonction d'affichage des quadruplets
void afficherQuad(){
   int i;
   FILE* out;
   out = fopen("quad.txt", "w");
   
    for (i=0;i<indq;i++){
        fprintf (out,"%d-( %s , %s , %s , %s )\n",i+1,q[i].opr,q[i].op1,q[i].op2,q[i].res);
    }
    fclose(out);
}

char *decode (int i){
    switch(i){
        case 1:
            return "BLE";
        break;
        case 2:
            return "BL";
        break;
        case 3:
            return "BGE";
        break;
        case 4:
            return "BG";
        break;
        case 5:
            return "BNE";
        break;
        case 6:
            return "BE";
        break;
    }
}

//Fonction de suppression des quadruplets vides
void SuppQuads(){
    int i,j,h=0;
    for (i=0;i<indq;i++){
        if(((strcmp(q[i].opr," ")==0) || (strcmp(q[i].opr,"=")==0)) && (strcmp(q[i].op1," ")==0) && (strcmp(q[i].op2," ")==0) && (strcmp(q[i].res," ")==0) ){
             
             for (j=i;j<indq-1;j++){
                 q[j]=q[j+1]; 
             }
        indq--;i--;
        }
    }
    i=0;
} 
  
//Creation d'un tableau qui contient les numéros des quads supprimés
int lenQS=0;
int QuadSupprimes [100];

void rempQuadSupprimes(){
    int k=0,i;
    for (i=0;i<indq;i++){
        if (((strcmp(q[i].opr," ")==0) || (strcmp(q[i].opr,"=")==0)) && (strcmp(q[i].op1," ")==0) && (strcmp(q[i].op2," ")==0) && (strcmp(q[i].res," ")==0) ){
               QuadSupprimes[k]=i; k++; lenQS++; 
        }
    }
}

int existInQs (int e){
    int i;    
    for(i=0;i<lenQS;i++){
        if(QuadSupprimes[i]==e) return 1;
    }
return 0;
}

//Fonction de mise à jour des adresses des branchements après optimisation
void MAJ(){
    int i,j;
	//Décrementer les adresses des branchements s'il y a des des quads supprimés a l'intérieur d'un bloc
    for (i=0 ; i<indq; i++){
        if (( q[i].opr[0]=='B') ){
            int s = atoi(strdup(q[i].op1));
            for(j=i;j<s;j++){
                if(existInQs (j)) s--;
            }
            sprintf(q[i].op1, "%d", s);
        }
    }
	//Décrementer les adresses de tous les branchements qui viennent aprés un quad supprimé
    for(i=0;i<lenQS;i++){
        int h = QuadSupprimes[i];
        for (j=h ; j<indq; j++){
            if (( q[j].opr[0]=='B') ){
                 int n = atoi(strdup(q[j].op1));
                 n--;
                 sprintf(q[j].op1, "%d", n);
            }
        }        
    }
    
}

//Fonction est vide qui vérifie si le quadruplet est vide 
 int estVide(int i)
  {

 if(((strcmp(q[i].opr," ")==0) || (strcmp(q[i].opr,"=")==0)) && (strcmp(q[i].op1," ")==0) && (strcmp(q[i].op2," ")==0) && (strcmp(q[i].res," ")==0) )
   return 1;
else return 0;
 }

FILE *optim;

void afficherQuadPr()
{ int i,j=0,k;
optim = fopen("QuadOpt.txt","w");

for (i=0;i<indq;i++){j++;
    fprintf (optim,"%d-( %s , %s , %s , %s )\n",i+1,q[i].opr,q[i].op1,q[i].op2,q[i].res);
}
fclose(optim);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                   Fonction d'optimisation						       //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void optimisation(){
	int i=0,j,k=0,k2,t,p,ind=1,indice1=0,indice2=0,indice3=0;
	char *temp_res;
      char *temp_var;
     int compt=0,indic=0,limite=0,used1=0,used2=0;
while(ind==1)
{

//Propagation de copie  + //Propagation d'expression
	while((i<indq))
         {         
           if(strcmp(q[i].opr,"=")==0)
           { 
		//Vérifier si l'affectation est dans un bloc if
		while(k<i) 
		 {
			if (q[k].opr[0]=='B' )
			 {compt=1; indic=atoi(q[k].op1)-1; } k++; 	
		}
	
              k=0;  
             temp_res=q[i].res;
             temp_var=strdup(q[i].op1);
             for(j=i+1;j<indq;j++)
                { 	 
	//si l'affectation est dans un bloc 'if'     
               if(compt==1)
               {
		//s'arrêter si fin du bloc 
		if(j==indic) break;
		//s'arrêter si debut d'un autre bloc
		if(q[j].opr[0]=='B') break; 
		//pas de propagation si les opérands changent
		if((strcmp(q[j].res,q[i].op1)==0)) break;
		if((strcmp(q[j].res,q[i].op2)==0)) break;
		//Propagation (op2)
             	if((strcmp(q[j].op2,temp_res)==0)) { indice1=1;  q[j].op2=strdup(temp_var);}
		//Propagation (op1)
		if((strcmp(q[j].op1,temp_res)==0)) { indice1=1;  q[j].op1=strdup(temp_var);}
		}
		else				
		{   
	//L'affectation n'est pas dans un bloc 'if'
		int in,limite2=0,n;
	//Pas de propagation si la variable change
		if((strcmp(q[j].res,temp_res)==0)  && q[j].opr[0]!='B')  {used2=1;}
	//pas de propagation si les opérands changent
			if((strcmp(q[j].res,q[i].op1)==0) )  {used1=1; }

			if((strcmp(q[j].res,q[i].op2)==0) )  {used1=1;}
      
	//Sauvegarder l'adresse du dernier branchement pour s'arrêter si on l'atteint
			if((strcmp(q[j].opr,"BR")==0) ) {
			for(int y=j-1;y>0;y--)
			{
			if((q[y].opr[0]=='B') && (strcmp(q[y].opr,"BR")!=0)  )  				{ limite=atoi(q[j].op1)-1; used1=0; break;}

			if((strcmp(q[y].opr,"BR")==0)) {  used1=0; break;}

			}
	//Sauvegarder l'adresse du branchement pour s'arrêter si on l'atteint
			in=atoi(q[j].op1)-1; 

		}

		//Propagation 
	  if((strcmp(q[j].op2,temp_res)==0)) { 
			n=j-1;
		//pour délimiter les blocs 
		while(n>i)
		   {
		     if((strcmp(q[n].opr,"BR")==0) ) 
         		{break;}
		     else { n--;}
		   }
                        if(n==i) n++;
		//Vérifier si il y'a eu un changement 
		for(int limite2=n;limite2<j;limite2++)
		{
		if((strcmp(q[limite2].res,temp_res)==0))  { used1=1;   }
		}
		if(used2==1)
		 {
		  if(used1==0 && j<in && j<limite) 
		    {indice1=1;   				
		     q[j].op2=strdup(temp_var);} 
			}
		   else
		    {
		   if(used1==0 ) {indice1=1;   q[j].op2=strdup(temp_var);} 
		   }
		} 
	//propagation
	if((strcmp(q[j].op1,temp_res)==0))  {
			 n=j-1;
			while(n>i)
			{
	//pour délimiter les blocs 
	        if((strcmp(q[n].opr,"BR")==0) )
			 {break;} 
		    else { n--;}
			}
                     if(n==i) n++;
	//Vérifier si il y'a eu un changement 
		for( limite2=n;limite2<j;limite2++)
		  {
		   if((strcmp(q[limite2].res,temp_res)==0))  { used1=1;  }
			}
		     if(used2==1)
		       {
			if(used1==0 && j<in && j<limite) 
			{indice1=1;   						    				q[j].op1=strdup(temp_var);
			} 
			}
		      else
			{
			if(used1==0 ) {indice1=1;   q[j].op1=strdup(temp_var);} 
			}
			}
		 } 
                }   used1=0;
           }  
	  compt=0; 
           i++;
         }
i=0;

//Elimination des opérations redondantes
	while(i<indq )
      	 { int n;
         char * temp_opr=q[i].opr;
     	 char * temp_op1=q[i].op1;
 	 char *temp_op2=q[i].op2;
 	 char *tempp_res;
	 temp_res=q[i].res;
	 for(j=i+1;j<indq;j++)
	  {
	//Si les opérands changent  :  pas d'élimination
	   if(  (strcmp(q[j].res,temp_op1)==0) || 				       (strcmp(q[j].res,temp_op2)==0)    ) 
    				 {break;}
   	   else 
		{if((strcmp(q[j].opr,temp_opr)==0) && (strcmp(q[j].op1,temp_op1)==0) && (strcmp(q[j].op2,temp_op2)==0) && (strcmp(q[j].op1," ")!=0) 			&& (strcmp(q[j].op2," ")!=0) &&  (strcmp(q[j].opr,"=")!=0) &&  (q[j].opr[0]!='B'))
		{   
		 for(n=j+1;n<indq;n++)
	{      
		 tempp_res=q[j].res;
	       if((strcmp(q[n].op1,tempp_res)==0) )
	      {	      
		    q[n].op1=temp_res;
	      }
		 if((strcmp(q[n].op2,tempp_res)==0) )
		{
		    q[n].op2=temp_res;
		}		
	}
	//Suppression du quad 
 		indice2=1;
    	        q[j].opr=" ";
 		q[j].op1=" ";
		q[j].op2=" ";
		q[j].res=" "; 
        }
}
}	
	  
	 i++;
	}

i=0;


//Simplification algébrique
	while(i<indq)
	 {
   	 if((atoi(q[i].op1)==2) && (strcmp(q[i].opr,"*")==0) )
	  {
	   indice3=1;
 	   q[i].opr="+";
	   q[i].op1=strdup(q[i].op2);
	  }
	 if((atoi(q[i].op2)==2) && (strcmp(q[i].opr,"*")==0)  )
	  {
	  indice3=1;
	  q[i].opr="+"; q[i].op2=strdup(q[i].op1);
	  }
	  i++;
 	 }


//Elimination des expression (+n-n)
   i=0;
	int r;  char *tempRes;
  for(i=1;i<indq-1;i++)
    {
	if( (strcmp(q[i].opr,"+")==0 && (strcmp(q[i+1].opr,"-")==0) || (strcmp(q[i].opr,"-")==0 && (strcmp(q[i+1].opr,"+")==0) )	))		
        { 
	  if  (strcmp(q[i].op1,q[i+1].op1)==0)  
	{  r=i+2;
	//remplacer toutes les occurences du temporaire par la variable 
		while(r<indq)
               { tempRes=strdup(q[i+1].res);
		  if   (strcmp(q[r].res,tempRes)==0 )
		q[r].res=strdup(q[i].op2);
		if   (strcmp(q[r].op1,tempRes)==0 )
		q[r].op1=strdup(q[i].op2);
		if   (strcmp(q[r].op2,tempRes)==0 )
		q[r].op2=strdup(q[i].op2);
		r++;
	       }
	//Suppression des quads 
	    q[i].res=" "; q[i].op1=" "; q[i].op2=" "; q[i].opr=" "; 
            q[i+1].res=" "; q[i+1].op1=" "; q[i+1].op2=" "; q[i+1].opr=" "; 
	}
       if   (strcmp(q[i].op2,q[i+1].op2)==0 )
	{
		 r=i+2;
	//remplacer toutes les occurences du temporaire par la variable 
		while(r<indq)
               {  tempRes=strdup(q[i+1].res);
		  if   (strcmp(q[r].res,tempRes)==0 )
		q[r].res=strdup(q[i].op1);
		if   (strcmp(q[r].op1,tempRes)==0 )
		q[r].op1=strdup(q[i].op1);
		if   (strcmp(q[r].op2,tempRes)==0 )
		q[r].op2=strdup(q[i].op1);
		r++;
	       }
	//Suppression des quads
	    q[i].res=" "; q[i].op1=" "; q[i].op2=" "; q[i].opr=" "; 
            q[i+1].res=" "; q[i+1].op1=" "; q[i+1].op2=" "; q[i+1].opr=" "; 
	}


	   }
	

   } 
 



i=0;
//Elimination code inutile 
    int util=0;
     for(i=0;i<indq;i++)
    {	
    	if(strcmp(q[i].opr,"=")==0 && q[i].res[0]=='t' )
	{ 
   //Vérifier si le quad d'Affectation à un temporaire n'est pas utilisé
		for(j=i+1;j<indq;j++)
		{
			if( (strcmp(q[j].op1,q[i].res)==0) || (strcmp(q[j].op2,q[i].res)==0)  || (strcmp(q[j].res,q[i].res)==0) )
			{	
				util=1;
			}
			
		}      
 //suppression du quad d'Affectation à un temporaire qui n'est pas utilisé
	 if(util==0) { q[i].res=" "; q[i].op1=" "; q[i].op2=" "; q[i].opr=" "; }
	
	
	} 
      util=0;
	
		
    } 



i=0;
        //Aucune modification des quadruplets
	 if((indice1==0) && (indice2==0) && (indice3==0))
		{ ind=0; /*MAJ();*/}
	//Réinitialisation des indices
	indice1=0; indice2=0;indice3=0;
    }
rempQuadSupprimes();
}









/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Fonction de génération du code machine 					       //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int nbre = 0;

int exists(char *var){
    int i = 0, t=0;
    for(i=0; i<nbre; i++){
        if (strcmp(varsTab[i], strdup(var))==0) t=1;
    }
    return t;
}

//tableau qui contient toutes les variables
void ajoutVar (char *var){
    
    if(!exists(var)){
        strcpy(varsTab[nbre], strdup(var));
        nbre++;
    }
}
void afficheVarsTab(){
    int i; 
    for(i=0; i<nbre; i++){
        printf("%s - ", varsTab[i]);
    }
}

//tableau pour stocker l'adresse des branchements pour mettre les étiquettes
int LabelTab [100];

void initLabelTab(){
    int i;
    for (i=0;i<indq;i++){
        LabelTab[i]=0;
    }
}

char *ChiffreOuNn (char *v){
    if (v[0]=='0' || v[0]=='1' || v[0]=='2' || v[0]=='3' || v[0]=='4' || v[0]=='5' || v[0]=='6' || v[0]=='7' || v[0]=='8' || v[0]=='9' || v[0]=='-')
        return v;
    else {
        char a[100]="[";
        strcat(a,v);
        strcat(a,"]");
        
        return strdup(a);
    }
}

int isAnumber(char *v){
    if (v[0]=='0' || v[0]=='1' || v[0]=='2' || v[0]=='3' || v[0]=='4' || v[0]=='5' || v[0]=='6' || v[0]=='7' || v[0]=='8' || v[0]=='9' || v[0]=='-')
        return 1;
    else return 0;
}

FILE* outMC;
void generationCode(){
    int i;
    initLabelTab();

    outMC = fopen("code.asm", "w");
    fprintf (outMC,"TITLE   code.asm: CODE, \n\nPile segment stack ; \ndw  100 dup(?)\nPile ends \n\nDATA segment\n\n"); 
    for(i=0; i<nbre; i++){
        fprintf(outMC,"\t%s  Dw  ?\n", varsTab[i]);
    }
    fprintf (outMC,"\nDATA ends\n\nCODE  segment\n   MAIN:\n\nASSUME CS:CODE, DS:DATA, SS:PILE\n\n;initialisation de la pile \nMov Ax, PILE\nMov SS, AX\nMov SP, Base_Pile\n");

    for (i=0;i<indq;i++){
        
		//mettre une étiquette s'il y en a
        if(LabelTab[i]==1) fprintf (outMC,"\nLabel_%d:\n", i+1);
         
		//cas des expression arithmétiques
        if ( ((q[i].opr)[0]=='+')  || ((q[i].opr)[0]=='-') || ((q[i].opr)[0]=='*') || ((q[i].opr)[0]=='/') )         
        { 

            if( ((q[i].op1)[0]!='t') && ((q[i].op2)[0]!='t') ){ 
            
                if (!(strcmp(q[i].opr, "+")))
                    fprintf (outMC,"\n\tMov Ax, %s\n\tAdd Ax, %s\n", ChiffreOuNn(q[i].op1), ChiffreOuNn(q[i].op2));
                if (!(strcmp(q[i].opr, "-")))
                    fprintf (outMC,"\n\tMov Ax, %s\n\tSub Ax, %s\n", ChiffreOuNn(q[i].op1), ChiffreOuNn(q[i].op2));
                if (!(strcmp(q[i].opr, "*")))
                    fprintf (outMC,"\n\tMov Ax, %s\n\tMul %s\n", ChiffreOuNn(q[i].op1), ChiffreOuNn(q[i].op2));
                if (!(strcmp(q[i].opr, "/")))
                    fprintf (outMC,"\n\tMov Ax, %s\n\tDiv %s\n", ChiffreOuNn(q[i].op1), ChiffreOuNn(q[i].op2));
     
            }
            else {
                  if( ((q[i].op1)[0]=='t') && ((q[i].op2)[0]=='t') ){
            
                         fprintf (outMC,"\n\tPop Bx\n");  
                         fprintf (outMC,"\tPop Ax");  
                         if (!(strcmp(q[i].opr, "+")))
                             fprintf (outMC,"\n\tAdd Ax, Bx\n");
                         if (!(strcmp(q[i].opr, "-")))
                             fprintf (outMC,"\n\tSub Ax, Bx\n");
                         if (!(strcmp(q[i].opr, "*")))
                             fprintf (outMC,"\n\tMul, Bx\n");
                         if (!(strcmp(q[i].opr, "/")))
                             fprintf (outMC,"\n\tDiv, Bx\n");
                  }else {
                        if (((q[i].op2)[0]!='t') ) {
                            fprintf (outMC,"\n\tPop Ax");  
                            if (!(strcmp(q[i].opr, "+")))
                                fprintf (outMC,"\n\tAdd Ax, %s\n",ChiffreOuNn(q[i].op2));
                            if (!(strcmp(q[i].opr, "-")))
                                fprintf (outMC,"\n\tSub Ax, %s\n",ChiffreOuNn(q[i].op2));
                            if (!(strcmp(q[i].opr, "*")))
                                fprintf (outMC,"\n\tMul %s\n",ChiffreOuNn(q[i].op2));
                            if (!(strcmp(q[i].opr, "/")))
                                fprintf (outMC,"\n\tDiv %s\n",ChiffreOuNn(q[i].op2));
                         }
                         else {
                                if (((q[i].op1)[0]!='t') ) {
                                fprintf (outMC,"\n\tPop Ax");  
                                if (!(strcmp(q[i].opr, "+")))
                                    fprintf (outMC,"\n\tAdd Ax, %s\n",ChiffreOuNn(q[i].op1));
                                if (!(strcmp(q[i].opr, "-")))
                                    fprintf (outMC,"\n\tMov Bx, %s\n\tSub Bx, Ax\n\tMov Ax, Bx\n",ChiffreOuNn(q[i].op1));
                                if (!(strcmp(q[i].opr, "*")))
                                    fprintf (outMC,"\n\tMul %s\n",ChiffreOuNn(q[i].op1));
                                if (!(strcmp(q[i].opr, "/")))
                                    fprintf (outMC,"\n\tMov Bx, Ax\n\tMov Ax, %s\n\tDiv Bx\n",ChiffreOuNn(q[i].op1));
                         }
                         }
                    }
                  }

                fprintf (outMC,"\tPush Ax\n");
            }else 
                if ( ((q[i].opr)[0]=='=') && ((q[i].op1)[0]=='t') )   {
                    fprintf (outMC,"\n\tPop Ax");
                    fprintf (outMC,"\n\tMov [%s], Ax\n",q[i].res);
                }
                else 
                    if ( ((q[i].opr)[0]=='=') && ((q[i].op1)[0]!='t') )  {

                        if ( isAnumber(q[i].op1) ) fprintf (outMC,"\n\tMov [%s], %s\n",q[i].res, q[i].op1);
                            else fprintf (outMC,"\n\tMov Ax, [%s]\n\tMov [%s] Ax\n",q[i].op1, q[i].res);
                    }
                    else {

					//Cas des branchements
                        if( (!strcmp(q[i].opr, "BE")) || (!strcmp(q[i].opr, "BLE")) || (!strcmp(q[i].opr, "BGE")) || (!strcmp(q[i].opr, "BNE")) ) {
                            
							//mettre a jour la tables des etiquettes
                            int num = atoi(strdup(q[i].op1)); 
                            char lbl[12]="";           
                            sprintf(lbl,"Label_%s\n",strdup(q[i].op1));
                            LabelTab[num-1]=1;

                            fprintf (outMC,"\n\tMov Ax, %s\n\tCmp Ax, %s",ChiffreOuNn(q[i].op2), ChiffreOuNn(q[i].res));

                            if(!strcmp(q[i].opr, "BE")) 
                                fprintf (outMC,"\n\tJE %s",lbl);
                            else
                            if(!strcmp(q[i].opr, "BNE")) 
                                fprintf (outMC,"\n\tJNE %s",lbl);
                            else
                            if(!strcmp(q[i].opr, "BG")) 
                                fprintf (outMC,"\n\tJG %s",lbl);
                            else
                            if(!strcmp(q[i].opr, "BL")) 
                                fprintf (outMC,"\n\tJL %s",lbl);
                            else
                            if(!strcmp(q[i].opr, "BGE")) 
                                fprintf (outMC,"\n\tJGE %s",lbl);
                            else
                            if(!strcmp(q[i].opr, "BLE")) 
                                fprintf (outMC,"\n\tJLE %s",lbl);
                                
                            lbl[0]='\0';
                        }
                        else if (!strcmp(q[i].opr, "BR")){
								//mettre a jour la tables des etiquettes
                                int num = atoi(strdup(q[i].op1));  
                                LabelTab[num-1]=1;                          

                                char lbl[12]="";           
                                sprintf(lbl,"Label_%s\n",strdup(q[i].op1));
                                fprintf (outMC,"\n\tJMP %s\n",lbl);
                                lbl[0]='\0';
                             }
                    }
    
     }
    if(LabelTab[indq]==1) fprintf (outMC,"\nLabel_%d:", indq+1);
    fprintf (outMC,"\nMov AH 4CH");
    fprintf (outMC,"\nINT 21H\n");  
    fprintf (outMC,"\nCODE ends\n\tEND MAIN\n\n"); 
    fclose(outMC);

}






