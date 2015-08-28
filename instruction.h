/*
 *  instruction.h
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "sas.h"

/* operand positions */
#define OP_POS_A	0x0001
#define OP_POS_B	0x0002

/* operand types */
#define OP_REG		0x0001
#define OP_WRD		0x0002
#define OP_REG_WRD	0x0003

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
	int word_length;
	union opword opword;
	u16 word2;
	u16 word3;
};

struct oper {
	int op_pos;
	int is_indirect;
	int oper_type;
	int reg;
	int word;
};

struct oper *make_operand(int op_pos, int is_indirect, int oper_type, int reg, int word);
void make_instruction(int opcode, struct oper *oper_a, struct oper *oper_b, struct instr *instruction);
int can_be_compressed(int a);

#endif /* INSTRUCTION_H */
