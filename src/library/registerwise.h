#ifndef _REG_WISE
#define _REG_WISE

#include <stdint.h>
#include <stdlib.h>
#include "bitwise.h"
#include "instruction.h"
#include "binFileLoader.h"
#include "arm11.h"


int32_t as_shifted_reg(int32_t value, int8_t S);
/* AS SHIFT REGISTER FUNCTION */
int32_t as_immediate_reg(int value);
/* AS IMMEDIATE REGISER FUNCTION */


///////////////////////////// MACROS /////////////////////////////////////////


/* REGISTER READ/WRITE */
#define REG_READ(r)     (arm_Ptr->registers[(r)])
#define REG_WRITE(r, v) (arm_Ptr->registers[(r)] = (v))

/* CPSR READ/WRITE */
#define CPSR_CLR(posf)    (BIT_CLR(REG_READ(CPSR), (posf)))
#define CPSR_SET(posf)    (BIT_SET(REG_READ(CPSR), (posf)))
#define CPSR_GET(posf    (BIT_GET(REG_READ(CPSR), (posf)))
#define CPSR_PUT(posf, i) {(IS_SET(i)) ? CPSR_SET(posf): CPSR_CLR(posf); }

/* PC INCREMENT */
#define INC_PC(i) (REG_READ(PC) += (i))


//////////////////////////////CPSR Structure //////////////////////////////////


typedef struct CPSR_STRUCT {

  unsigned int _000 : 28;
  unsigned int bitV : 1;
  unsigned int bitC : 1;
  unsigned int bitZ : 1;
  unsigned int bitN : 1;
}CPSR_STRUCT;



#endif
