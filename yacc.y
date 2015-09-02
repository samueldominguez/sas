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
%token <string> ASCIZ
%token <string> ASCII8
%token <string> ASCIZLEN
%token <string> ASCII8LEN
%token <string> NOASCIZ
%token <string> NOASCII8
%token <string> NOASCIZLEN
%token <string> NOASCII8LEN
%token <integer> NUMBER
%token <integer> REG
%token <integer> OP1
%token <integer> OP2

%type <string> symbol
%type <oper> operand op_expr expr

%%

program:
	program line '\n'		
	| program '\n'			
	| program error '\n'		{ yyerrok; }
	|
	;

line:
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

	| directive
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

directive:
	dat
	| ASCIZ	STRING			{
						int i;
						int length = strlen($2);
						for (i = 0; i < length; ++i) add_dat_element((int) $2[i]);
						add_dat_element(0x0000);
						write_dat_dir();
						init_dat_dir();
					}

	| ASCII8 STRING			{
						int i;
						int length = strlen($2);
						u16 word;
						for (i = 0; i < length; ++i) {
							word = (int) $2[i] << 8;
							++i;
							if (i < length) word += (int) $2[i];
							add_dat_element((int) word);
						}
						if (length % 2 == 0) add_dat_element(0x0000);
						write_dat_dir();
						init_dat_dir();
					}
	
	| ASCIZLEN STRING		{
						int i;
						int length = strlen($2);
						add_dat_element(length);
						for (i = 0; i < length; ++i) add_dat_element($2[i]);
						write_dat_dir();
						init_dat_dir();
					}

	| ASCII8LEN STRING		{
						int i;
						int length = strlen($2);
						u16 word;
						add_dat_element(length);
						for (i = 0; i < length; ++i) {
							word = (int) $2[i] << 8;
							++i;
							if (i < length) word += (int) $2[i];
							add_dat_element((int) word);
						}
						write_dat_dir();
						init_dat_dir();
					}

	| NOASCIZ STRING		{
						int i;
						int length = strlen($2);
						for (i = 0; i < length; ++i) add_dat_element((int) $2[i]);
						write_dat_dir();
						init_dat_dir();
					}

	| NOASCII8 STRING		{
						int i;
						int length = strlen($2);
						u16 word;
						for (i = 0; i < length; ++i) {
							word = (int) $2[i] << 8;
							++i;
							if (i < length) word += (int) $2[i];
							add_dat_element((int) word);
						}
						write_dat_dir();
						init_dat_dir();
					}
	;

dat:
	DAT dat_expr			{
						write_dat_dir();
						init_dat_dir();
					}
	;

dat_expr:
	dat_elem

	| dat_elem ',' dat_expr
	;

dat_elem:
	NUMBER				{
						add_dat_element($1);
					}

	| STRING			{
						int i;
						int length = strlen($1);
						for (i = 0; i < length; ++i) add_dat_element((int) $1[i]);
					}
	;

operand:
	op_expr				{
						$1->is_indirect = 0;
						$$ = $1;
					}

	| '[' op_expr ']'		{
						$2->is_indirect = 1;
						$$ = $2;
					}
	;

op_expr:
	REG				{
						$$ = make_operand(0, 0, OP_REG, $1, 0, NULL);
					}
	
	| expr				{
						$$ = $1;
					}

	| REG expr  /* PICK n */	{
						set_operand_reg($2, $1);
						set_operand_type($2, OP_REG_WRD);
						$$ = $2;
					}

	| expr '+' REG			{
						set_operand_reg($1, $3);
						set_operand_type($1, OP_REG_WRD);
						$$ = $1;
					}

	| REG '+' expr			{
						set_operand_reg($3, $1);
						set_operand_type($3, OP_REG_WRD);
						$$ = $3;
					}	
	;

expr:
	NUMBER				{
						$$ = make_operand(0, 0, OP_WRD, 0, $1, NULL);
					}

	| symbol			{
						$$ = make_operand(0, 0, OP_WRD, 0, 0x0000, $1);
					}
	;

symbol:
	SYMBOL				{ 
						$$ = yylval.string;
					}
	;

%%
