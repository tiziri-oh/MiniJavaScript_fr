/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_BISON_PARSEUR_TAB_H_INCLUDED
# define YY_YY_BISON_PARSEUR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NOMBRE = 258,
    IDENT = 259,
    BOOLEEN = 260,
    STRING = 261,
    SI = 262,
    SINON = 263,
    FAIRE = 264,
    POUR = 265,
    TANTQUE = 266,
    ECRIRE = 267,
    VAR = 268,
    FONCTION = 269,
    BREAK = 270,
    RETOURNER = 271,
    NON = 272,
    TYPEOF = 273,
    PLUS = 274,
    MOINS = 275,
    MUL = 276,
    DIV = 277,
    MODULO = 278,
    PUISSANCE = 279,
    AFFECT = 280,
    ET = 281,
    OU = 282,
    EGALE3 = 283,
    SUP = 284,
    INF = 285,
    DIFF = 286,
    SUP_EG = 287,
    INF_EG = 288,
    PLUS_EG = 289,
    MUL_EG = 290,
    MOINS_EG = 291,
    DIV_EG = 292,
    MODULO_EG = 293,
    PUISSANCE_EG = 294,
    PRIO1 = 295,
    PRIO2 = 296,
    PRIO3 = 297,
    FINSI = 298
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 17 "bison/parseur.y" /* yacc.c:1909  */

	char*  		identifiant;
	char  		operateur[8];
	double 		nombre;
	AST_Noeud*	ast_noeud;
	bool 		booleen;
	char* 		string;	 

#line 107 "bison/parseur.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (AST* ast);

#endif /* !YY_YY_BISON_PARSEUR_TAB_H_INCLUDED  */
