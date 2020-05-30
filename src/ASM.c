#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malib.h"
#include "ASM.h"
#include "miniJSM.h"

//On le calcule à la compilation
void chercherInitBreak(AST_Noeud* noeud, AST_Noeud* boucle){
	struct liste_noeud* ptr = noeud->fils->debut;
	int distance = -1;
	//Les break dans les boucles à l'interieur de la boucle mere (pour, tantque et faire tantque) ne sont pas prise en compte
	if(   (noeud->type == AST_pour) 
		||(noeud->type == AST_tantque)
		||(noeud->type == AST_faire_tq))
	{
		return;
	}
	//break trouvé et il n'a pas de fils
	if(noeud->type == AST_break){
		int nbinstboucle = compterInstrEntreASTNoeuds(boucle, noeud, 0, &distance);
		// printf("boucle : %d, distance: %d\n", nbinstboucle, distance);
		/* distance ne prends pas en compte le nombre d'instruction dans break d'ou le +1 compterInstrASTNoeud(tmp, 0) */
		noeud->attribut.sautBreak = nbinstboucle - (distance + 1 );
		return;
	}
	//sinon continuer a chercher le prchaine break

	while(ptr){
		chercherInitBreak(ptr->valeur, boucle);
		ptr = ptr->suivant;
	}
}


int compterInstrASTNoeud(AST_Noeud* noeud, int offset){
	return compterInstrEntreASTNoeuds(noeud, NULL, offset, NULL);
}

