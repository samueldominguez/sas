/*
 *  yacc.y
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
%{
#include <stdio.h>
#include "output.h"
#include "symbol.h"
/*#include "instruction.h"*/

void yyerror();
int yylex();

extern int lines;
extern struct instr instruction;

%}

%union {
	int integer;
	char *string;
}

%token <string> SYMBOL LABEL STRING
%token <integer> NUMBER
%token <integer> REG
%token <integer> OP1 OP2

%type <integer> symbol op_expr operand expr

%defines "yacc.h"

%%

program:
	program line '\n'		
	| program '\n'			
	| program error '\n'		{ yyerrok; }
	|
	;

line:		{ ++lines; }
	statement
	| label
	| label statement
	;							

label:
	LABEL			{ 
					printf("symbol: %s\n", yylval.string);
					add_symbol(yylval.string, currw);
				}
	;

statement:
	instr
	;

instr:
	OP2 operand ',' operand				{ printf("OP2\n"); }
	| OP1 operand					{ printf("OP1\n"); }
	;

operand:
	op_expr
	| '[' op_expr ']'			{ printf("indirect\n"); }
	;

op_expr:
	REG
	| expr
	| REG expr  /* PICK n */		
	| expr '+' REG		
	| REG '+' expr		
	;

expr:
	NUMBER				
	| symbol
	;

symbol:
	SYMBOL				{ 
						$$ = get_symbol(yylval.string, currw);	
					}
	;

%%
