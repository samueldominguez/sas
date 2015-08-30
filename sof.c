/*
 * sof.c
 * Copyright (C) 2015 Samuel Dominguez Lorenzo
 *
 * This is an implementation of the Simple Object Format (SOF)
 * See file SIMPLE_OBJECT_FORMAT for reference.
 */
#include <stdio.h>
#include <string.h>

#include "output.h"
#include "label.h"

/* max symbol pair length, then used for single binary section element.
 * max label/symbol length = 63 , max hex word length = 6, newline + space = 2
 * 63 + 6 + 2 = 71 (+ null byte = 72)
 */
#define ATOM_MAX_LENGTH		72

/* max hex word length: "0x" = 2 + 4 hex digits = 6 (+ null byte = 7)
 * + newline for convenience = 8
 */
#define WORD_MAX_LENGTH		 8

/* writes the sof to file "out", expects the stream to be
 * already open for writing
 */
void write_sof(FILE *out)
{
	struct label undefined;
	char line[ATOM_MAX_LENGTH];
	char word[WORD_MAX_LENGTH];
	int i;

	/* first write the first section */
	fputs("symbols:\n", out);

	/* now write defined symbols with their values */
	for (i = 0; i < label_count; ++i) {
		strcpy(line, label_table[i].name);
		strcat(line, " ");
		sprintf(word, "0x%04x\n", label_table[i].ram_address);
		strcat(line, word);
		fputs(line, out);
	}

	/* next section, binary */
	fputs("binary:\n", out);
	
	for (i = start_write_address; i < currw; ++i) {
		if (check_undefined_at_address(i, &undefined)) {
			fputs(undefined.name, out);
			fputs(" ", out);
		} else {
			sprintf(word, "0x%04x", ram[i]);
			fputs(word, out);
			fputs(" ", out);
		}
	}
	fputs("\nend", out);
}