// nbInstANoeudDest ne prend pas en compte le nombre d'instructions dans le noeudDest
int compterInstrEntreASTNoeuds(AST_Noeud* noeud, AST_Noeud* NoeudDest, int offset, int* nbInstANoeudDest)
{
	if(noeud == NULL)
	{
		return offset;
	}

	//comparaison par adresse
	if(NoeudDest == noeud && nbInstANoeudDest != NULL){
		//if(*nbInstANoeudDest < 0)
		*nbInstANoeudDest = offset; // sauvegarder la valeur actuel de nombre d'instruction jusqu'au noeud destination
	}
	
	struct liste_noeud* ptr = NULL;
	
	AST_Noeud* tmp = NULL;

	switch(noeud->type)
	{
		case AST_ident:
			offset++;
			break;
			
		case AST_nombre:
			offset++;
			break;
		
		case AST_bool:
			offset++;
			break;
		
		case AST_string:
			offset++;
			break;

		case AST_op_binair:
			ptr = noeud->fils->debut;
			while(ptr != NULL){
				offset = compterInstrEntreASTNoeuds(ptr->valeur, NoeudDest, offset, nbInstANoeudDest);
				ptr = ptr->suivant;
			}
			if(strcmp(noeud->attribut.operateur, "+") == 0
						|| strcmp(noeud->attribut.operateur, ">") == 0
						|| strcmp(noeud->attribut.operateur, ">=") == 0
						|| strcmp(noeud->attribut.operateur, "<") == 0
						|| strcmp(noeud->attribut.operateur, "<=") == 0
					){
				offset += 24;
			}else{
				offset++;
			}
				
			break;
			
		case AST_op_unair:
			ptr = noeud->fils->debut;
			while(ptr != NULL){
				offset = compterInstrEntreASTNoeuds(ptr->valeur, NoeudDest, offset, nbInstANoeudDest);
				ptr = ptr->suivant;
			}
			if(strcmp(noeud->attribut.operateur, "!") == 0){				
				offset += 7;
			}else{
				offset++;
			}
			
			break;
		case AST_var:
			offset++;
			break;		
		case AST_assigne:
			ptr = noeud->fils->fin;
			offset = compterInstrEntreASTNoeuds(ptr->valeur, NoeudDest, offset, nbInstANoeudDest);
			tmp = (AST_Noeud*)noeud->fils->debut->valeur;
			if(strcmp(noeud->attribut.operateur, "=") == 0){
				offset++;
			}else if(strcmp(noeud->attribut.operateur, "+=") == 0){
				offset += 26;
			}else{
				offset += 3;
			}
			break;

		case AST_ecrire:
			ptr = noeud->fils->debut;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest);
			offset++;
			break;

		case AST_si: 
			ptr = noeud->fils->debut;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest);
			offset++;
			ptr = ptr->suivant;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest);
			break;
		case AST_op_ternaire:
		case AST_si_sinon:
			ptr = noeud->fils->debut;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest); // compiler condition
			offset++;
			ptr = ptr->suivant;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest); // compiler branche si vrai
			offset++;
			ptr = ptr->suivant;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest); // compiler branche si faux
			break;
		
		case AST_tantque:
			ptr = noeud->fils->debut;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest);
			offset++;
			ptr = ptr->suivant;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest);
			offset++;
			break;
	
		case AST_faire_tq:
			offset++;
			ptr = noeud->fils->fin;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest);
			offset++;
			ptr = noeud->fils->debut;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest);
			offset++;
			break;
	
		case AST_pour: //Pour (i = 0; i < n; i = i + 1) ecrire (i);
			ptr = noeud->fils->debut;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest); //init i
			ptr = ptr->suivant;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest);//cond sur i
			offset++;			
			ptr = ptr->suivant;
			tmp = ptr->valeur; // i = i + 1					
			ptr = ptr->suivant;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest);//programme (ens d instruction)			
			offset = compterInstrEntreASTNoeuds(tmp,NoeudDest, offset, nbInstANoeudDest);//incrementation de i
			offset++;
			break;
			
		case AST_break:
			offset++;
			break;

		case AST_retourner:
			ptr = noeud->fils->debut;
			if(ptr != NULL)
				offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest);
			offset++;
			break;

		case AST_appel:
			ptr = noeud->fils->debut; //premier fils
			tmp = ptr->valeur;	//appel fonction dans context
			offset+=2;
			ptr = ptr->suivant;  // compiler expressions			
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest);
			offset++;
			break;
		case AST_decl_fonc:
			ptr = noeud->fils->debut; //premier fils
			tmp = ptr->valeur;	//declare fonction dans context
			offset++;
			ptr = ptr->suivant;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest); //Lambda
			ptr = ptr->suivant;
			offset = compterInstrEntreASTNoeuds(ptr->valeur,NoeudDest, offset, nbInstANoeudDest); //arguments
			offset++;
			break;
		case AST_fonc:
			offset++;
			ptr = noeud->fils->debut;
			while(ptr != NULL){
				offset = compterInstrEntreASTNoeuds(ptr->valeur, NoeudDest, offset, nbInstANoeudDest);
				ptr = ptr->suivant;
			}
			break;
		case AST_exps:
			ptr = noeud->fils->debut;
			while(ptr != NULL){
				tmp = ptr->valeur;	
				offset = compterInstrEntreASTNoeuds(tmp,NoeudDest, offset, nbInstANoeudDest);
				offset++;
				ptr = ptr->suivant;
			}
			break;
		case AST_args:
			ptr = noeud->fils->debut;
			while(ptr != NULL){
				tmp = ptr->valeur;	
				offset++;
				ptr = ptr->suivant;
			}
			break;
		case AST_lambda:
			offset++;
			break;

		default:
			ptr = noeud->fils->debut;
			while(ptr != NULL){
				offset = compterInstrEntreASTNoeuds(ptr->valeur, NoeudDest, offset, nbInstANoeudDest);
				ptr = ptr->suivant;
			}

			break;			
	}

	return offset;
}

