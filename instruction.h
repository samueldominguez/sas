#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "sas.h"

/* instruction types, OP2 means 2 arguments (set a, 2) OP1 means 1 argument */
#define	INS_DAT		0x0001
#define INS_OP2		0x0002
#define INS_OP1		0x0003

union opword {
	u16 raw;
	struct {
		u8 o : 5;
		u8 b : 5;
		u8 a : 6;
	} __attribute__((__packed__)) basic;
	struct {
		u8 zeros : 5;
		u8 o	 : 5;
		u8 a	 : 6;
	} __attribute__((__packed__)) nonbasic;
};

struct instr {
	int type; /* INS_DAT, INS_OP2, INS_OP1  */
	int word_length;
	union opword opword;
	u16 word2;
	u16 word3;
};

#endif /* INSTRUCTION_H */
