/*
 *  label.c
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include "label.h"

extern int sas_error;

void init_label_table()
{
	label_count = 0;
}

void init_undefined_table()
{
	undefined_symbols_count = 0;
}

void add_label(char *s, int ram_address)
{
	char err[86];
	if (is_redefine(s)) {
		sprintf(err, "redefining symbol '%s'", s);
		error(err);
		sas_error = 1;
		return;
	}
	strcpy(label_table[label_count].name, s);
	label_table[label_count].ram_address = ram_address;
	label_count++;
}

void add_undefined(char *s, int ram_address)
{
	strcpy(undefined_symbols[undefined_symbols_count].name, s);
	undefined_symbols[undefined_symbols_count].ram_address = ram_address;
	undefined_symbols_count++;
}

int check_undefined_at_address(int address, struct label *undefined)
{
	int i;

	for (i = 0; i < undefined_symbols_count; ++i) {
		if (undefined_symbols[i].ram_address == address) {
			strcpy(undefined->name, undefined_symbols[i].name);
			undefined->ram_address = undefined_symbols[i].ram_address;
			return 1;
		}
	}
	return 0;
}

int is_redefine(char *s)
{
	int i;
	for (i = 0; i < label_count; ++i) {
		if (strcmp(label_table[i].name, s) == 0) return 1;
	}
	return 0;
}
