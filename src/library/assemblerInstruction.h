#ifndef _INSTRUCTION_ASSEMBLER_TYPE_
#define _INSTRUCTION_ASSEMBLER_TYPE_

#include <stdint.h>
#include <stdlib.h>


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


////////////////////////// REGISTER DEFINITIONS  //////////////////////////////

typedef enum
{
    r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16
} Register;

////////////////////////// Definition of Mnemonic ////////////////////////////

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

typedef enum Cond
{
    eq =  0,
    ne =  1,
    ge = 10,
    lt = 11,
    gt = 12,
    le = 13,
    al = 14
} Cond;


#endif
