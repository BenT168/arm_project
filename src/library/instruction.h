#ifndef _INSTRUCTION_TYPE
#define _INSTRUCTION_TYPE

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

////////////////////////////// EMULATE /////////////////////////////////////

//////////////////// Structure of Instruction //////////////////////////////


typedef struct DataProcessingInstruct
{
    unsigned int Operand2 : 12; /* Second Operand 12-bits */
    unsigned int Rd       :  4; /* Destination register 4-bits */
    unsigned int Rn       :  4; /* First operand register 4-bits */
    unsigned int SetCond  :  1; /* Set condition codes 1-bit */
    unsigned int Opcode   :  4; /* Operation code 4-bits */
    unsigned int ImmOp    :  1; /* Immediate Operand field 1-bit */
    unsigned int _00      :  2; /* Nullity field 2-bits */
    unsigned int Cond     :  4; /* Condition field 4-bits */
} DataProcessingInstruct;


typedef struct MultiplyInstruct
{
  //Operand registers 4-bits each
    unsigned int Rm      : 4;
    unsigned int _1001   : 3;
    unsigned int Rs      : 4;
    unsigned int Rn      : 4;
    unsigned int Rd      : 4; /* Destination registers 4-bits */
    unsigned int SetCond : 1; /* Set Condition codes  1-bit */
    unsigned int Acc 	   : 1; /* Accumulate 1-bit */
    unsigned int _000000 : 6;
    unsigned int Cond 	 : 4; /* Condition field 4-bits */
} MultiplyInstruct;


typedef struct SDTInstruct
{
    unsigned int Offset : 12;
    unsigned int Rd     :  4;
    unsigned int Rn     :  4;
    unsigned int L      :  1;
    unsigned int _00    :  2;
    unsigned int U      :  1;
    unsigned int P      :  1;
    unsigned int I      :  1;
    unsigned int _01    :  2;
    unsigned int Cond   :  4;
} SDTInstruct;


typedef struct BranchInstruct
{
    unsigned int Offset : 24;
    unsigned int _0     :  1;
    unsigned int _101   :  3;
    unsigned int Cond   :  4;
} BranchInstruct;


///////////////////////////// Types of Shift /////////////////////////////////


typedef struct ImmReg
{
    unsigned int Imm      : 8;
    unsigned int Rotate   : 4;
} ImmReg;


typedef struct ShiftReg
{
    unsigned int Rm       : 4;
    unsigned int Flag     : 1;
    unsigned int Type     : 2;
    unsigned int Amount   : 5;
} ShiftReg;


typedef struct ShiftRegOptional
{
    unsigned int Amount   : 5;
    unsigned int Type     : 2;
    unsigned int Flag     : 1;
    unsigned int Rm       : 4;
} ShiftRegOptional;


////////////////////////// Definition of Mnemonic ////////////////////////////


typedef enum Cond
{
    EQ =  0,
    NE =  1,
    GE = 10,
    LT = 11,
    GT = 12,
    LE = 13,
    AL = 14
} Cond;


typedef enum Opcode
{
    AND =  0,
    EOR =  1,
    SUB =  2,
    RSB =  3,
    ADD =  4,
    TST =  8,
    TEQ =  9,
    CMP = 10,
    ORR = 12,
    MOV = 13
} Opcode;


typedef enum ShiftType
{
    LSL = 0, /* logical left */
    LSR = 1, /* logical right */
    ASR = 2, /* arithmetic right */
    ROR = 3  /* rotate right */
} ShiftType;

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
