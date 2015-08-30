/*
 * directive.c
 * Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include "directive.h"

void init_dat_dir()
{
	dat_dir.i = 0;
}

void add_dat_element(int word)
{
	if (dat_dir.i == 0xffff) return;
	dat_dir.dat_element[dat_dir.i] = word;
	dat_dir.i++;
}

void write_dat_dir()
{
	int i;

	for (i = 0; i < dat_dir.i; ++i) ram[i + currw] = (u16) dat_dir.dat_element[i];
	currw += dat_dir.i;
}
