#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include "malib.h"

union AST_attribut{
	char*				identifiant; //si justez un nom ou ident
	char				operateur[8];
	bool				booleen;
	double				nombre;
	int 				sautBreak; // pour le jump
	int 				pos;//pour lambda
	char* 				string; // pour le type string 
};



enum AST_type_e{
	AST_entete,
	AST_prog,
	AST_cmd,
	AST_cmd_nop,
	AST_exp,
	AST_exp_groupe,
	AST_ident,
	AST_nombre,
	AST_bool,
	AST_op_binair,
	AST_op_ternaire,
	AST_op_unair,
	AST_assigne,
	AST_cmds,
	AST_si,
	AST_si_sinon,
	AST_tantque,
	AST_faire_tq,
	AST_ecrire,
	AST_pour,
	AST_var_affect,
	AST_var,
	AST_string,
	AST_break,
	AST_retourner,
	AST_appel,
	AST_decl_fonc,
	AST_lambda,
	AST_fonc,
	AST_exps,
	AST_args	
};

struct AST_Noeud{
	enum 		AST_type_e	type;
	union 		AST_attribut	attribut;
	struct 		liste*		fils;
	
};

typedef struct AST_Noeud AST_Noeud;


struct AST{
	AST_Noeud*			tete; 
};

typedef struct AST AST;


//les fonction de bases

//un ast vide
AST* creerAST();

//afficher ast 
void afficher_ast(AST* ast);

//supprimer l'arbre ast
void supprimer_ast(AST* ast);

//creer un noeud
AST_Noeud* creer_noeud_ast(int type);

//ajouter un noeud a un ast(dans la tete de la liste)
void ajouter_noeud_ast_debut(AST_Noeud* parent, AST_Noeud* fils);

//ajouter fin(dans la liste de l'arbre
void ajouter_noeud_ast_fin(AST_Noeud* parent, AST_Noeud* fils);

//supprimer un noeud => prendre le noeud a supp avec son pere et son fils
void supprimer_noeud_ast(AST_Noeud* noeud);

//aficher noeud_ast
void afficher_noeud_ast(AST_Noeud* noeud, int hauteur);

void hosting(AST* ast);

void hosting_noeud(AST_Noeud* noeud, AST_Noeud* entete);

int chercher_position_fonction(AST_Noeud* noeud, char* ident, AST* ast);

void calcul_lambda_pos(AST_Noeud* noeud, AST* ast);

#endif
