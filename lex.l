/*
 *  lex.l
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
%{
#include <stdio.h>
#include "yacc.h"
#include "output.h"

extern int sas_error;
extern int lines;
%}

%option noinput
%option nounput

ws			[ \t]
letter		[A-Za-z]
digit		[0-9]
hexdigit	[0-9a-fA-F]
symbolchar	{letter}|[\.$_]
symbol		{symbolchar}({symbolchar}|{digit})*
gpreg		[ABCXYZIJabcxyzij]
xreg		SP|PC|EX|POP|PEEK|PUSH|PICK|sp|pc|ex|pop|peek|push|pick
op2			SET|ADD|SUB|MUL|MLI|DIV|DVI|MOD|MDI|AND|[BX]OR|SH[LR]|ASR|IF[BCENGALU]|ADX|SBX|ST[ID]
op2_lc		set|add|sub|mul|mli|div|dvi|mod|mdi|and|[bx]or|sh[lr]|asr|if[bcengalu]|adx|sbx|st[id]
op1			JSR|HCF|INT|RFI|IA[GSQ]|HW[NQI]
op1_lc		jsr|hcf|int|rfi|ia[gsq]|hw[nqi]


%%

:{symbol}			{ 
					yylval.string = yytext + 1;
					return LABEL;
				}
{symbol}:			{ 
					yylval.string = yytext;
					yytext[strlen(yytext) - 1] = '\0';
					return LABEL;
				}
0x{hexdigit}+			{
					yylval.integer = str2num(yytext);	
					return NUMBER;
				}
{digit}+			{ 
					yylval.integer = str2num(yytext);	
					return NUMBER;
				}
{gpreg}|{xreg}			{ 
					yylval.integer = str2reg(yytext);
					return REG;
				}
{op2}|{op2_lc}			{ 
					yylval.integer = str2opcode(yytext);
					return OP2;
				}
{op1}|{op1_lc}			{ 
					yylval.integer = str2opcode(yytext);
					return OP1;
				}
{symbol}			{ 
					yylval.string = yytext;
					return SYMBOL;
				}
\"(\\.|[^\\"])*\"		{ 
					yylval.string = yytext;
					return STRING;
				}

[,\+\[\]\n]				return *yytext;

[ \t\r]+			; /* whitespace + DOS line endings */	
;.*				; /* comment */
.				{
					fprintf(stderr, 
					"error: line %d: unknown character '%c'\n",
					lines, *yytext);
					sas_error = 1;
				}

%%

void yyerror(char *s)
{
	fprintf(stdout, "line %d: %s\n", lines, s);
}

int yywrap(void)
{
	return 1;
}
