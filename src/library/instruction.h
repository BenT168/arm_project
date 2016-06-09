#ifndef _INSTRUCTION_TYPE
#define _INSTRUCTION_TYPE

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

////////////////////////////////////////////////////////////////////////////
////////////////////////////// EMULATE /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

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
    unsigned int _1001   : 4;
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
    unsigned int Offset : 12; /* Offset 12-bits */
    unsigned int Rd     :  4; /* Destination register 4-bits */
    unsigned int Rn     :  4; /* First operand register 4-bits */
    unsigned int L      :  1; /* Load/Store bit 1-bit */
    unsigned int _00    :  2; /* Nullity field 2-bits */
    unsigned int Up     :  1; /* Up bit 1-bit */
    unsigned int P      :  1; /* Pre/Post indexing bit 1-bit */
    unsigned int ImmOff :  1; /* Immediate Offset 1-bit */
    unsigned int _01    :  2;
    unsigned int Cond   :  4; /* Condition field 4-bits */
} SDTInstruct;


typedef struct BranchInstruct
{
    unsigned int Offset : 24; /* Offset 24-bits */
    unsigned int _1010  :  4;
    unsigned int Cond   :  4; /* Condition field 4-bits */
} BranchInstruct;


typedef struct BDTInstruct //Block Data Transfer Instructions (extension)
{
    unsigned int RegList: 16; /* Register List 16-bits */
    unsigned int Rn     :  4; /* Destination register 4-bits */
    unsigned int L      :  1; /* Load/Store bit 1-bit */
    unsigned int W      :  1; /* Write back 1-bit */
    unsigned int S      :  1; /* PSR & Force User codes  1-bit */
    unsigned int Up     :  1; /* Up bit 1-bit */
    unsigned int P      :  1; /* Pre/Post indexing bit 1-bit */
    unsigned int _100   :  3;
    unsigned int Cond   :  4; /* Condition field 4-bits */
} BDTInstruct;


typedef struct SoftwareInterruptInstruct
{
    unsigned int Ignored : 24; /* Ignored by Processor 24-bits */
    unsigned int _1111   :  4;
    unsigned int Cond    :  4; /* Condition field 4-bits */
} SoftwareInterruptInstruct;

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
  unsigned int Rm       : 4;
  unsigned int Flag1    : 1;
  unsigned int Type     : 2;
  unsigned int Flag2    : 1;
  unsigned int Rs       : 4;
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
    MOV = 13,
    BEQ = 14,
    BNE = 15
} Opcode;


typedef enum ShiftType
{
    LSL = 0, /* logical left */
    LSR = 1, /* logical right */
    ASR = 2, /* arithmetic right */
    ROR = 3  /* rotate right */
} ShiftType;


//////////////////////////// ASSEMBLER /////////////////////////////////////

#define f_pair(x) { x, #x },

////////////////////////// Definition of Mnemonic ////////////////////////////

#define mnemonic_toString(f) \
        f(add) f(sub) f(rsb) f(and) f(eor) f(orr) f(mov) f(tst) \
        f(teq) f(cmp) f(mul) f(mla) f(ldr) f(str) f(beq) f(bne) \
        f(bge) f(blt) f(bgt) f(ble) f(b)   f(lsl) f(andeq) f(ldm) \
        f(stm) f(swi)

enum Mnemonic
{
    //Data Processing_RESULT
    and = 0, eor = 0, sub = 0, rsb = 0, add = 0, orr = 0,
    //Data_Processing_MOV
    mov = 1,
    //Data_Processing_CPSR
    tst = 2, teq = 2, cmp = 2,
    //Multiply_MUL
    mul = 3,
    //Multiply_MLA
    mla = 4,
    //Single Data Transfer
    ldr = 5, str = 5,
    //Branch
    beq = 6, bne = 6, bge = 6, blt = 6, bgt = 6, ble = 6, b = 6,
    // Block Data Transfer
    ldm = 7, stm = 7,
    // Software Interrupt
    swi = 8,
    //Special
    lsl = 9, andeq = 10,
} Mnemonic;

struct { /* Mnemoic_toString is implemented as */
  enum Mnemonic num;  /* name of the Mnemonic */
  char *str; /* string of the mmemonic within in */
} mnemonic_array[] = { mnemonic_toString(f_pair) };


/////////////////////////////// Opcode /////////////////////////////////////////


#define opcode_toString(f) \
	f(AND) f(EOR) f(SUB) f(RSB) f(ADD) f(TST) f(TEQ) f(CMP) f(ORR) f(MOV)

struct { /* Opcode_toString is implemented as */
  Opcode num;  /* name of the opcode */
  char *str; /* string where opcode within in */
} opcode_array[] = { opcode_toString(f_pair) };


//////////////////////////// Cond //////////////////////////////////////////


#define cond_toString(f) \
	f(EQ) f(NE) f(GE) f(LT) f(GT) f(LE) f(AL)

struct { /* Cond_toString is implemented as */
  Cond num;  /* name of the Cond */
  char *str; /* string where the cond within in */
} cond_array[] = { cond_toString(f_pair) };


//////////////////////////// Shift Type ////////////////////////////////////


#define shift_toString(f) \
	f(LSL) f(LSR) f(ASR) f(ROR)

struct { /* ShiftType_toString is implemented as */
  ShiftType num;  /* name of the shift type */
  char *str; /* string where the shift type within in */
} shift_array[] = { shift_toString(f_pair) };


/////////////////////////// String_to_Enum ////////////////////////////////////


#define STR_TO_ENUM(a) int str_to_##a(char *buffer)        \
{                                                          \
 	int len = sizeof(a##_array)/sizeof(a##_array[0]);        \
  	for (int i = 0; i < len; i++)                          \
	{	        					                                     \
    char *low_buffer = strdup(a##_array[i].str);           \
          for (int j = 0 ; j < strlen(low_buffer) ; j++ )  \
          {                                                 \
            if ( low_buffer[j] > 'z' || low_buffer[j] < 'a') { \
              low_buffer[j] = tolower(low_buffer[j]);           \
            }                                                  \
          }                                                     \
          if (strcmp(buffer, low_buffer) == 0)                   \
          {                                                 \
            return a##_array[i].num;                             \
		}                                                      \
    free(low_buffer); \
	}                                                            \
	return -1;                                                    \
}

STR_TO_ENUM(mnemonic)
STR_TO_ENUM(opcode)
STR_TO_ENUM(cond)
STR_TO_ENUM(shift)


#endif
