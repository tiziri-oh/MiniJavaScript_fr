
%{
#include <stdio.h> 	
#include <stdlib.h>
#include <string.h>
#include "AST.h"
#include <stdbool.h>

FILE* yyin;
int  yylex();
void yyerror(AST* ast ,const char* s, ...);
  
%}

%parse-param {AST* ast}

%union {
	char*  		identifiant;
	char  		operateur[8];
	double 		nombre;
	AST_Noeud*	ast_noeud;
	bool 		booleen;
	char* 		string;	 
}

%token	NOMBRE
%token	IDENT
%token  BOOLEEN
%token	STRING
%token  SI SINON FAIRE POUR TANTQUE ECRIRE VAR FONCTION BREAK RETOURNER
			 

%token NON TYPEOF PLUS MOINS MUL DIV MODULO PUISSANCE AFFECT ET OU
%token EGALE3 SUP INF DIFF SUP_EG INF_EG 
%token PLUS_EG MUL_EG MOINS_EG DIV_EG MODULO_EG PUISSANCE_EG	
			


%type <ast_noeud> programme commande expression affect_expr expressions arguments
%type <operateur> op_unair op_binair assigne 



%right AFFECT PLUS_EG MUL_EG MOINS_EG DIV_EG MODULO_EG PUISSANCE_EG
%right '?' ':'
%left OU
%left ET
%left DIFF EGALE3 
%left SUP INF SUP_EG INF_EG
%left PLUS MOINS 
%left MUL MODULO DIV  
%right PUISSANCE
%right NON TYPEOF
%nonassoc '(' ')'


%nonassoc PRIO1
%nonassoc PRIO2
%nonassoc PRIO3


%right FINSI SINON

%start debut


%%

debut : programme 											{
																ast->tete = creer_noeud_ast(AST_prog);
																ajouter_noeud_ast_fin(ast->tete, $1);
															}
	  ;

programme : commande										{								
																$$ = creer_noeud_ast(AST_cmds);
																ajouter_noeud_ast_debut($$, $1);
															}
											
			|commande programme								{
																ajouter_noeud_ast_debut($2, $1);
																$$  = $2;
															}
			;

commande : ';'												{	$$ = creer_noeud_ast(AST_cmd_nop);}

			| affect_expr ';'								{
																$$ = creer_noeud_ast(AST_cmd);
																ajouter_noeud_ast_fin($$, $1);
															}

			| FONCTION IDENT {$<identifiant>$ = yylval.identifiant;} 
			 '(' arguments ')' '{' programme '}' 
															{

																$$ = creer_noeud_ast(AST_fonc);
																AST_Noeud* ident =  creer_noeud_ast(AST_ident);
																ident->attribut.identifiant = $<identifiant>3;
																ajouter_noeud_ast_fin($$, ident);
																ajouter_noeud_ast_fin($$, $5);
																AST_Noeud* prog =  creer_noeud_ast(AST_prog);
																ajouter_noeud_ast_fin(prog, $8);
																ajouter_noeud_ast_fin($$, prog);
																AST_Noeud* list_cmds = prog->fils->debut->valeur;
																//pointer sur la derniere instruction de la fonction
																AST_Noeud* cmd = list_cmds->fils->fin->valeur;
																if(cmd->type != AST_retourner)
																{
																	ajouter_noeud_ast_fin(list_cmds, creer_noeud_ast(AST_retourner));
																}
															}
											
																		
			| '{' programme '}'								{
																$$ = $2;
															}

											
			| SI '(' expression ')' commande 	%prec FINSI	{
																$$ = creer_noeud_ast(AST_si);
																ajouter_noeud_ast_fin($$, $3);
																ajouter_noeud_ast_fin($$, $5);
															}
									
			| SI '(' expression ')' commande SINON commande	{
																	$$ = creer_noeud_ast(AST_si_sinon);
																	ajouter_noeud_ast_fin($$, $3);																	
																	ajouter_noeud_ast_fin($$, $5);
																	ajouter_noeud_ast_fin($$, $7);
																}

			| BREAK ';'										{
																$$ = creer_noeud_ast(AST_break);
															}
			
			| TANTQUE '(' expression ')' commande 			{
																	$$ = creer_noeud_ast(AST_tantque);
																	ajouter_noeud_ast_fin($$, $3);																	
																	ajouter_noeud_ast_fin($$, $5);
																}
			
			| FAIRE commande TANTQUE '(' expression ')'		{
																	$$ = creer_noeud_ast(AST_faire_tq);
																	ajouter_noeud_ast_fin($$, $2);																	
																	ajouter_noeud_ast_fin($$, $5);
																}

			| RETOURNER '(' expression ')' ';'				{
																$$ = creer_noeud_ast(AST_retourner);
																ajouter_noeud_ast_fin($$, $3);	
															}
			
			| POUR '('  affect_expr ';' expression ';' expression ')' commande {
																					$$ = creer_noeud_ast(AST_pour);
																					ajouter_noeud_ast_fin($$, $3);																	
																					ajouter_noeud_ast_fin($$, $5);
																					ajouter_noeud_ast_fin($$, $7);
																					ajouter_noeud_ast_fin($$, $9);
																				}
			
			| ECRIRE '(' expression ')' ';' 									{
																					$$ = creer_noeud_ast(AST_ecrire);
																					ajouter_noeud_ast_fin($$, $3);
																				}
			;


