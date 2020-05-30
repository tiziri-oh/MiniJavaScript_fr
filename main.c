#include <stdio.h>
#include <stdlib.h>
#include "AST.h"
#include "ASM.h"
#include "miniJSM.h"

int parse(AST* ast, FILE* myfile);

int main(int argc, char const *argv[])
{
	bool debug = false;
	
	if(argc < 3){
		printf("Usage : myJSCompiler <fichier_source_mini_javascript> <fichier_destination_assembleur> \n");
		exit(-1);
	}

	if(argc >= 4)
	{
		debug = true;
	}
	
	FILE* file = fopen(argv[1], "r");
	if(file == NULL){
		perror("fopen fichier source");
		return -2;
	}
	
	AST* ast = creerAST();

	parse(ast, file);

	fclose(file);

	printf("AST initial\n");
	afficher_ast(ast);
	printf("\n----------------------------------------------------------------\n");

	
	hosting(ast);

	printf("AST après hosting\n");
	afficher_ast(ast);	
	printf("\n----------------------------------------------------------------\n");

	
	//printf("debut de la compilation ...\n");
	
	compilerAST(ast, argv[2]);

	//printf("fin de la compilation ...\n");

	supprimer_ast(ast);

	if(debug)
	{
		struct machineJS* jsm = creerJSM();

		decoderASM (argv[2], jsm);

		demmarerJSM(jsm, debug);

		arreterJSM(jsm);
	}

	return 0;
}
