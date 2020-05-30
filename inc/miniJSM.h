#ifndef MINIJSM_H
#define MINIJSM_H

#include <stdbool.h>
#include "malib.h" 	
#include "ASM.h"

#define MAX_PILE	128
#define MAX_RAM		128
#define MAX_CODE	1024
 

//type de données

enum var_type_e{
	var_type_booleen,
	var_type_reel,
	var_type_string,
	var_type_indefini,
	var_type_cloture,
	var_type_objet
};


union donnee{
	double	reel;
	bool 	booleen;
	char* 	string;
};

struct variable{
	enum  var_type_e	type;
	union donnee 		valeur;
};

struct map_noeud{
	char*  			  nom;
	struct variable   var;
	struct map_noeud* suivant;
};

struct context{
	struct map_noeud* tete;
};

struct instruction{
	int  code_oper;	//pour le type d'instruction
	int  offset;	//pour Jump et CodJmp, Lambda
	union donnee cst;     //pour CstRe, CstBo, cstStr
	char* ident; //pour GetVar, SetVar
};

//La machine virtuelle
struct machineJS{

	int 				SP;
	int					PC;
	struct context*		CC;
	struct instruction* CODE;
	struct context*    	CONTEXT;
	struct variable*    PILE; //utilisee ici comme tableau d union 
	struct variable*  	RAM;  // a verifier après avec les autres fragments
	
};
struct machineJS* creerJSM();
void  afficherJSM(struct machineJS* jsm);
void  decoderASM (const char* nomFichier, struct machineJS* jsm);
void  demmarerJSM(struct machineJS* jsm, bool debug);
void  arreterJSM(struct machineJS* jsm);

//manipulation du context
struct variable contextGet(struct context* context, char* nom);
void contextSet(struct context* context, char* nom, struct variable valeur);
void contextInsert(struct context* context, char* nom, struct variable valeur);
struct context* contextCreer();
void contextSupprimer(struct context* context);
void contextAfficher(struct context* context);

//manipulation de la pile
void push(struct machineJS* jsm, struct variable donnee);
struct variable pull(struct machineJS* jsm);
struct variable pop (struct machineJS* jsm);

//liste des instruction JSM en lien avec chaque instr ASM
void exec_addition 		(struct machineJS* jsm);
void exec_soustraction 	(struct machineJS* jsm);
void exec_multiplication(struct machineJS* jsm);
void exec_division (struct machineJS* jsm);
void exec_negation (struct machineJS* jsm);
void exec_modulo (struct machineJS* jsm);
void exec_non (struct machineJS* jsm);
void exec_botore (struct machineJS* jsm);
void exec_retobo (struct machineJS* jsm);

//Gestion de l'arithmetique des String
void exec_concat	(struct machineJS* jsm);			
void exec_cstString	(struct machineJS* jsm);
void exec_stToRe	(struct machineJS* jsm);
void exec_reToSt	(struct machineJS* jsm);

//comparaison des string
void exec_inferieurEqS	(struct machineJS* jsm);
void exec_superieurEqS	(struct machineJS* jsm);
void exec_inferieurStS	(struct machineJS* jsm);
void exec_superieurStS	(struct machineJS* jsm);
void exec_videS			(struct machineJS* jsm);
//Comparaison
void exec_egal (struct machineJS* jsm);
void exec_different (struct machineJS* jsm);
void exec_inferieurEq (struct machineJS* jsm);
void exec_superieurEq (struct machineJS* jsm);
void exec_inferieurSt (struct machineJS* jsm);
void exec_superieurSt (struct machineJS* jsm);

//Jumps
void exec_saut (struct machineJS* jsm);
void exec_sautCond (struct machineJS* jsm);
void exec_cas (struct machineJS* jsm);
void exec_casp (struct machineJS* jsm);

//Gestion de la pile
void exec_constRe (struct machineJS* jsm);
void exec_constBo (struct machineJS* jsm);
void exec_constInd (struct machineJS* jsm);
void exec_copy (struct machineJS* jsm);
void exec_swap (struct machineJS* jsm);
void exec_drop (struct machineJS* jsm);
void exec_noop (struct machineJS* jsm);
void exec_typede (struct machineJS* jsm);

//Gestion des variables
void exec_setvar (struct machineJS* jsm);
void exec_getvar (struct machineJS* jsm);
void exec_dclvar (struct machineJS* jsm);

//securite et appels systeme
void exec_arreter (struct machineJS* jsm);
void exec_erreur (struct machineJS* jsm);
void exec_assertion (struct machineJS* jsm);
void exec_afficher (struct machineJS* jsm);

//liste d'instructions
struct instr_machine {
	char* nom_instruction;
	void (*instrFunct) (struct machineJS*);
};



#endif
