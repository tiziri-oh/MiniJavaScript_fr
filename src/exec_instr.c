#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "miniJSM.h"

void exec_addition (struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type = var_type_reel, .valeur.reel = a.valeur.reel + b.valeur.reel};
	push(jsm, c);
	++jsm->PC;
}
void exec_soustraction (struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type = var_type_reel, .valeur.reel = a.valeur.reel - b.valeur.reel};
	push(jsm, c);
	++jsm->PC;
}
void exec_multiplication (struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type = var_type_reel, .valeur.reel = a.valeur.reel * b.valeur.reel};
	push(jsm, c);
	++jsm->PC;
}
void exec_division (struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type = var_type_reel, .valeur.reel = a.valeur.reel / b.valeur.reel};
	push(jsm, c);
	++jsm->PC;
}
void exec_negation (struct machineJS* jsm){
	struct variable b = pop(jsm);
	b.valeur.reel *= -1; 
	push(jsm, b);
	++jsm->PC;
}
void exec_modulo (struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type = var_type_reel, .valeur.reel = (int)a.valeur.reel % (int)b.valeur.reel};
	push(jsm, c);
	++jsm->PC;
}
void exec_non (struct machineJS* jsm){
	struct variable b = pop(jsm);
	b.valeur.booleen = !(b.valeur.booleen); 
	push(jsm, b);
	++jsm->PC;
}
void exec_botore (struct machineJS* jsm){
	struct variable b = pop(jsm);
	b.type = var_type_reel;
	if(b.valeur.booleen == true)
	{
		b.valeur.reel = 1;
	}else{
		b.valeur.reel = 0;
	}
	push(jsm, b);
	++jsm->PC;
}
void exec_retobo (struct machineJS* jsm){
	struct variable r = pop(jsm);
	r.type = var_type_booleen;
	if(r.valeur.reel == 0)
	{
		r.valeur.booleen = false;
	}else{
		r.valeur.booleen = true;
	}
	push(jsm, r);
	++jsm->PC;
}

//Comparaison
void exec_egal (struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type =var_type_booleen,  .valeur.booleen = false};
	if(a.type == b.type){
		switch(a.type){
			case var_type_booleen:
				if(a.valeur.booleen == b.valeur.booleen){
					c.valeur.booleen = true;
				}
				break;
			case var_type_reel:
				if(a.valeur.reel == b.valeur.reel){
					c.valeur.booleen = true;
				}
				break;
			default:
				break;
		}

	}
	push(jsm, c);
	++jsm->PC;
}

void exec_different (struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type =var_type_booleen,  .valeur.booleen = false};
	if(memcmp(&a,&b,sizeof(struct variable)) != 0){
		c.valeur.booleen = true;
	}
	push(jsm, c);
	++jsm->PC;
}
void exec_inferieurEq (struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type =var_type_booleen,  .valeur.booleen = false};
	if(a.valeur.reel <= b.valeur.reel){
		c.valeur.booleen = true;
	}
	push(jsm, c);
	++jsm->PC;
}
void exec_superieurEq (struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type =var_type_booleen,  .valeur.booleen = false};
	if(a.valeur.reel >= b.valeur.reel){
		c.valeur.booleen = true;
	}
	push(jsm, c);
	++jsm->PC;
}
void exec_inferieurSt (struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type =var_type_booleen,  .valeur.booleen = false};
	if(a.valeur.reel < b.valeur.reel){
		c.valeur.booleen = true;
	}
	push(jsm, c);
	++jsm->PC;
}
void exec_superieurSt (struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type =var_type_booleen,  .valeur.booleen = false};
	if(a.valeur.reel > b.valeur.reel){
		c.valeur.booleen = true;
	}
	push(jsm, c);
	++jsm->PC;
}

//Jumps
void exec_saut (struct machineJS* jsm){
	int offset = jsm->CODE[jsm->PC].offset;
	jsm->PC += offset + 1;
}
void exec_sautCond (struct machineJS* jsm){
	int offset = jsm->CODE[jsm->PC].offset;
	
	if(pop(jsm).valeur.booleen){
		jsm->PC += 1;
	}else{
		jsm->PC += offset + 1;
	}
}

void exec_cas (struct machineJS* jsm){
	struct variable a = pop(jsm);
	jsm->PC += (int) a.valeur.reel + 1;
}
void exec_casp (struct machineJS* jsm){
	++jsm->PC;
}

//Gestion de la pile
void exec_constRe (struct machineJS* jsm){
	struct variable var = {.type = var_type_reel, .valeur = jsm->CODE[jsm->PC].cst};
	push(jsm, var);
	++jsm->PC;
}
void exec_constBo (struct machineJS* jsm){
	struct variable var = {.type = var_type_booleen, .valeur = jsm->CODE[jsm->PC].cst};
	push(jsm, var);
	++jsm->PC;
}
void exec_constInd (struct machineJS* jsm){
	struct variable var = {.type = var_type_indefini};
	push(jsm, var);
	++jsm->PC;
}
void exec_copy (struct machineJS* jsm){
	push(jsm, pull(jsm));
	++jsm->PC;
}
void exec_swap (struct machineJS* jsm){
	struct variable a = pop(jsm);
	struct variable b = pop(jsm);
	push(jsm, a);
	push(jsm, b);
	++jsm->PC;
}
void exec_drop (struct machineJS* jsm){
	pop(jsm);
	++jsm->PC;
}
void exec_noop (struct machineJS* jsm){
	++jsm->PC;
}
void exec_typede (struct machineJS* jsm){
	struct variable v = pull(jsm);
	struct variable res = {.type = var_type_reel, .valeur.reel = v.type};
	push(jsm,res);
	++jsm->PC;
}

