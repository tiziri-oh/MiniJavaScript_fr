#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "miniJSM.h"

const struct instr_machine list_exec_fonctions [] = {
	//Arithmetique
	{ADDITION, exec_addition }, 
	{SOUSTRACTION, exec_soustraction }, 
	{MULTIPLICATION, exec_multiplication }, 
	{DIVISION, exec_division }, 
	{NEGATION, exec_negation }, 
	{MODULO, exec_modulo }, 
	{NON, exec_non }, 
	{BOTORE, exec_botore }, 
	{RETOBO, exec_retobo }, 
	//Comparaison
	{EGAL, exec_egal }, 
	{DIFFERENT, exec_different }, 
	{INFERIEUREQ, exec_inferieurEq }, 
	{SUPERIEUREQ, exec_superieurEq }, 
	{INFERIEURST, exec_inferieurSt }, 
	{SUPERIEURST, exec_superieurSt }, 
	//Jumps
	{SAUT, exec_saut }, 
	{SAUTCOND, exec_sautCond }, 
	{CAS, exec_cas }, 
	{CASP, exec_casp }, 
	//Gestion de la pile
	{CSTRE, exec_constRe }, 
	{CSTBO, exec_constBo }, 
	{CSTINDEF, exec_constInd }, 
	{COPY, exec_copy }, 
	{SWAP, exec_swap }, 
	{DROP, exec_drop }, 
	{NOOP, exec_noop }, 
	{TYPEDE, exec_typede }, 
	//Gestion des variables
	{SETVAR, exec_setvar }, 
	{GETVAR, exec_getvar }, 
	{DCLVAR, exec_dclvar },
	//securite et appels systeme
	{ERREUR, exec_erreur }, 
	{ASSERTION, exec_assertion }, 
	{AFFICHER, exec_afficher },
	{ARRETER, exec_arreter },
	//Gestion de l'arithmetique des String
	{CONCAT, exec_concat},	
	{CSTSTRING , exec_cstString	},
	{STTORE, exec_stToRe },
	{RETOST, exec_reToSt },
	//comparaison des string
	{INFERIEUREQS, exec_inferieurEqS },
	{SUPERIEUREQS, exec_superieurEqS },
	{INFERIEURSTS, exec_inferieurStS },
	{SUPERIEURSTS, exec_superieurStS },
	{EST_VIDE_S, exec_videS },

};


struct variable contextGet(struct context* context, char* nom){
	struct variable v;
	if(context == NULL){
		return v;
	}
	struct map_noeud* ptr = NULL;
	struct context* ptrCont = context;

	ptr = ptrCont->tete;
	while(ptr != NULL){
		if(strcmp(ptr->nom, nom) == 0){
			return ptr->var;
		}
		ptr = ptr->suivant;
	}	
	printf("Avertissement GetVar: variable %s indéfinie\n", nom);
	return v;	
}

void 	contextSet(struct context* context, char* nom, struct variable valeur){
	if(context == NULL){
		return ;
	}
	struct map_noeud* ptr = NULL;
	struct context* ptrCont = context;
	ptr = ptrCont->tete;
	while(ptr != NULL){
		if(strcmp(ptr->nom, nom) == 0){
			ptr->var = valeur;
			return;
		}
		ptr = ptr->suivant;
	}
	printf("Avertissement SetVar: variable %s indéfinie\n", nom);
	//Temporaire jusqu'au fragment 2
	//contextInsert(context, nom, valeur);
}

void 	contextInsert(struct context* context, char* nom, struct variable valeur){
	if(context == NULL){
		return ;
	}
	
	struct map_noeud* context_noeud = (struct map_noeud*)malloc(sizeof(struct map_noeud));
	context_noeud->nom = (char*) malloc ((strlen(nom) + 1) * sizeof(char));
	context_noeud->nom = nom;
	context_noeud->var = valeur;
	
	context_noeud->suivant = context->tete;
	context->tete = context_noeud;
}