affect_expr : expression 															{$$ = $1;}
    		 | VAR IDENT {	$<identifiant>$ = yylval.identifiant;} 
    		   AFFECT expression 										{
			 		 	 													//fils 1 Noeud var
			 		 	 													AST_Noeud* decvar  = creer_noeud_ast(AST_var);
			 		 	 													AST_Noeud* ident_var = creer_noeud_ast(AST_ident);
			 		 	 													ident_var->attribut.identifiant = $<identifiant>3;
				 															ajouter_noeud_ast_fin(decvar, ident_var);							

				 															//fils 2 Noeud assign	 									
													 						AST_Noeud* assign = creer_noeud_ast(AST_assigne);
													 						strcpy(assign->attribut.operateur, "="); 
													 						AST_Noeud* ident_assign = creer_noeud_ast(AST_ident);
													 						ident_assign->attribut.identifiant = (char*) malloc ((strlen($<identifiant>3) + 1) * sizeof(char));
														 					strcpy(ident_assign->attribut.identifiant, $<identifiant>3);
																			ajouter_noeud_ast_fin(assign, ident_assign);
																			ajouter_noeud_ast_fin(assign, $5);

																			//noeud principal
				 															$$ = creer_noeud_ast(AST_var_affect);
				 															ajouter_noeud_ast_fin($$, decvar);
			 		 	 													ajouter_noeud_ast_fin($$, assign);
 		 	 															}
	    	 | VAR IDENT 												{
	    		 															$$ = creer_noeud_ast(AST_var);
	    		 															AST_Noeud* ident = creer_noeud_ast(AST_ident); 
				 															ident->attribut.identifiant = yylval.identifiant;
				 															ajouter_noeud_ast_fin($$, ident);
	    		 														}
    		 ;

expression : NOMBRE 						{
												$$ = creer_noeud_ast(AST_nombre);
												$$->attribut.nombre = yylval.nombre;
											}
			
			 | BOOLEEN						{
			 									$$ = creer_noeud_ast(AST_bool);
			 									$$->attribut.booleen = yylval.booleen;	
			 								}

			 | STRING 						{
			 									$$ = creer_noeud_ast(AST_string);
			 									$$->attribut.string = yylval.string;
			 								}
											
											
			 | IDENT						{$$ = creer_noeud_ast(AST_ident);
			 								 $$->attribut.identifiant = yylval.identifiant;
			 								}
			 
			 
			 | '(' expression ')'			{$$ = creer_noeud_ast(AST_exp_groupe);
			 									ajouter_noeud_ast_fin($$, $2);
			 								}
			 | IDENT { $<identifiant>$ = yylval.identifiant;} 
			   '(' expressions ')'			{	
			   									AST_Noeud* ident = creer_noeud_ast(AST_ident);
			   									ident->attribut.identifiant = $<identifiant>2;
			   									$$ = creer_noeud_ast(AST_appel);
			 									ajouter_noeud_ast_fin($$, ident);
			 									ajouter_noeud_ast_fin($$, $4);
			   								}


			 | op_unair expression	%prec PRIO3		{	
			 									$$ = creer_noeud_ast(AST_op_unair);
			 									strcpy($$->attribut.operateur, $1); 
			 									ajouter_noeud_ast_fin($$, $2);			 									
			 								}		 
			 
			 | expression op_binair expression	{
			 										$$ = creer_noeud_ast(AST_op_binair);
			 										strcpy($$->attribut.operateur, $2); 
											 		ajouter_noeud_ast_fin($$, $1);
											 		ajouter_noeud_ast_fin($$, $3);
			 								}	
			 								
			 | expression '?' expression ':' expression %prec PRIO2 {
			 												$$ = creer_noeud_ast(AST_op_ternaire);
											 				ajouter_noeud_ast_fin($$, $1);
											 				ajouter_noeud_ast_fin($$, $3);		
											 				ajouter_noeud_ast_fin($$, $5);		
			 											}
			 //https://perso.esiee.fr/~najmanl/compil/Bison/bison_6.html#SEC48
			 | IDENT {$<ast_noeud>$ = creer_noeud_ast(AST_ident); 
			 		  $<ast_noeud>$->attribut.identifiant = yylval.identifiant;
			 		 }  
			 		  assigne expression %prec  AFFECT{
			 									$$ = creer_noeud_ast(AST_assigne);
			 									strcpy($$->attribut.operateur, $3); 
											 	ajouter_noeud_ast_fin($$, $<ast_noeud>2);
											 	ajouter_noeud_ast_fin($$, $4);
			 							}			 
			 ;

