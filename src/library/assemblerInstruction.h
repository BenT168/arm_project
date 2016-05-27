#ifndef ASSEMBLER_INSTRUCTION_TYPE
#define ASSEMBLER_INSTRUCTION_TYPE

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//////////////////////////// ASSEMBLER /////////////////////////////////////

#define g_pair(x) { x, #xs }
////////////////////////// Definition of Mnemonic ////////////////////////////

#define mnemonic_tostring(f) \
        g(add) g(sub) g(rsb) g(and) g(eor) g(orr) g(mov) g(tst)
        g(tqe) g(cmp) g(mul) g(mla) g(ldr) g(str) g(beq) g(bne)
        g(bge) g(blt) g(bgt) g(ble) g(b)   g(lsl) g(andeq)

typedef enum Mnemonic
{
    //Data Processing
    add = 4,
    sub = 2,
    rsb = 3,
    and = 0,
    eor = 1,
    orr = 12,
    mov = 13,
    tst = 8,
    tqe = 9,
    cmp = 10,
    //Multiply
    mul, mla,
    //Single Data Transfer
    ldr, str,
    //Branch
    beq, bne, bge, blt, bgt, ble, b,
    //Special
    lsl, andeq
} Mnemonic;

struct Mnemonic_toStringStructType { /* Mnemoic_toString is implemented as */
  Mnemonic num;  /* name of the Mnemonic */
  char *str; /* string of the mmemonic within in */
};

Mnemonic_array[] = { Mnemonic_toString(g_pair) };

typedef struct Mnemonic_toStringStructType *Mnemonic_toString;

/////////////////////////////// Opcode ///////////////////////////////////

#define Opcode_toString g() \
g(AND) g(EOR) g(SUB) g(RSB) g(ADD) g(TST) g(TEQ) g(CMP) g(ORR) g(MOV)

struct Opcode_toStringStructType { /* Opcode_toString is implemented as */
  Opcode num;  /* name of the opcode */
  char *str; /* string where opcode within in */
};

Opcode_array[] = { Opcode_toString(g_pair) };

typedef struct Opcode_toStringStructType *Opcode_toString;

//////////////////////////// Cond //////////////////////////////////////////

#define Cond_toString g() \
g(EQ) g(NE) g(GE) g(LT) g(GT) g(LE) g(AL)

Struct Cond_toStringStructType { /* Cond_toString is implemented as */
  Cond num;  /* name of the Cond */
  char *str; /* string where the cond within in */
};

Cond_array[] = { Cond_toString(g_pair) };

typedef struct Cond_toStringStructType *Cond_toString;

//////////////////////////// Shift Type ////////////////////////////////////

#define ShiftType_toString g() \
g(LSL) g(LSR) g(ASR) g(ROR)

Struct ShiftType_toStringStructType { /* ShiftType_toString is implemented as */
  ShiftType num;  /* name of the shift type */
  char *str; /* string where the shift type within in */
};

SHiftType_array[] = { ShiftType_toString(g_pair) };

typedef struct ShiftType_toStringStructType *ShiftType_toString;

/////////////////////////// String_to_Enum ////////////////////////////////////


#define STR_TO_ENUM(a) int str_to_enum(char *buffer)     \
{                                                        \
  for (int x = 0 ; x < strlen(buffer) ; x++)             \
  {                                                      \
    buffer[x] = tolower(buffer[x]);                      \
  }                                                      \
  int len = sizeof(enum_array)/sizeof(enum_array[0]);    \
  for (int i = 0; i < len; i++)                          \
	{	                                                     \
		char *low_x = strtolwr(enum_array[i].str);           \
		if (strcmp(x, low_x) == 0)                           \
		{                                                    \
			return enum_array[i].num;                          \
		}                                                    \
	}                                                      \
	return -1;                                             \
}


STR_TO_ENUM(mnemonic)
STR_TO_ENUM(opcode)
STR_TO_ENUM(cond)
STR_TO_ENUM(shiftType)


#endif