void compilerASTNoeud(AST_Noeud* noeud, FILE* file)
{
	if(noeud == NULL)
		return;
	struct liste_noeud* ptr = NULL;
	AST_Noeud* tmp = NULL;
	int cmp1, cmp2, cmp3;
	char operateurRe[16], operateurStr[16];
	bool typeverif  = false;

	switch(noeud->type){
		case AST_ident:
			fprintf(file, "GetVar %s\n", noeud->attribut.identifiant);
			break;
			
		case AST_nombre:
			fprintf(file, "CstRe %g\n", noeud->attribut.nombre);
			break;
		
		case AST_bool:
			fprintf(file, "CstBo %s\n", noeud->attribut.booleen ? "Vrais" : "Faux");
			break;
		case AST_string:
			fprintf(file, "CstStr \"%s\"\n",  noeud->attribut.string);			
			break;
			
		case AST_op_binair:
			ptr = noeud->fils->debut;
			while(ptr != NULL){
				compilerASTNoeud(ptr->valeur, file);
				ptr = ptr->suivant;
			}
			if(strcmp(noeud->attribut.operateur, "+") == 0){
				strcpy(operateurRe, ADDITION);
				strcpy(operateurStr, CONCAT);	
				typeverif = true;			
			}else if(strcmp(noeud->attribut.operateur, "-") == 0)
				fprintf(file, "SubiRe\n");
				
			else if(strcmp(noeud->attribut.operateur, "*") == 0)
				fprintf(file, "MultRe\n");
				
			else if(strcmp(noeud->attribut.operateur, "/") == 0)
				fprintf(file, "DiviRe\n");
				
			else if(strcmp(noeud->attribut.operateur, "%") == 0)
				fprintf(file, "Modulo\n");
				
			else if(strcmp(noeud->attribut.operateur, "<") == 0){
				strcpy(operateurRe, INFERIEURST);
				strcpy(operateurStr, INFERIEURSTS);	
				typeverif = true;
			}
				
			else if(strcmp(noeud->attribut.operateur, "<=") == 0){
				strcpy(operateurRe, INFERIEUREQ);
				strcpy(operateurStr, INFERIEUREQS);	
				typeverif = true;
			}
				
			
			else if(strcmp(noeud->attribut.operateur, ">") == 0){
				strcpy(operateurRe, SUPERIEURST);
				strcpy(operateurStr, SUPERIEURSTS);	
				typeverif = true;
			}
				
			else if(strcmp(noeud->attribut.operateur, ">=") == 0){
				strcpy(operateurRe, SUPERIEUREQ);
				strcpy(operateurStr, SUPERIEUREQS);	
				typeverif = true;
			}
				
			else if(strcmp(noeud->attribut.operateur, "!==") == 0)
				fprintf(file, "NotEq\n");
				
			else if(strcmp(noeud->attribut.operateur, "===") == 0)
				fprintf(file, "Equal\n");
				
			else if(strcmp(noeud->attribut.operateur, "&&") == 0)
				fprintf(file, "AND\n");
				
			else if(strcmp(noeud->attribut.operateur, "||") == 0)
				fprintf(file, "OR\n");
				
			if(typeverif){
				fprintf(file, "Swap\n");      //f1, f2
				fprintf(file, "TypeOf\n");	  //t1, f1, f2
				fprintf(file, "CstRe 1\n");	  //1,t1, f1, f2
				fprintf(file, "Equal\n");	  //b, f1, f2
				fprintf(file, "ConJmp 10\n");  //f1, f2
					//cas ou a est reel
					fprintf(file, "Swap\n");	  //f2,f1
					fprintf(file, "TypeOf\n");	  //t2, f2,f1
					fprintf(file, "CstRe 1\n");	  //1, t2, f2,f1
					fprintf(file, "Equal\n");     //b, f2,f1
					fprintf(file, "ConJmp 2\n");  //f2,f1
						//cas b reel
						fprintf(file, "%s\n",operateurRe);  // cas 
						fprintf(file, "Jump 12\n");
						//cas b est string
						fprintf(file, "StToRe\n"); 
						fprintf(file, "%s\n",operateurRe);  // cas 
						fprintf(file, "Jump 9\n");
					//cas a est string
					fprintf(file, "Swap\n"); 		//f2,f1
					fprintf(file, "TypeOf\n"); 		//t2,f2,f1
					fprintf(file, "CstRe 2\n"); 	//2, t2 f2,f1
					fprintf(file, "Equal\n"); 		//b , f2,f1
					fprintf(file, "ConJmp 2\n");    //b , f2,f1
						fprintf(file, "%s\n",operateurStr);  // cas 
						fprintf(file, "Jump 2\n");
						fprintf(file, "ReToSt\n"); 
						fprintf(file, "%s\n",operateurStr);  // cas 
			}
			break;
			
		case AST_op_unair:
			ptr = noeud->fils->debut;
			while(ptr != NULL){
				compilerASTNoeud(ptr->valeur, file);
				ptr = ptr->suivant;
			}
			if(strcmp(noeud->attribut.operateur, "-") == 0)
				fprintf(file, "NegaRe\n");
			
			else if(strcmp(noeud->attribut.operateur, "!") == 0){				
				fprintf(file, "TypeOf\n"); 	  //t1, f1
				fprintf(file, "CstRe 2\n");	  //2,t1, f1
				fprintf(file, "Equal\n");	  //b, f1	
				fprintf(file, "ConJmp 2\n"); //f1
					fprintf(file, "IsEmpt\n");
					fprintf(file, "Jump 1\n");
					fprintf(file, "Not\n");
			}
				
			else if(strcmp(noeud->attribut.operateur, "TypeOf") == 0)
				fprintf(file, "TypeOf\n");
				
			else 
				fprintf(file, "Error\n");
				
			break;
			
		case AST_assigne:
			if(strcmp(noeud->attribut.operateur, "=") == 0){
				ptr = noeud->fils->fin;
				compilerASTNoeud(ptr->valeur, file);
				tmp = (AST_Noeud*)noeud->fils->debut->valeur;
				fprintf(file, "SetVar %s\n", tmp->attribut.identifiant);
			}else{
				//mettre dans la pile la valeur dest
				tmp = (AST_Noeud*)noeud->fils->debut->valeur;
				fprintf(file, "GetVar %s\n", tmp->attribut.identifiant);
				//empiler le second fils
				ptr = noeud->fils->fin;
				compilerASTNoeud(ptr->valeur, file);
				
				//faire les operations
				if(strcmp(noeud->attribut.operateur, "+=") == 0){		
					fprintf(file, "Swap\n");      //f1, f2
					fprintf(file, "TypeOf\n");	  //t1, f1, f2
					fprintf(file, "CstRe 1\n");	  //1,t1, f1, f2
					fprintf(file, "Equal\n");	  //b, f1, f2
					fprintf(file, "ConJmp 10\n");  //f1, f2
						//cas ou a est reel
						fprintf(file, "Swap\n");	  //f2,f1
						fprintf(file, "TypeOf\n");	  //t2, f2,f1
						fprintf(file, "CstRe 1\n");	  //1, t2, f2,f1
						fprintf(file, "Equal\n");     //b, f2,f1
						fprintf(file, "ConJmp 2\n");  //f2,f1
							//cas b reel
							fprintf(file, "%s\n",ADDITION);  // cas 
							fprintf(file, "Jump 12\n");
							//cas b est string
							fprintf(file, "StToRe\n"); 
							fprintf(file, "%s\n",ADDITION);  // cas 
							fprintf(file, "Jump 9\n");
						//cas a est string
						fprintf(file, "Swap\n"); 		//f2,f1
						fprintf(file, "TypeOf\n"); 		//t2,f2,f1
						fprintf(file, "CstRe 2\n"); 	//2, t2 f2,f1
						fprintf(file, "Equal\n"); 		//b , f2,f1
						fprintf(file, "ConJmp 2\n");    //b , f2,f1
							fprintf(file, "%s\n",CONCAT);  // cas 
							fprintf(file, "Jump 2\n");
							fprintf(file, "ReToSt\n"); 
							fprintf(file, "%s\n",CONCAT);  // cas 
					fprintf(file, "SetVar %s\n", tmp->attribut.identifiant);

				}
				
				else if(strcmp(noeud->attribut.operateur, "-=") == 0){				
					fprintf(file, "SubiRe\n");
					fprintf(file, "SetVar %s\n", tmp->attribut.identifiant);
				}
				else if(strcmp(noeud->attribut.operateur, "/=") == 0){
					fprintf(file, "DiviRe\n");
					fprintf(file, "SetVar %s\n", tmp->attribut.identifiant);
				}
				
				else if(strcmp(noeud->attribut.operateur, "*=") == 0){
					fprintf(file, "MultRe\n");
					fprintf(file, "SetVar %s\n", tmp->attribut.identifiant);
				}
				
				else if(strcmp(noeud->attribut.operateur, "%=") == 0){
					fprintf(file, "Modulo\n");
					fprintf(file, "SetVar %s\n", tmp->attribut.identifiant);
				}

				else if(strcmp(noeud->attribut.operateur, "**=") == 0){
					fprintf(file, "PowRe\n");
					fprintf(file, "SetVar %s\n", tmp->attribut.identifiant);
				}
			}			
			break;

		case AST_ecrire:
			ptr = noeud->fils->debut;
			compilerASTNoeud(ptr->valeur, file);
			fprintf(file, "Print\n");
			break;

		case AST_si: 
			ptr = noeud->fils->debut;
			compilerASTNoeud(ptr->valeur, file);
			fprintf(file, "ConJmp %d\n", compterInstrASTNoeud(ptr->suivant->valeur,0));
			ptr = ptr->suivant;
			compilerASTNoeud(ptr->valeur, file);
			break;
		
		case AST_op_ternaire:
		case AST_si_sinon:
			ptr = noeud->fils->debut;
			compilerASTNoeud(ptr->valeur, file); // compiler condition
			//sauter branche vrai si condition fausse de nombre instruct de cette branche
			fprintf(file, "ConJmp %d\n",compterInstrASTNoeud(ptr->suivant->valeur,1)/*offset = 1 pour le jump dans la branche vraie*/);
			ptr = ptr->suivant;
			compilerASTNoeud(ptr->valeur, file); // compiler branche si vrai
			fprintf(file, "Jump %d\n", compterInstrASTNoeud(ptr->suivant->valeur,0));
			ptr = ptr->suivant;
			compilerASTNoeud(ptr->valeur, file); // compiler branche si faux
			break;
		
		case AST_tantque:
			ptr = noeud->fils->debut;
			compilerASTNoeud(ptr->valeur, file);
			cmp1 = compterInstrASTNoeud(noeud->fils->fin->valeur,1);
			fprintf(file, "ConJmp %d\n",cmp1);
			
			ptr = ptr->suivant;
			chercherInitBreak(ptr->valeur, noeud);
			cmp1 += compterInstrASTNoeud(noeud->fils->debut->valeur,1);
			compilerASTNoeud(ptr->valeur, file);
			fprintf(file, "Jump %d\n", -(cmp1 + 1)/*pour me positionné avant la premiere instruction losque l'offset est neg*/);			
			break;
	
		case AST_faire_tq:

			cmp1 = compterInstrASTNoeud(noeud->fils->fin->valeur,0);
			fprintf(file, "Jump %d\n", cmp1 + 1);//saut condition + ConJmp
			ptr = noeud->fils->fin;
			compilerASTNoeud(ptr->valeur, file);//compiler condition 
			cmp2 = compterInstrASTNoeud(noeud->fils->debut->valeur,0);
			fprintf(file, "ConJmp %d\n", cmp2 + 1);//saut vers fin (command + jump) si condition est fausse			
			ptr = noeud->fils->debut;
			chercherInitBreak(ptr->valeur, noeud);
			compilerASTNoeud(ptr->valeur, file);
			fprintf(file, "Jump %d\n", -(cmp1 + cmp2 + 1 + 1)/*pour me positionné avant la premiere instruction losque l'offset est neg*/);
			break;
	
		case AST_pour: //Pour (i = 0; i < n; i = i + 1) ecrire (i);
			ptr = noeud->fils->debut;
			compilerASTNoeud(ptr->valeur, file); //init i
			ptr = ptr->suivant;
			
			compilerASTNoeud(ptr->valeur, file);//cond sur i
			cmp1 = compterInstrASTNoeud(noeud->fils->debut->suivant->valeur,0);
			cmp2 = compterInstrASTNoeud(noeud->fils->debut->suivant->suivant->valeur,0);
			cmp3 = compterInstrASTNoeud(noeud->fils->fin->valeur,0);
			fprintf(file, "ConJmp %d\n", cmp2 + cmp3 + 1);
			
			ptr = ptr->suivant;
			tmp = ptr->valeur; // i = i + 1
					
			ptr = ptr->suivant;
			chercherInitBreak(ptr->valeur, noeud);
			compilerASTNoeud(ptr->valeur, file);//programme (ens d instruction)
			
			compilerASTNoeud(tmp, file);//incrementation de i
			fprintf(file, "Jump %d\n", -(cmp1 + cmp2 + cmp3 + 1 + 1));
			break;

		case AST_var:
			ptr = noeud->fils->debut;
			tmp = ptr->valeur;
			fprintf(file, "DclVar %s\n", tmp->attribut.identifiant);
			break;
		case AST_break:
			fprintf(file, "Jump %d\n", noeud->attribut.sautBreak);
			break;
		case AST_retourner:
			ptr = noeud->fils->debut;
			if(ptr != NULL)
				compilerASTNoeud(ptr->valeur, file);
			fprintf(file, "Return\n");
			break;
		case AST_appel:
			ptr = noeud->fils->debut; //premier fils
			tmp = ptr->valeur;	//appel fonction dans context
			fprintf(file, "GetVar %s\n", tmp->attribut.identifiant);
			fprintf(file, "StCall\n");
			ptr = ptr->suivant;  // compiler expressions
			compilerASTNoeud(ptr->valeur, file);
			fprintf(file, "Call\n");
			break;
		case AST_decl_fonc:
			ptr = noeud->fils->debut; //premier fils
			tmp = ptr->valeur;	
			//declare fonction dans context
			fprintf(file, "DclVar %s\n", tmp->attribut.identifiant);
			ptr = ptr->suivant;
			compilerASTNoeud(ptr->valeur, file); //Lambda
			ptr = ptr->suivant;
			compilerASTNoeud(ptr->valeur, file); //arguments
			fprintf(file, "SetVar %s\n", tmp->attribut.identifiant);
			break;
		case AST_fonc:
			fprintf(file, "Jump %d\n", compterInstrASTNoeud(noeud,0) - 1 /*-1 for Jump*/);
			ptr = noeud->fils->debut;
			while(ptr != NULL){
				compilerASTNoeud(ptr->valeur, file);
				ptr = ptr->suivant;
			}
			break;
		case AST_exps:
			ptr = noeud->fils->debut;
			while(ptr != NULL){
				tmp = ptr->valeur;	
				compilerASTNoeud(tmp, file);
				fprintf(file, "SetArg\n");
				ptr = ptr->suivant;
			}
			break;
		case AST_args:
			ptr = noeud->fils->debut;
			while(ptr != NULL){
				tmp = ptr->valeur;	
				fprintf(file, "DclArg %s\n", tmp->attribut.identifiant);
				ptr = ptr->suivant;
			}
			break;
		case AST_lambda:
			fprintf(file, "Lambda %d\n", noeud->attribut.pos);
			break;
		default:
			ptr = noeud->fils->debut;
			while(ptr != NULL){
				compilerASTNoeud(ptr->valeur, file);
				ptr = ptr->suivant;
			}

			break;			
	}	
}

