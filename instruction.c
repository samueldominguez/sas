/*
 *  instruction.c
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include "instruction.h"

extern u16 currw;

extern void sas_error(char *s);
extern void add_undefined(char *s, int ram_address);

struct oper *make_operand(int op_pos, int is_indirect, int oper_type, int reg, int word, char *symbol)
{
	struct oper *oper;

	oper = malloc(sizeof (struct oper));
	if (oper) {
		oper->op_pos = op_pos;
		oper->is_indirect = is_indirect;
		oper->oper_type = oper_type;
		oper->reg = reg;
		oper->word = word;
		if (symbol) {
			strncpy(oper->symbol_name, symbol, SYMBOL_MAX_LENGTH + 1);
			oper->is_symbol = 1;
		} else {
			oper->is_symbol = 0;
		}
	} else {
		sas_error("memory allocation failed for operand data structure");
	}
	return oper;
}

void set_operand_reg(struct oper *oper, int reg)
{
	if (oper) oper->reg = reg;
}

void set_operand_type(struct oper *oper, int type)
{
	if (oper) oper->oper_type = type;
}

/*
 * IMPORTANT: the first word of the instruction is in format aaaaaabbbbbooooo
 * however when refered to operands, the first operand (oper_a) is really
 * the 5 bit representation of 'b', and the second operand (oper_b) is really
 * the 6 bit representation of 'a' */
void make_instruction(int opcode, struct oper *oper_a, struct oper *oper_b, struct instr *instruction)
{
	/* is basic (OP2, oper, oper) */
	if (oper_b) {
		instruction->opword.basic.o = (u8) opcode;
		
		switch (oper_a->oper_type) {
		case OP_REG:
			instruction->opword.basic.b = (u8) oper_a->reg;
			if (oper_a->is_indirect) {
				instruction->opword.basic.b += 0x08;
			}
		break;
		case OP_WRD:
			if (oper_a->is_indirect) {
				instruction->opword.basic.b = IND_NW;
			} else {
				instruction->opword.basic.b = DIR_NW;
			}
			instruction->word2 = (u16) oper_a->word;
			instruction->word_length++;
		break;
		case OP_REG_WRD:
			instruction->opword.basic.b = (u8) (oper_a->reg + 0x10);
			instruction->word2 = (u16) oper_a->word;
			instruction->word_length++;
		break;
		default:
		;
		/* do nothing */
		}

		switch (oper_b->oper_type) {
		case OP_REG:
			instruction->opword.basic.a = (u8) oper_b->reg;
			if (oper_b->is_indirect) {
				instruction->opword.basic.a += 0x08;
			}
		break;
		case OP_WRD:
			if (oper_b->is_indirect) {
				instruction->opword.basic.a = IND_NW;
				if (instruction->word_length == 2) {
					instruction->word3 = instruction->word2;
					instruction->word2 = (u16) oper_b->word;
				} else {
					instruction->word2 = (u16) oper_b->word;
				}
				instruction->word_length++;
			} else {			
				if (can_be_compressed(oper_b->word) && oper_b->is_symbol == 0) {
					if (oper_b->word == 0xffff) {
						instruction->opword.basic.a = 0x20;
					} else {
						instruction->opword.basic.a = oper_b->word + 0x21;
					}
				} else {
					instruction->opword.basic.a = DIR_NW;
					if (instruction->word_length == 2) {
						instruction->word3 = instruction->word2;
						instruction->word2 = (u16) oper_b->word;
					} else {
						instruction->word2 = (u16) oper_b->word;
					}
					instruction->word_length++;
				}
			}
		break;
		case OP_REG_WRD:
			instruction->opword.basic.a = (u8) (oper_b->reg + 0x10);
			if (instruction->word_length == 2) {
				instruction->word3 = instruction->word2;
				instruction->word2 = (u16) oper_b->word;
			} else {
				instruction->word2 = (u16) oper_b->word;
			}
			instruction->word_length++;
		break;
		default:
		;
		/* do nothing */
		}
		free(oper_a);
		free(oper_b);
	} else { /* is nonbasic (OP1 oper) */
		instruction->opword.nonbasic.o = (u8) opcode;
		instruction->opword.nonbasic.zeros = 0;

		switch(oper_a->oper_type) {
		case OP_REG:
			instruction->opword.nonbasic.a = (u8) oper_a->reg;
			if (oper_a->is_indirect) {
				instruction->opword.nonbasic.a += 0x08;
			}
		break;
		case OP_WRD:
			if (oper_a->is_indirect) {
				instruction->opword.nonbasic.a = IND_NW;
				instruction->word2 = (u16) oper_a->word;
				instruction->word_length++;
			} else {			
				if (can_be_compressed(oper_a->word) && oper_a->is_symbol != 0) {
					if (oper_a->word == 0xffff) {
						instruction->opword.nonbasic.a = 0x20;
					} else {
						instruction->opword.nonbasic.a = oper_a->word + 0x21;
					}
				} else {
					instruction->opword.nonbasic.a = DIR_NW;
					instruction->word2 = (u16) oper_a->word;
					instruction->word_length++;
				}
			}
		break;
		case OP_REG_WRD:
			instruction->opword.nonbasic.a = (u8) (oper_a->reg + 0x10);
			instruction->word2 = (u16) oper_a->word;
			instruction->word_length++;
		break;
		default:
		;
		/* do nothing */
		}
		free(oper_a);	
	}
	if (oper_a->is_symbol) {
		if (instruction->word_length == 2) {
			add_undefined(oper_a->symbol_name, currw + 1);
		} else add_undefined(oper_a->symbol_name, currw + 2);
	}
	if (oper_b && oper_b->is_symbol) {
		add_undefined(oper_b->symbol_name, currw + 1);
	}
}

int can_be_compressed(int a)
{
	if ((a >= 0x0000 && a <= 0x1e) || a == 0xffff) return 1;
	else return 0;
}
