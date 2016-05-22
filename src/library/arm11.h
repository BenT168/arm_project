<<<<<<< HEAD
#ifndef _ARM_TYPE
#define _ARM_TYPE

#define MEMORY_CAPACITY 65536
#define REGISTER_COUNT  17

////////////////////////////   PIPELINE  //////////////////////////////////////

typedef struct Pipeline
{
    int32_t fetched;
    int32_t decoded;
} Pipeline;

////////////////////////////  STATE OF ARM  ///////////////////////////////////

typedef struct ARM_State
{
    int8_t   memory[MEMORY_CAPACITY];
    int32_t  registers[REGISTER_COUNT];
    Pipeline *pipeline;
} ARM_State;

////////////////////////// REGISTER DEFINITIONS  //////////////////////////////

typedef enum Register
{
    R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12,
    SP, LR, /* R13, R14 */
    PC,
    CPSR
} Register;

////////////////////////// FLAG DEFINITIONS  //////////////////////////////////

typedef enum CPSR_Flag
{
    N   = 31, /* negative */
    Z   = 30, /* zero */
    C   = 29, /* executed */
    V   = 28  /* overflowed */
} CPSR_Flag;


#endif
=======
#ifndef _ARM_TYPE
#define _ARM_TYPE

#define MEMORY_CAPACITY 65536 //2^16
#define REGISTER_COUNT  17
#define SIZE_OF_WORD 32


////////////////////////////   PIPELINE  //////////////////////////////////////

typedef struct Pipeline
{
    int32_t fetched;
    int32_t decoded;
} Pipeline;

////////////////////////////  STATE OF ARM  ///////////////////////////////////

typedef struct ARM_State
{
    int8_t   memory[MEMORY_CAPACITY];
    int32_t  registers[REGISTER_COUNT];
    Pipeline *pipeline;
} ARM_State;

////////////////////////// REGISTER DEFINITIONS  //////////////////////////////

typedef enum Register
{
    R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12,
    SP, LR, /* R13, R14 */
    PC,
    CPSR
} Register;

////////////////////////// FLAG DEFINITIONS  //////////////////////////////////

typedef enum CPSR_Flag
{
    N   = 31, /* negative */
    Z   = 30, /* zero */
    C   = 29, /* executed */
    V   = 28  /* overflowed */
} CPSR_Flag;


#endif
