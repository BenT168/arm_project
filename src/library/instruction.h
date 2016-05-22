#ifndef _INSTRUCTION_TYPE
#define _INSTRUCTION_TYPE

#include <stdint.h>
#include <stdlib.h>

//////////////////// Structure of Instruction //////////////////////////////

typedef struct DataProcessingInstruct
{
  unsigned int Operand2 : 12;/* Second Operand 12-bits */
  unsigned int Rd       : 4; /* Destination register 4-bits */
  unsigned int Rn       : 4; /* First operand register 4-bits */
  unsigned int SetCond  : 1; /* Set condition codes 1-bit */
  unsigned int Opcode   : 4; /* Operation code 4-bits */
  unsigned int ImmOp    : 1; /* Immediate Operand field 1-bit */
  unsigned int _00      : 2; /* Nullity field 2-bits */
  unsigned int Cond     : 4; /* Condition field 4-bits */
} DataProcessingInstruct;

typedef struct MultiplyInstruct
{
  //Operand registers 4-bits each
  unsigned int Rm      : 4;
  unsigned int _1001   : 3;
  unsigned int Rs      : 4;
  unsigned int Rn      : 4;
  unsigned int Rd 	   : 4; /* Destination registers 4-bits */
  unsigned int SetCond : 1; /* Set Condition codes  1-bit */
  unsigned int Acc 	   : 1; /* Accumulate 1-bit */
  unsigned int _000000 : 6;
  unsigned int Cond 	 : 4; /* Condition field 4-bits */

} MultyplyInstruct;

typedef struct SDTInstruct
{
    unsigned int Offset : 12;
    unsigned int Rd     : 4;
    unsigned int Rn     : 4;
    unsigned int L      : 1;
    unsigned int _00    : 2;
    unsigned int U      : 1;
    unsigned int P      : 1;
    unsigned int I      : 1;
    unsigned int _01    : 2;
    unsigned int Cond   : 4;
} SDTInstruct;

typedef struct BranchInstruct
{
    unsigned int Offset : 24;
    unsigned int _0     : 1;
    unsigned int _101   : 3;
    unsigned int Cond   : 4;
} BranchInstruct;

///////////////////////////// Types of Shift /////////////////////////////////

typedef struct ImmReg
{
    unsigned int Rotate    : 4;
    unsigned int Imm       : 8;
} ImmReg;

typedef struct ShiftReg
{
    unsigned int Rm       : 4;
    unsigned int Flag     : 1;
    unsigned int Type     : 2;
    unsigned int Amount   : 5;
} ShiftReg;

////////////////////////// Definition of Mnemonic ////////////////////////////

typedef enum Cond
{
    EQ = 0,
    NE = 1,
    GE = 10,
    LT = 11,
    GT = 12,
    LE = 13,
    AL = 14
} Cond;

typedef enum Opcode
{
    AND = 0,
    EOR = 1,
    SUB = 2,
    RSB = 3,
    ADD = 4,
    TST = 8,
    TEQ = 9,
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

#endif
