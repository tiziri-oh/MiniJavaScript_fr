%{
#include <string.h>
#include <stdbool.h>
#include "AST.h"
#include "parseur.tab.h"
%}            


%%


[0-9]+\.?[0-9]*         {
						 yylval.nombre=atof(yytext);
						 return NOMBRE;
						}
[ \t\n]        			; /* ignore les blancs et tabulations */
[a-z][a-zA-Z0-9_]*    	{
							yylval.identifiant = (char*)malloc((strlen(yytext)+ 1) * sizeof(char));//strlen ne compte pas \0
							strcpy(yylval.identifiant , yytext); 
							return IDENT;
						}
(Vrais)|(Faux)  		{
							(strcmp(yytext, "Vrais") == 0) ? (yylval.booleen = true) : (yylval.booleen = false);					
							return BOOLEEN;
						}
\"[ !#-z]*\" 			{
							yylval.string = (char*)malloc((strlen(yytext) - 1) * sizeof(char));
							yytext[(strlen(yytext) - 1)] = '\0';
							strcpy(yylval.identifiant , &yytext[1]); 
							return STRING;
						}

Si			{return SI;}	
Sinon		{return SINON;}
Faire		{return FAIRE;}
Pour		{return POUR;} 
TantQue		{return TANTQUE;}
Ecrire		{return ECRIRE;}	
Var 		{return VAR;}
Retourner 	{return RETOURNER;}
Break		{return BREAK;}
Fonction 	{return FONCTION;}

"!" 		{return NON;}		
"TypeOf" 	{return TYPEOF;}
"+"			{return PLUS;}
"-" 		{return MOINS;}
"*"			{return MUL;}
"/"			{return DIV;}
"%"			{return MODULO;}
"==="		{return EGALE3;}
">"			{return SUP;}
"<"			{return INF;}
"!=="		{return DIFF;}
"&&"		{return ET;}
"||"		{return OU;}
"**"		{return PUISSANCE;}
">="		{return SUP_EG;}
"<="		{return INF_EG;}
"="			{return AFFECT;}
"+="		{return PLUS_EG;}
"*="		{return MUL_EG;}
"-="		{return MOINS_EG;}
"/="		{return DIV_EG;}
"%="		{return MODULO_EG;}
"**="		{return PUISSANCE_EG;}

\/\*(.*\n*)*\*\/ ;
\/\/.*\n	 	 ;

. 			{printf("nombre\n");return yytext[0];}


%%


int yywrap ()
{
  return (1);
}


