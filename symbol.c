/*
 *  symbol.c
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include "symbol.h"

static struct symbol sym;
static struct usymbol usym;

void add_symbol(char *s, int val)
{
	sym.name = s;
	sym.value = val;
	symbols.data[symbols.i++] = sym;
}

void add_usymbol(char *s, int pos)
{
	usym.name = s;
	usym.pos = pos;
	usymbols.data[usymbols.i++] = usym;
}	

int get_symbol(char *s, int pos)
{
	int i;
	for (i = 0; i < symbols.i; ++i) {
		if (strcmp(s, symbols.data[i].name) == 0) {
			return symbols.data[i].value;
		}
	}
	/* the symbol has not been defined up to here, so we save it
	 * in a table. when reach the end of all the streams (out of the
	 * scope of this function), we check if these symbols have been
	 * defined afterwards, and if they have we replace them with the
	 * intended values, else there is an error
  	 */
	add_usymbol(s, pos);
	return 0;
}
