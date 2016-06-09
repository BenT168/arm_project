#ifndef _REG_WISE
#define _REG_WISE

#include <stdint.h>
#include <stdlib.h>

#include "bitwise.h"
#include "instruction.h"
#include "arm11.h"

///////////////////////////// MACROS /////////////////////////////////////////

/* REGISTER READ/WRITE */
#define REG_READ(r)     (arm_Ptr->registers[(r)])
#define REG_WRITE(r, v) (arm_Ptr->registers[(r)] = (v))

/* FLAG SET/CLEAR */
#define FLAG_SET(Flag)    ((REG_READ(CPSR) & (int32_t)Flag) != 0)
#define FLAG_CLR(Flag)    ((REG_READ(CPSR) & (int32_t)Flag) == 0)
#define FLAG_PUT(Flag, V) {(IS_SET(V)) ? FLAG_SET(Flag) : FLAG_CLR(Flag);}

/* CPSR READ/WRITE */
#define CPSR_CLR(posf)    (BIT_CLR(REG_READ(CPSR), (posf)))
#define CPSR_SET(posf)    (BIT_SET(REG_READ(CPSR), (posf)))
#define CPSR_GET(posf)    (BIT_GET(REG_READ(CPSR), (posf)))
#define CPSR_PUT(posf, i) {(IS_SET(i)) ? CPSR_SET(posf): CPSR_CLR(posf);}
/* PC INCREMENT */
#define INC_PC(i) (REG_READ(PC) += (i))


#endif
