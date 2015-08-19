#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "sas.h"

union opword {
	u16 raw;
	struct {
		u8 o : 5;
		u8 b : 5:
		u8 a : 6;
	} __attribute__((__packed__)) basic;
	struct {
		u8 zeros : 5;
		u8 o	 : 5;
		u8 a	 : 6;
	} __attribute__((__packed__)) nonbasic;
};

struct instr {
	union opword opword;
	u16 word2;
	u16 word3;
};

#endif /* INSTRUCTION_H */
