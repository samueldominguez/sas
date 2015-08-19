/*
 *  label.c
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include "label.h"

void init_label_table()
{
	label_count = 0;
}

void add_label(char *s, int ram_address)
{
	strcpy(label_table[label_count].name, s);
	label_table[label_count].ram_address = ram_address;
	label_count++;
}
