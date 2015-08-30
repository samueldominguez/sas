/*
 *  yacc.y
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
%{
#include "output.h"
#include "label.h"
#include "instruction.h"
#include "directive.h"

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
%token <string> DAT
%token <integer> NUMBER
%token <integer> REG
%token <integer> OP1
%token <integer> OP2

%type <string> symbol
%type <oper> operand op_expr expr

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

	| dat
	;

instruct:

	OP2 operand ',' operand 	{
						instruction.word_length = 1;
						make_instruction($1, $2, $4, &instruction);
					}

	| OP1 operand			{
						instruction.word_length = 1;
						make_instruction($1, $2, NULL, &instruction);
					}
	;

dat:
	DAT dat_expr			{
						write_dat_dir();
						init_dat_dir();
					}
	;

dat_expr:
	NUMBER				{
						add_dat_element($1);
					}

	| STRING			{
						int i;
						int length = strlen($1);
						for (i = 0; i < length; ++i) add_dat_element((int) $1[i]);
					}
	| dat_expr ',' dat_expr
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
						yylval.oper = make_operand(0, 0, OP_REG, $1, 0, NULL);
					}
	
	| expr

	| REG expr  /* PICK n */	{
						set_operand_reg(yylval.oper, $1);
						set_operand_type(yylval.oper, OP_REG_WRD);
					}

	| expr '+' REG			{
						set_operand_reg(yylval.oper, $3);
						set_operand_type(yylval.oper, OP_REG_WRD);
					}

	| REG '+' expr			{
						set_operand_reg(yylval.oper, $1);
						set_operand_type(yylval.oper, OP_REG_WRD);
					}	
	;

expr:
	NUMBER				{
						yylval.oper = make_operand(0, 0, OP_WRD, 0, $1, NULL);
					}

	| symbol			{
						yylval.oper = make_operand(0, 0, OP_WRD, 0, 0x0000, $1);
					}
	;

symbol:
	SYMBOL				{ 
						$$ = yylval.string;
					}
	;

%%
