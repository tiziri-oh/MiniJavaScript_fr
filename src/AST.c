#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AST.h"
#include "ASM.h"

//un ast vide
AST* creerAST(){
	AST* ast = (AST*) malloc(sizeof(AST));
	ast->tete = NULL;
	return ast;
}

//creer un noeud
AST_Noeud* creer_noeud_ast(int type){
	AST_Noeud* ast_noeud = (AST_Noeud*) malloc (sizeof(AST_Noeud));
	ast_noeud->type = type;
	ast_noeud->fils = creer_liste();
	return ast_noeud;
}

//ajouter un fils a un parent(dans le debut de la liste)
void ajouter_noeud_ast_debut(AST_Noeud* parent, AST_Noeud* fils){
	ajout_liste_debut(parent->fils, creer_liste_noeud(fils));
}


//ajouter un fils a un parent(dans le debut de la liste)
void ajouter_noeud_ast_fin(AST_Noeud* parent, AST_Noeud* fils){
	ajout_liste_fin(parent->fils, creer_liste_noeud(fils));
}


//supprimer un noeud => prendre le noeud a supp avec son pere et son fils
void supprimer_noeud_ast(AST_Noeud* noeud){
	int i;
	if(noeud == NULL){
		return;
	}
	if(noeud->type == AST_ident){
		free(noeud->attribut.identifiant);
	}
	struct liste_noeud* ptr = noeud->fils->debut;
	while(ptr != NULL){
		supprimer_noeud_ast(ptr->valeur);
		ptr = ptr->suivant;
	}
	supprimer_liste(noeud->fils);
}

//supprimer ast
void supprimer_ast(AST* ast){
	if(ast == NULL){
		return;
	}
	supprimer_noeud_ast(ast->tete);
	ast->tete = NULL;
	free(ast);
}

//afficher les noeuds d'un arbre
void afficher_noeud_ast(AST_Noeud* noeud, int hauteur){
	int j;

	for (j = 0; j < (hauteur - 1); ++j){
		printf("   ");
	}
	if( hauteur != 0){
		printf("\\__");
	}
	
	switch(noeud->type){
		case AST_entete:
			printf("Entete\n");
			break;
		case AST_prog: 
			printf("Programme\n");
 			break;
 		case AST_cmds: 
			printf("liste de commandes\n");
 			break;
		case AST_cmd: 
			printf("Commande\n");
 			break;
 		case AST_cmd_nop:
	 		printf("NOP\n");
 			break;
		case AST_exp: 
			printf("Expression\n");
 			break;
		case AST_exp_groupe: 
			printf("Groupe expression\n");
 			break;
		case AST_ident: 
			printf("Identifiant : %s\n", noeud->attribut.identifiant);
 			break;
		case AST_nombre: 
			printf("Nombre : %g\n", noeud->attribut.nombre);
 			break;
		case AST_bool: 
			printf("Booleen : %d\n", noeud->attribut.booleen);
 			break;
 		case AST_string:
			printf("String %s\n", noeud->attribut.string);
			break;
		case AST_var_affect:
			printf("Declaration initialisé\n");
			break;
		case AST_var:
			printf("Declaration\n");
			break;
		case AST_op_binair: 
			printf("Operateur binaire : %s\n", noeud->attribut.operateur);
 			break;
		case AST_op_ternaire: 
			printf("Operateur ternaire : _ ? _ : _\n");
			break;
		case AST_op_unair: 
			printf("Operateur unaire : %s\n", noeud->attribut.operateur);
 			break;
		case AST_assigne: 
			printf("Assignation : %s\n", noeud->attribut.operateur);
 			break;
		case AST_si: 
			printf("Noeud Si\n");
 			break;
		case AST_si_sinon: 
			printf("Noeud Si Sinon\n");
			break;
		case AST_tantque: 
			printf("Tant Que\n");
			break;
		case AST_faire_tq: 
			printf("Faire Tant Que\n");
 			break;
		case AST_ecrire: 
			printf("Ecrire\n");
 			break;
		case AST_pour:
			printf("Pour\n");
			break;
		case AST_break:
			printf("Break\n");
			break;
		case AST_retourner:
			printf("Retourner\n");
			break;
		case AST_appel:
			printf("Appel de fonction\n");
			break;
		case AST_decl_fonc:
			printf("Declaration de fonction\n");
			break;
		case AST_fonc:
			printf("Fonction (%s)\n", noeud->attribut.identifiant);
			break;
		case AST_exps:
			printf("Liste d'expression\n");
			break;
		case AST_args:
			printf("Liste d'arguments\n");
			break;
		case AST_lambda:
			printf("Lambda %d\n", noeud->attribut.pos);
			break;
		default:
			break;
	}
	hauteur++;
	struct liste_noeud* ptr = noeud->fils->debut;
	while(ptr != NULL){
		afficher_noeud_ast(ptr->valeur, hauteur);
		ptr = ptr->suivant;
	}
}