struct context* contextCreer(){
	struct context* cont = (struct context*) malloc (sizeof(struct context));
	cont->tete = NULL;
	return cont;
}

void contextSupprimer(struct context* context){
	if(context == NULL){
		return ;
	}
	struct map_noeud* ptr = context->tete;
	struct map_noeud* tmp;
	while(ptr != NULL){
		tmp = ptr->suivant;
		free(ptr);
		ptr = tmp;
	}
	free(context);
}


void contextAfficher(struct context* context){
	struct map_noeud* ptr = NULL;
	if(context == NULL){
		printf("Context non existant!\n");
		return;
	}
	ptr = context->tete;
	while(ptr != NULL){
		switch (ptr->var.type){
			case var_type_booleen:
				printf("[%s, %d, %s], ", ptr->nom, ptr->var.type, ptr->var.valeur.booleen ? "Vrais" : "Faux");
				break;
			case var_type_reel:
				printf("[%s, %d, %g], ", ptr->nom, ptr->var.type, ptr->var.valeur.reel);
				break;
			case var_type_string:
				printf("[%s, %d, %s], ", ptr->nom, ptr->var.type, ptr->var.valeur.string);
				break;
			default:
				printf("[%s, Undef], ", ptr->nom);
				break;
		}
		
		ptr = ptr->suivant; 
	}
	printf("\n");
}

void push(struct machineJS* jsm, struct variable donnee){
	jsm->PILE[++(jsm->SP)] = donnee;	
}

struct variable pop(struct machineJS* jsm){
	struct variable donnee = jsm->PILE[jsm->SP];
	--jsm->SP;
	return donnee;	
}

struct variable pull(struct machineJS* jsm){
	return jsm->PILE[jsm->SP];
}



struct machineJS* creerJSM(){
	struct machineJS* jsm = (struct machineJS*) malloc (sizeof(struct machineJS));
	jsm->CODE = (struct instruction*)malloc(MAX_CODE * sizeof(struct instruction));
	if(jsm->CODE == NULL){
		perror("Allocation section CODE erreur");
		exit(-1);
	}
	jsm->PILE = (struct variable*) malloc(MAX_PILE * sizeof(struct variable));
	if(jsm->PILE == NULL){
		perror("Allocation section PILE erreur");
		exit(-1);
	}
	jsm->RAM = (struct variable*) malloc(MAX_RAM * sizeof(struct variable));
	if(jsm->RAM == NULL){
		perror("Allocation section RAM erreur");
		exit(-1);
	}
	jsm->CONTEXT = contextCreer();
	jsm->SP = -1;
	jsm->PC = 0;
	jsm->CC = jsm->CONTEXT;
}

void afficherJSM(struct machineJS* jsm)
{
	printf("CC: ");
	contextAfficher(jsm->CC);
	printf("PILE: ");
	int i;
	for(i = jsm->SP; i >= 0; i--){
		if(jsm->PILE[i].type == var_type_reel){
			printf("[%g], ", jsm->PILE[i].valeur.reel);
		}else if(jsm->PILE[i].type == var_type_booleen){
			printf("[%s], ", jsm->PILE[i].valeur.booleen ? "Vrais" : "Faux");
		}else if(jsm->PILE[i].type == var_type_string){
			printf("[%s], ", jsm->PILE[i].valeur.string);
		}else{
			printf("[Undef], ");
		}
	}
	printf("\n");
	printf("PC: %d, SP: %d\n", jsm->PC, jsm->SP);
}

