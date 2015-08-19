#ifndef SYMBOL_H
#define SYMBOL_H

#include <string.h>
#include <stdio.h>
#include "sas.h"

struct symbol {
	char *name;
	int value;
};

struct usymbol {
	char *name;
	int pos;
};

struct symtable {
	struct symbol data[BUFSIZ];
	int i;
};

struct usymtable {
	struct usymbol data[BUFSIZ];
	int i;
};

struct symtable symbols;
struct usymtable usymbols;

void add_symbol(char *s, int val);
void add_usymbol(char *s, int pos);
int get_symbol(char *s, int pos);

#endif /* SYMBOL_H */
