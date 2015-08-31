/*
 *  sas.c
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "sas.h"
#include "opt.h"
#include "output.h"
#include "label.h"
#include "instruction.h"
#include "directive.h"

int yyparse(void);
extern FILE *yyin;
extern void write_sof(FILE *out);

struct opt_t opts; /* this is where all the specified options are stored */
char defout[] = "a.o"; /* default object file name for output */
/*union value lvalue; this is used by yacc/lex */
int sas_error = 0; /* if there is an error parsing the file, this is 1 */
int lines = 0; /* global line count */
struct instr instruction;

static void display_usage(void)
{
	fprintf(stderr,
	"sas - simple dcpu-16 assembler; version %.2f\n", VERS);
	fprintf(stderr, "usage: sas [options] [asmfiles]\n");
	fprintf(stderr, "options:\n");
	fprintf(stderr,
	"  -o outfile		write object to outfile, default name is a.o\n");
	fprintf(stderr,
	"  --version		display version number and exit\n");
	fprintf(stderr,
	"  -h, --help		display this help and exit\n");
}

static void display_version(void)
{
	fprintf(stderr, "sas version: %.2f\n", VERS);
}

static void suggest_help(void)
{
	fprintf(stderr, "sas: type `sas -h' for help\n");
}

void error(char *s)
{
	fprintf(stderr, "sas: error: %s\n", s);
}

static void opts_free(void)
{
	opts.asm_fcount--;
	for (; opts.asm_fcount >= 0; opts.asm_fcount--) {
		if (opts.asm_fname[opts.asm_fcount] != NULL) {
			free(opts.asm_fname[opts.asm_fcount]);
			opts.asm_fname[opts.asm_fcount] = NULL;
		}
	}
	free(opts.asm_fname);
}

static void close_files(FILE **asmfiles, int n)
{
	for (--n; n >= 0; --n) {
		if (asmfiles[n]){
			fclose(asmfiles[n]);
		}
	}
}

static void handle_args(int argc, char **argv)
{
	char opt = 0;
	int longind = 0;
	int outcount = 0;
	char **hdata = NULL;
	char *obj_file = NULL;
	static const char *optstring = "ho:";
	static const struct option longopts[] = {
		{"help", no_argument, NULL, 0},
		{"version", no_argument, NULL, 0},
	};

	opts.asm_fname = NULL;
	opts.obj_fname = NULL;
	opts.obj_name_spec = 0;
	opts.asm_fcount = 0;	

	opt = getopt_long(argc, argv, optstring, longopts, &longind);

	while (opt != -1) {
		switch (opt) {
		case 'o':
			if (outcount == 0) {
				opts.obj_name_spec = 1;
				opts.obj_fname = optarg;
				outcount++;
			} else {
				error("output file name already defined");
				exit(EXIT_FAILURE);
			}
			break;
		case 'h':
			display_usage();
			exit(EXIT_FAILURE);
			break;
		case '?':
			suggest_help();
			exit(EXIT_FAILURE);
		case 0:
			if (strcmp("help", longopts[longind].name) == 0) {
				display_usage();
				exit(EXIT_FAILURE);
			} else if (strcmp("version",
					longopts[longind].name) == 0) {
				display_version();
				exit(EXIT_FAILURE);
			}
			break;
		}
		opt = getopt_long(argc, argv, optstring, longopts, &longind);
	}

	if (optind == argc) {
		error("no input files specified");
		suggest_help();
		exit(EXIT_FAILURE);
	} else {
		while (optind < argc) {
			hdata = realloc(opts.asm_fname, sizeof (char*) * (opts.asm_fcount + 1));
			if (hdata == NULL) {
				error("memory error.");
				opts_free();
				exit(EXIT_FAILURE);
			}
			opts.asm_fname = hdata;
			opts.asm_fname[opts.asm_fcount] = malloc(sizeof (char) * strlen(argv[optind]) + 1);
			if (opts.asm_fname[opts.asm_fcount] == NULL) {
				error("memory error.");
				opts_free();
				exit(EXIT_FAILURE);
			}
			strcpy(opts.asm_fname[opts.asm_fcount], argv[optind]);
			opts.asm_fcount++;
			optind++;
		}
	}
	if (opts.obj_name_spec == 0) {
		opts.obj_fname = defout;
	}
}
 
int main(int argc, char **argv)
{
	char errstr[BUFSIZ];
	char line[BUFSIZ];
	FILE* asmfiles[BUFSIZ];
	FILE *objfile = NULL;
	int i;

	currw = 0x0000;
	start_write_address = 0x0000;

	handle_args(argc, argv);
	memset(ram, 0x0000, RAM_SIZE);

	for (i = 0; i < opts.asm_fcount; ++i) {
		printf("asm file[%d]: '%s'\n", i + 1, opts.asm_fname[i]);
		asmfiles[i] = fopen(opts.asm_fname[i], "r");
		if (asmfiles[i] == NULL) {
			sprintf(errstr, "couldn't open '%s'", opts.asm_fname[i]);
			error(errstr);
			error("perhaps it doesn't exist?");
			opts.asm_fcount = i;
			goto exit;
		}
	}
	if (opts.obj_name_spec) printf("obj file: '%s'\n", opts.obj_fname);
	else printf("obj file: '%s'\n", defout);

	init_label_table();
	init_undefined_table();
	init_dat_dir();

	lines = 0;
	for (i = 0; i < opts.asm_fcount; ++i) {
		yyin = asmfiles[i];
		yyparse();
	}
/*	for (i = 0; i < currw; ++i) printf("0x%04x\n", ram[i]);*/
	objfile = fopen(opts.obj_fname, "w");
	if (objfile == NULL) {
		sprintf(errstr, "couldn't open '%s' for output", opts.obj_fname);
		error(errstr);
	} else {
		write_sof(objfile);
		fclose(objfile);
	}
	exit:
	close_files(asmfiles, opts.asm_fcount);
	opts_free();
	exit(EXIT_SUCCESS);
}
