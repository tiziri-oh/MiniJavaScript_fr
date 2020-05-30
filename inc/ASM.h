#ifndef ASM_H
#define ASM_H
#include "AST.h"

//voire toutes les fonctions utiles pour un asm ainsi que pour compiler
void chercherInitBreak(AST_Noeud* noeud, AST_Noeud* boucle);
int compterInstrASTNoeud(AST_Noeud* noeud, int offset);
int compterInstrEntreASTNoeuds(AST_Noeud* source, AST_Noeud* destination, int offset, int* trouve);
void compilerAST(AST* ast, const char* nomFichier);
void compilerASTNoeud(AST_Noeud* noeud, FILE* file);
int inference_type(	AST_Noeud* noeud);

//Arithmetique
#define ADDITION			"AddRe"
#define SOUSTRACTION		"SubiRe"
#define MULTIPLICATION		"MultRe"
#define DIVISION			"DiviRe"
#define NEGATION			"NegaRe"
#define MODULO				"Modulo"
#define NON					"Not"
#define BOTORE				"BoToRe"
#define RETOBO				"ReToBo"

//Comparaison
#define EGAL				"Equal"
#define DIFFERENT			"NoEq"
#define INFERIEUREQ			"LowEqR"
#define SUPERIEUREQ			"GreEqR"
#define INFERIEURST			"LowStR"
#define SUPERIEURST			"GreStR"

//Jumps
#define SAUT				"Jump"
#define SAUTCOND			"ConJmp"
#define CAS					"Case"
#define CASP				"CaseP"

//Gestion de la pile
#define CSTRE				"CstRe"
#define CSTBO				"CstBo"
#define CSTINDEF			"CstUn"
#define COPY				"Copy"
#define SWAP				"Swap"
#define DROP				"Drop"
#define NOOP				"Noop"
#define TYPEDE				"TypeOf"

//Gestion des variables
#define SETVAR				"SetVar"
#define GETVAR				"GetVar"
#define DCLVAR				"DclVar"

//Gestion de l'arithmetique des String
#define CONCAT				"Concat"
#define CSTSTRING			"CstStr"
#define STTORE				"StToRe"
#define RETOST				"ReToSt"


//comparaison des string
#define INFERIEUREQS		"LowEqS"
#define SUPERIEUREQS		"GreEqS"
#define INFERIEURSTS		"LowStS"
#define SUPERIEURSTS		"GreStS"			
#define EST_VIDE_S			"IsEmpt"

//securite et appels systeme
#define ARRETER				"Halt"
#define ERREUR				"Err"
#define ASSERTION			"Assert"
#define AFFICHER			"Print"

#endif
