/*
 *  label.c
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include "label.h"

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