expressions : 	 %empty 						{$$ = creer_noeud_ast(AST_exps);}
				| expression 					{
													$$ =  creer_noeud_ast(AST_exps);
													ajouter_noeud_ast_fin($$, $1);
												}
				| expression ',' expressions	{
												  //inverser l'ordre des expressions car utiliser comme pile de noms
												  ajouter_noeud_ast_fin($3, $1);
												  $$  = $3;
												}
				;


arguments :	%empty						{ $$ =  creer_noeud_ast(AST_args);} 
			| IDENT 					{
											$$ =  creer_noeud_ast(AST_args);
											AST_Noeud* ident =  creer_noeud_ast(AST_ident);
											ident->attribut.identifiant = yylval.identifiant;
											ajouter_noeud_ast_fin($$, ident);
										}
			| IDENT { 
						$<ast_noeud>$ =  creer_noeud_ast(AST_ident);
						$<ast_noeud>$->attribut.identifiant = yylval.identifiant;
					} ',' arguments		{ 
										  ajouter_noeud_ast_debut($4, $<ast_noeud>2);
										  $$  = $4;
										}
			;

op_unair :	MOINS 			{strcpy($$, "-");}
			| NON 			{strcpy($$, "!");}
			| TYPEOF   		{strcpy($$, "TypeOf");}
			
			;
			
		
			
	
op_binair :	  PLUS 	{strcpy($$, "+");}
			| MOINS {strcpy($$, "-");}
			| MUL 	{strcpy($$, "*");}
			| DIV 	{strcpy($$, "/");}
			| MODULO {strcpy($$, "%");}
			| EGALE3 {strcpy($$, "===");}
			| INF 	{strcpy($$, "<");}
			| SUP 	{strcpy($$, ">");}
			| DIFF	{strcpy($$, "!==");}
			| ET	{strcpy($$, "&&");}
			| OU	{strcpy($$, "||");}
			| PUISSANCE {strcpy($$, "**");}
			| SUP_EG {strcpy($$, ">=");}
			| INF_EG {strcpy($$, "<=");}
			;
			
assigne :	  AFFECT	{strcpy($$, "=");}
			| PLUS_EG	{strcpy($$, "+=");}
			| MUL_EG	{strcpy($$, "*=");}
			| MOINS_EG	{strcpy($$, "-=");}
			| DIV_EG	{strcpy($$, "/=");}
			| MODULO_EG	{strcpy($$, "%=");}
			| PUISSANCE_EG	{strcpy($$, "**=");}
			;

%%


  
#include <stdio.h>
#include <stdlib.h>

int parse(AST* ast, FILE* myfile) {
	yyin = myfile;
	do { 
	  yyparse(ast);
	} while(!feof(yyin));
	return 0;
}

void yyerror(AST* ast ,const char* s, ...){
  fprintf(stderr, "Parse error: %s\n", s);
  exit(1);
}

