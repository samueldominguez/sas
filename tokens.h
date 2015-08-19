/*
 *  tokens.h
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#ifndef TOKENS_H
#define TOKENS_H

#include "symbol.h"

union value {
	int integer;
	char *string;
	struct symbol *dsymbol;
	struct symbol *usymbol;
};

#define OPCODE1		300
#define OPCODE2		301
#define GPREG		302
#define IND_GPREG	303
#define IND_GPREG_NW	304
#define XREG		305
#define IND_NW_TOK	308
#define NW		309
#define LABEL		310
#define	SYMBOL		311
#define NUMBER		312
#define STRING		313

#endif /* TOKENS_H */
