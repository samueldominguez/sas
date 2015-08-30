/*
 * directive.h
 * Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#ifndef DIRECTIVE_H
#define DIRECTIVE_H

#include "sas.h"
#include "output.h"

struct dat_dir {
        int dat_element[RAM_SIZE];
        int i;
};

struct dat_dir dat_dir;

void init_dat_dir();
void add_dat_element(int word);
void write_dat_dir();

#endif /* DIRECTIVE_H */