//afficher l'arbre AST
void afficher_ast(AST* ast){
	int hauteur = 0;
	if(ast->tete != NULL){
		afficher_noeud_ast(ast->tete, hauteur);
	}else{
		printf("AST Vide!\n");
	}
}

//Hosting
void hosting(AST* ast){
	if(ast != NULL){
		if(ast->tete != NULL){
			AST_Noeud* entete = creer_noeud_ast(AST_entete);
			ajouter_noeud_ast_debut(ast->tete, entete);
			hosting_noeud(ast->tete, entete);
			calcul_lambda_pos(ast->tete, ast);
		}
	}
}

//hosting noeud
void hosting_noeud(AST_Noeud* noeud, AST_Noeud* entete){
	AST_Noeud* ptr = NULL;
	AST_Noeud* tmp = NULL;
	struct liste_noeud *courant, *precedent = NULL;
	if(noeud == NULL){
		return;
	}
	if(noeud->type == AST_fonc)
	{
		tmp = creer_noeud_ast(AST_decl_fonc);
		ajouter_noeud_ast_fin(tmp, noeud->fils->debut->valeur);//ajouter ident
		ajouter_noeud_ast_fin(tmp, creer_noeud_ast(AST_lambda));//ajout lambda
		ajouter_noeud_ast_fin(tmp, noeud->fils->debut->suivant->valeur);//ajout noeuds args
		ajouter_noeud_ast_fin(entete, tmp);
		
		//copie du nom de la fonction dans identifiant du noeud fonction
		tmp = noeud->fils->debut->valeur;
		noeud->attribut.identifiant = strdup(tmp->attribut.identifiant);
		
		//enlever les noeuds ident et args de fonction et garder que le coprs de la fonction (qui est en derniers)
		noeud->fils->debut = noeud->fils->fin;
		AST_Noeud* prog = noeud->fils->fin->valeur;

		//ajout de l'entete à la fonction
		AST_Noeud* entete_fonc = creer_noeud_ast(AST_entete);
		ajouter_noeud_ast_debut(prog, entete_fonc);
		hosting_noeud(prog, entete_fonc); 
		//ne pas propager le traitement ===> return 
		return;
	}
	courant = noeud->fils->debut;
	while(courant){
		ptr = courant->valeur;
		//afficher_noeud_ast(ptr, 0);
		hosting_noeud(ptr, entete);
		if(ptr->type == AST_var){
			//printf("var detecté\n");
			//afficher_noeud_ast(ptr, 0);
			ajouter_noeud_ast_fin(entete, ptr);
			if(precedent != NULL){
				precedent->suivant = courant->suivant;
			}else{
				noeud->fils->debut = courant->suivant;
			}
			//afficher_noeud_ast(entete, 0);
		}else{
			precedent = courant;
		}
		courant = courant->suivant;

	}
	
}

int chercher_position_fonction(AST_Noeud* noeud, char* ident, AST* ast){
	int position = -1;
	if(noeud->type == AST_fonc){
		if(strcmp(noeud->attribut.identifiant, ident) == 0){
			compterInstrEntreASTNoeuds(ast->tete, noeud, 1, &position);
		}
		return position;
	}
	struct liste_noeud* ptr = noeud->fils->debut;
	while(ptr){
		position = chercher_position_fonction(ptr->valeur, ident, ast);
		if(position != -1){
			return position;
		}
		ptr = ptr->suivant;		
	}
	return position;

}

void calcul_lambda_pos(AST_Noeud* noeud, AST* ast){
	AST_Noeud* ent = NULL;
	AST_Noeud* tmp = NULL;
	AST_Noeud* lambda = NULL;
	AST_Noeud* ident = NULL;
	struct liste_noeud* ptr = NULL;

	if(noeud->type == AST_prog){
		ent = noeud->fils->debut->valeur;
		ptr = ent->fils->debut;
		while(ptr){
			tmp = ptr->valeur;
			if(tmp->type == AST_decl_fonc){
				ident = tmp->fils->debut->valeur;
				lambda  = tmp->fils->debut->suivant->valeur;
				lambda->attribut.pos = chercher_position_fonction(noeud->fils->fin->valeur, ident->attribut.identifiant, ast);
				int pos_lambda = -1;
				compterInstrEntreASTNoeuds(ast->tete, lambda,0, &pos_lambda);
				lambda->attribut.pos -= (pos_lambda + 1);
			}
			ptr = ptr->suivant;
		}
		//entete deja parcouru on passe a la liste de cmds de prog
		ptr = noeud->fils->fin;
	}else{
		ptr = noeud->fils->debut;
	}
	while(ptr){
		calcul_lambda_pos(ptr->valeur, ast);
		ptr = ptr->suivant;
	}
}