//parcourt AST + affichage
void compilerAST(AST* ast, const char* nomFichier){
	FILE* fichier = fopen(nomFichier, "w");
	if(fichier == NULL){
		printf("erreur fopen \n");
		return;
	}
	if(ast == NULL){
		fprintf(fichier, "Halt\n");
		return;
	}
	compilerASTNoeud(ast->tete, fichier);
	
	fprintf(fichier, "Halt\n");		
	fclose(fichier);
}

int inference_type(	AST_Noeud* noeud){
	if(noeud == NULL){
		return var_type_indefini;
	}
	struct liste_noeud* ptr = noeud->fils->debut;
	int type;
	switch (noeud->type){
		case AST_bool:
			return var_type_booleen;
			break;
		case AST_nombre:
			return var_type_reel;
			break;
		case AST_string:
			return var_type_string;
			break;
		case AST_op_unair:
			if(strcmp(noeud->attribut.operateur, "!") == 0 ){
				return var_type_booleen;
			}else{
				return var_type_reel;
			}
			break;
		case AST_op_binair:
			if((strcmp(noeud->attribut.operateur, "-") == 0 )
				|| (strcmp(noeud->attribut.operateur, "/") == 0)
				|| (strcmp(noeud->attribut.operateur, "%") == 0)
				|| (strcmp(noeud->attribut.operateur, "**") == 0))
			{
				return var_type_reel;
			}else if(strcmp(noeud->attribut.operateur, "+") == 0 ){
				return inference_type(ptr->valeur);
			}else{
				return var_type_booleen;
			}
			break;
		case AST_assigne:
			if((strcmp(noeud->attribut.operateur, "-=") == 0 )
				|| (strcmp(noeud->attribut.operateur, "/=") == 0)
				|| (strcmp(noeud->attribut.operateur, "%=") == 0)
				|| (strcmp(noeud->attribut.operateur, "**=") == 0))
			{
				return var_type_reel;
			}else if(strcmp(noeud->attribut.operateur, "+=") == 0 ){
				return inference_type(ptr->suivant->valeur);
			}else{
				return var_type_indefini;
			}
			break;
		case AST_si:
		case AST_si_sinon:
			return inference_type(ptr->suivant->valeur);
			break;
		default:
			type = var_type_indefini;
			while(ptr != NULL){
				// [ a(undef) + "mot" (string) ] => string;
				type = inference_type(ptr->valeur);
				if( type  != var_type_indefini ){
					return type;
				}
				ptr = ptr->suivant;
			}
			return type;
			break;
	}

}
