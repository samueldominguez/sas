/*
 *  output.c
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include "output.h"

int str2opcode(char *s)
{
	/* normal opcodes, OPCODE2 */
	if (EQ(s, "set")) return SET;
	if (EQ(s, "add")) return ADD;
	if (EQ(s, "sub")) return SUB;
	if (EQ(s, "mul")) return MUL;
	if (EQ(s, "mli")) return MLI;
	if (EQ(s, "div")) return DIV;
	if (EQ(s, "dvi")) return DVI;
	if (EQ(s, "mod")) return MOD;
	if (EQ(s, "mdi")) return MDI;
	if (EQ(s, "and")) return AND;
	if (EQ(s, "bor")) return BOR;
	if (EQ(s, "xor")) return XOR;
	if (EQ(s, "shr")) return SHR;
	if (EQ(s, "asr")) return ASR;
	if (EQ(s, "shl")) return SHL;
	if (EQ(s, "ifb")) return IFB;
	if (EQ(s, "ifc")) return IFC;
	if (EQ(s, "ife")) return IFE;
	if (EQ(s, "ifn")) return IFN;
	if (EQ(s, "ifg")) return IFG;
	if (EQ(s, "ifa")) return IFA;
	if (EQ(s, "ifl")) return IFL;
	if (EQ(s, "ifu")) return IFU;
	if (EQ(s, "adx")) return ADX;
	if (EQ(s, "sbx")) return SBX;
	if (EQ(s, "sti")) return STI;
	if (EQ(s, "std")) return STD;

	/* special opcodes, OPCODE1 */
	if (EQ(s, "jsr")) return JSR;
	if (EQ(s, "int")) return INT;
	if (EQ(s, "iag")) return IAG;
	if (EQ(s, "ias")) return IAS;
	if (EQ(s, "rfi")) return RFI;
	if (EQ(s, "iaq")) return IAQ;
	if (EQ(s, "hwn")) return HWN;
	if (EQ(s, "hwq")) return HWQ;
	if (EQ(s, "hwi")) return HWI;
	return 0;
}

int str2reg(char *s)
{
	if (EQ(s, "a")) return DIR_A;
	if (EQ(s, "b")) return DIR_B;
	if (EQ(s, "c")) return DIR_C;
	if (EQ(s, "x")) return DIR_X;
	if (EQ(s, "y")) return DIR_Y;
	if (EQ(s, "z")) return DIR_Z;
	if (EQ(s, "i")) return DIR_I;
	if (EQ(s, "j")) return DIR_J;
	if (EQ(s, "pop") || EQ(s, "push")) return POPPUSH;
	if (EQ(s, "peek")) return PEEK;
	if (EQ(s, "pick")) return PICK;
	if (EQ(s, "sp")) return DIR_SP;
	if (EQ(s, "pc")) return DIR_PC;
	if (EQ(s, "ex")) return DIR_EX;
	return 0;
}

int str2num(char *s)
{
	return (int) strtol(s, NULL, 0);
}
