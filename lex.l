/*
 *  lex.l
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
%{
#include <stdio.h>
#include "yacc.tab.h"
#include "output.h"

extern int sas_error;

int bad_unsigned_range(int a);
int bad_signed_range(int a);
int get_valid_number(char *s);
%}

%option noinput
%option nounput
%option yylineno

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
dat		DAT
dat_lc		dat
asciz		asciz
ascii8		ascii8
ascizlen	ascizlen
ascii8len	ascii8len
noasciz		noasciz
noascii8	noascii8
fill		fill

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
[\-]?0x{hexdigit}+			{
					yylval.integer = get_valid_number(yytext);
					return NUMBER;
				}
[\-]?{digit}+			{
					yylval.integer = get_valid_number(yytext);
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
{dat}|{dat_lc}			{
					return DAT;
				}
\.{asciz}			{
					return ASCIZ;
				}
\.{ascii8}			{
					return ASCII8;
				}
\.{ascizlen}			{
					return ASCIZLEN;
				}
\.{ascii8len}			{
					return ASCII8LEN;
				}
\.{noasciz}			{
					return NOASCIZ;
				}
\.{noascii8}			{
					return NOASCII8;
				}
\.fill				{
					return FILL;
				}
{symbol}			{ 
					yylval.string = yytext;
					return SYMBOL;
				}
\"(\\.|[^\\"])*\"		{
					if (strcmp(yytext, "") == 0) {
						yylval.string[0] = '\0';
					} else {
						yytext[strlen(yytext) - 1] = '\0';
						yylval.string = yytext + 1;
					}
					return STRING;
				}

[,\+\[\]\n]				return *yytext;

[ \t\r]+			; /* whitespace + DOS line endings */	
;.*				; /* comment */
.				{
					fprintf(stderr, 
					"error: line %d: unknown character '%c'\n",
					yylineno, *yytext);
					sas_error = 1;
				}

%%

void yyerror(char *s)
{
	fprintf(stdout, "line %d: %s\n", yylineno, s);
}

int yywrap()
{
	return 1;
}

/* 1 on BAD, 0 on GOOD */
int bad_unsigned_range(int a)
{
	if (a > 0xffff || a < 0x0000) return 1;
	else return 0;
}

/* following two's complement */
/* 1 on BAD, 0 on GOOD */
int bad_signed_range(int a)
{
	if (a < -0x8000) return 1;
	else return 0;
}

int get_valid_number(char *s)
{
	int value = str2num(s);

	/* signed */
	if (s[0] == '+' || s[0] == '-') {
		if (bad_signed_range(value)) {
			fprintf(stderr, "line %d: signed number '%s' out of 16 bit range, masked to 0x0000\n", yylineno, s);
			return 0x0000;
		} else {
			return value;
		}
	} else { /* unsigned */
		if (bad_unsigned_range(value)) {
			fprintf(stderr, "line %d: unsigned number '%s' out of 16 bit range, masked to 0x0000\n", yylineno, s);
			return 0x0000;
		} else {
			return value;
		}
	}
}