void  decoderASM (const char* nomFichier, struct machineJS* jsm)
{
	FILE* fileASM = fopen(nomFichier, "r");
	if(fileASM == NULL) {
		perror("erreur fichier ASM");
		return;
	}
	char* ligne;
	char* tok;
	int i, j = 0;
	int taille = sizeof(list_exec_fonctions)/sizeof(struct instr_machine);
	while( (ligne = lire_ligne_fichier (fileASM) ) !=NULL ) { 	
		//printf("ligne : %s;\n", ligne);
		tok = strtok(ligne, " \n");
		//printf("decodage instr : %s", tok);				
		for(i = 0; i < taille; i++)
		{ 				
			if(strcmp(tok, list_exec_fonctions[i].nom_instruction) == 0)
			{
				//init instruction
				jsm->CODE[j].cst = (union donnee){.reel = 0};
				jsm->CODE[j].offset = 0;
				jsm->CODE[j].ident = NULL;
				jsm->CODE[j].code_oper = i;	
				
				if(    (strcmp(tok, SAUT)     == 0) 
					|| (strcmp(tok, SAUTCOND) == 0)
				)
				{ 
					tok = strtok(NULL, " \n");			
					jsm->CODE[j].offset = atoi(tok);
				
				}
				else if(   (strcmp(tok, SETVAR) == 0) 
				         ||(strcmp(tok, GETVAR) == 0)
				         ||(strcmp(tok, DCLVAR) == 0)
				)
				{ 
					tok = strtok(NULL, " \n");
					jsm->CODE[j].ident = (char*) malloc((strlen(tok) + 1) * sizeof(char));
					strcpy(jsm->CODE[j].ident, tok);
				
				}
				else if((strcmp(tok, CSTRE) == 0))
				{ 
					tok = strtok(NULL, " \n");
					jsm->CODE[j].cst = (union donnee)atof(tok);		
				}
				else if((strcmp(tok, CSTBO) == 0))
				{ 
					tok = strtok(NULL, " \n");
					//printf(" %s", tok);
					if(strcmp(tok, "Vrais") == 0){					
						jsm->CODE[j].cst = (union donnee){.booleen = true};
					}else{
						jsm->CODE[j].cst = (union donnee){.booleen = false};
					}
				}
				else if((strcmp(tok, CSTSTRING) == 0))
				{ 
					tok = strtok(NULL, "\n");
					char* tmp;
					if(tok != NULL){
						tmp = (char*)malloc((strlen(tok) + 1) * sizeof(char));
						strcpy(tmp, tok);
					}else{
						tmp = strdup("");
					}
					jsm->CODE[j].cst = (union donnee){.string = tmp};
					//printf(" %s", tok);									
				}
				j++; 								
				break;
			}
		}
		//printf("\n");
		free(ligne);//Allouer dans la fonction lire_ligne_fichier
	}
	//printf("%d instruction au total\n", j);
	fclose(fileASM);
}
void  demmarerJSM(struct machineJS* jsm, bool affiche_exec){
	const struct instr_machine* instrCourante;
	if(affiche_exec)
	{
		printf("-------------Demarrage de la machine JS--------------------\n");
		afficherJSM(jsm);
		printf("Appuyez sur la touche entrée pour continuer...\n");
	}
	for(jsm->PC = 0; jsm->PC < MAX_CODE;){
		instrCourante = &list_exec_fonctions[jsm->CODE[jsm->PC].code_oper];
		if(affiche_exec)
		{
			fgetc(stdin);
			printf("Execution de l'instruction: %8s, offset %3d, cst %d, ident (%s)\n",
				instrCourante->nom_instruction, 
				jsm->CODE[jsm->PC].offset, 
				(int)jsm->CODE[jsm->PC].cst.reel,
				jsm->CODE[jsm->PC].ident
			);
		}
		
		instrCourante->instrFunct(jsm);
	
		if(affiche_exec)
		{
			afficherJSM(jsm);
		}

		if(instrCourante->instrFunct == exec_arreter)
			break;
	}
}

void  arreterJSM(struct machineJS* jsm)
{
	free(jsm->CODE);
	free(jsm->PILE);
	free(jsm->RAM);
	contextSupprimer(jsm->CONTEXT);
	free(jsm);
}