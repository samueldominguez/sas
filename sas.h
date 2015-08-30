/*
 *  sas.h
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#ifndef SAS_H
#define SAS_H

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

typedef signed short s16;
typedef unsigned short u16;
typedef uint8_t u8;

#define VERS	0.4

#define ARRAY_SIZE(a) (sizeof (a) / sizeof (*a))

#define RAM_SIZE	0xffff

/* dcpu opcodes and registers */

#define NONBASIC 0x00
#define SET      0x01
#define ADD      0x02
#define SUB      0x03
#define MUL      0x04
#define MLI      0x05
#define DIV      0x06
#define DVI      0x07
#define MOD      0x08
#define MDI      0x09
#define AND      0x0a
#define BOR      0x0b
#define XOR      0x0c
#define SHR      0x0d
#define ASR      0x0e
#define SHL      0x0f
#define IFB      0x10
#define IFC      0x11
#define IFE      0x12
#define IFN      0x13
#define IFG      0x14
#define IFA      0x15
#define IFL      0x16
#define IFU      0x17
#define ADX      0x1a
#define SBX      0x1b
#define STI      0x1e
#define STD      0x1f

#define JSR      0x01
#define INT      0x08
#define IAG      0x09
#define IAS      0x0a
#define RFI      0x0b
#define IAQ      0x0c
#define HWN      0x10
#define HWQ      0x11
#define HWI      0x12

#define DIR_A    0x00
#define DIR_B    0x01
#define DIR_C    0x02
#define DIR_X    0x03
#define DIR_Y    0x04
#define DIR_Z    0x05
#define DIR_I    0x06
#define DIR_J    0x07
#define IND_A    0x08
#define IND_B    0x09
#define IND_C    0x0a
#define IND_X    0x0b
#define IND_Y    0x0c
#define IND_Z    0x0d
#define IND_I    0x0e
#define IND_J    0x0f
#define IND_NW_A 0x10
#define IND_NW_B 0x11
#define IND_NW_C 0x12
#define IND_NW_X 0x13
#define IND_NW_Y 0x14
#define IND_NW_Z 0x15
#define IND_NW_I 0x16
#define IND_NW_J 0x17
#define POPPUSH  0x18
#define PEEK     0x19
#define PICK     0x1a
#define DIR_SP   0x1b
#define DIR_PC   0x1c
#define DIR_EX   0x1d
#define IND_NW   0x1e
#define DIR_NW   0x1f

#endif /* SAS_H */
