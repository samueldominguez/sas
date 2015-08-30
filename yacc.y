/*
 *  yacc.y
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
%{
#include <stdio.h>
#include "output.h"
#include "label.h"
#include "instruction.h"

void yyerror();
void error();
int yylex();

extern int lines;
extern struct label label_table[];
extern char *yytext;

struct instr instruction;
struct oper oper;
%}

%union {
	int integer;
	char *string;
	struct oper *oper;
}

%token <string> SYMBOL
%token <string> LABEL
%token <string> STRING
%token <integer> NUMBER
%token <integer> REG
%token <integer> OP1
%token <integer> OP2


%type <integer> symbol expr
%type <oper> operand op_expr

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
	LABEL				{
						printf("label def: %s\n", yylval.string);
						add_label($1, currw);
					}
	;

statement:
	instruct			{
					 	if ((currw + (instruction.word_length - 1)) > 0xffff) {
							error("cpu ram limit exceeded, please modularize");
							YYACCEPT;
						}
						ram[currw] = instruction.opword.raw;
						if (instruction.word_length > 2) {
							ram[currw + 2] = instruction.word3;
							ram[currw + 1] = instruction.word2;
						} else if (instruction.word_length > 1) {
							ram[currw + 1] = instruction.word2;
						}
						currw += instruction.word_length;
					}
	;

instruct:

	OP2 operand ',' operand 	{
						instruction.word_length = 1;
						make_instruction($1, $2, $4, &instruction);
					}

	| OP1 operand			{
						instruction.word_length = 1;
						printf("making instruction\n");
						make_instruction($1, $2, NULL, &instruction);
					}
	;

operand:
	op_expr				{
						yylval.oper->is_indirect = 0;
						$$ = yylval.oper;
					}

	| '[' op_expr ']'		{
						yylval.oper->is_indirect = 1;
						$$ = yylval.oper;
					}
	;

op_expr:
	REG				{
						yylval.oper = make_operand(0, 0, OP_REG, $1, 0);
					}
	
	| expr				{
						printf("expr: %d\n", $1);
						yylval.oper = make_operand(0, 0, OP_WRD, 0, $1);
					}

	| REG expr  /* PICK n */	{
						yylval.oper = make_operand(0, 0, OP_REG_WRD, $1, $2);
					}

	| expr '+' REG			{
						yylval.oper = make_operand(0, 0, OP_REG_WRD, $3, $1);
					}

	| REG '+' expr			{
						yylval.oper = make_operand(0, 0, OP_REG_WRD, $1, $3);
					}	
	;

expr:
	NUMBER
				
	| symbol
	;

symbol:
	SYMBOL				{ 
						/* REFERENCED SYMBOL
						 * word 0xfffe is returned
						 * so that a word space is
						 * is kept, since 0x0000 will
						 * get compressed and 0xffff
						 * too */
						add_undefined($1, currw);
						$$ = 0xfffe;
					}
	;

%%
