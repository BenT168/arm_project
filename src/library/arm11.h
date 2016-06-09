#ifndef _ARM_TYPE
#define _ARM_TYPE

#include <stdint.h>
#include <stdio.h>

#define MEMORY_CAPACITY 65536
#define REGISTER_COUNT  17
#define SIZE_OF_WORD    32

////////////////////////////   PIPELINE  //////////////////////////////////////

typedef struct Pipeline
{
    int32_t fetched;
    int32_t decoded;
} Pipeline;

//////////////////////////////// MODE ////////////////////////////////////////

typedef enum
{
   User       = 0x10,
   FIQ        = 0x11,
   IRQ        = 0X12,
   Supervisor = 0x13,
   Abort      = 0x17,
   Undefined  = 0x1b,
   System     = 0x1f
} ARM_Mode;

////////////////////////////  STATE OF ARM  ///////////////////////////////////

typedef struct ARM_State
{
    int8_t   memory[MEMORY_CAPACITY];
    int32_t  registers[REGISTER_COUNT];
    int32_t  SPSR;
    Pipeline *pipeline;
    ARM_Mode  mode;
} ARM_State;

////////////////////////// REGISTER DEFINITIONS  //////////////////////////////

typedef enum
{
    R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12,
    SP, LR, /* R13 = Stack Pointer, R14 = Link Register */
    PC, /* R15 = Program Counter */
    CPSR, /* R16 = Current Proccessor Status Processor */

    R8_FIQ, R9_FIQ, R10_FIQ, R11_FIQ, R12_FIQ, R13_FIQ, R14_FIQ, R13_IRQ,
    R14_IRQ, R13_SVC, R14_SVC, R13_ABT, R14_ABT, R13_UNDEF, R14_UNDEF,

    SPSR_FIQ, SPSR_IRQ, SPSR_SVC, SPSR_ABT, SPSR_UNDEF
} Register;


////////////////////////// FLAG DEFINITIONS  //////////////////////////////////

typedef enum
{
    N   = 31, /* negative */
    Z   = 30, /* zero */
    C   = 29, /* executed */
    V   = 28  /* overflowed */
}  CPSR_Flag;

typedef enum
{
   T    = 0x20,      /* thumb */
   F    = 0x40,      /* FIQ disable */
   I    = 0x80,      /* IRQ disable */
   A    = 0x100,     /* Abort disable */
   E    = 0x200,     /* Endianness */
   Q    = 0x8000000, /* Saturation */
}  SPSR_Flag;



#endif