//Gestion des variables
void exec_setvar (struct machineJS* jsm){
	contextSet(jsm->CC, jsm->CODE[jsm->PC].ident, pop(jsm));
	++jsm->PC;
}
void exec_getvar (struct machineJS* jsm){
	struct variable var = contextGet(jsm->CC, jsm->CODE[jsm->PC].ident);
	if(var.type == var_type_string){
		char* copie = strdup(var.valeur.string);
		var.valeur.string = copie;
	}
	push(jsm, var);
	++jsm->PC;
}

void exec_arreter (struct machineJS* jsm){
	++jsm->PC;
}
void exec_erreur (struct machineJS* jsm){
	++jsm->PC;
}
void exec_assertion (struct machineJS* jsm){
	++jsm->PC;
}
void exec_afficher (struct machineJS* jsm){
	struct variable p = pop(jsm);
	switch(p.type){
		case var_type_reel:
			printf("PRINT: [reel: %g]\n", p.valeur.reel);
			break;
		case var_type_booleen:
			printf("PRINT: [bool: %s]\n", p.valeur.booleen ? "Vrais" : "Faux");
			break;
		case var_type_string:
			printf("PRINT: [string: %s]\n", p.valeur.string);
			break;
		default:
			printf("PRINT: [undef]\n");
			break;
	}
	++jsm->PC;
}



//Gestion de l'arithmetique des String
void exec_concat	(struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);	
	int taille = strlen(a.valeur.string) + strlen(b.valeur.string) + 1;	
	char* tmp = (char*) malloc(taille * sizeof(char));
	tmp[0] = 0;
	strcat(tmp, a.valeur.string);
	strcat(tmp, b.valeur.string);
	struct variable c = {.type = var_type_string, .valeur.string = tmp};
	free(a.valeur.string);
	free(b.valeur.string);
	
	push(jsm, c);
	++jsm->PC;
}


void exec_cstString	(struct machineJS* jsm){
	struct variable var = {.type = var_type_string, .valeur = jsm->CODE[jsm->PC].cst};
	push(jsm, var);
	++jsm->PC;
}

void exec_stToRe	(struct machineJS* jsm){
	struct variable a = pop(jsm);
	a.type = var_type_reel;
	a.valeur.reel = atof(a.valeur.string);
	push(jsm, a);
	++jsm->PC;
}
void exec_reToSt	(struct machineJS* jsm){
	struct variable a = pop(jsm);
	a.type = var_type_string;
	double d =  a.valeur.reel;
	char* str = (char*)malloc(64 * sizeof(char)); 
	a.valeur.string = str;
	sprintf(str, "%g", d);
	push(jsm, a);
	++jsm->PC;
}

//comparaison des string
void exec_inferieurEqS	(struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type =var_type_booleen,  .valeur.booleen = false};
	if(strcmp(a.valeur.string , b.valeur.string) <= 0){
		c.valeur.booleen = true;
	}
	free(b.valeur.string);
	free(a.valeur.string);
	push(jsm, c);
	++jsm->PC;
}
void exec_superieurEqS	(struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type =var_type_booleen,  .valeur.booleen = false};
	if(strcmp(a.valeur.string , b.valeur.string) >= 0){
		c.valeur.booleen = true;
	}
	free(b.valeur.string);
	free(a.valeur.string);
	push(jsm, c);
	++jsm->PC;
}
void exec_inferieurStS	(struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type =var_type_booleen,  .valeur.booleen = false};
	if(strcmp(a.valeur.string , b.valeur.string) < 0){
		c.valeur.booleen = true;
	}
	free(b.valeur.string);
	free(a.valeur.string);
	push(jsm, c);
	++jsm->PC;
}
void exec_superieurStS	(struct machineJS* jsm){
	struct variable b = pop(jsm);
	struct variable a = pop(jsm);
	struct variable c = {.type =var_type_booleen,  .valeur.booleen = false};
	if(strcmp(a.valeur.string , b.valeur.string) > 0){
		c.valeur.booleen = true;
	}
	free(b.valeur.string);
	free(a.valeur.string);
	push(jsm, c);
	++jsm->PC;
}
void exec_videS	(struct machineJS* jsm){
	struct variable a = pop(jsm);
	struct variable c = {.type =var_type_booleen,  .valeur.booleen = false};
	if(strcmp(a.valeur.string , "") == 0){
		c.valeur.booleen = true;
	}
	free(a.valeur.string);
	push(jsm, c);
	++jsm->PC;
}
void exec_dclvar (struct machineJS* jsm){
	struct variable valeur = {.type = var_type_indefini};
	contextInsert(jsm->CC, jsm->CODE[jsm->PC].ident, valeur);
	++jsm->PC;	
}