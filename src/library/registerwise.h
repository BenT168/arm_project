#ifndef _REG_WISE
#define _REG_WISE

#include <stdint.h>
#include <stdlib.h>
#include "bitwise.h"
#include "instruction.h"
#include "binFileLoader.h"
#include "arm11.h"


int look_CPSR(int i);
int32_t as_shifted_reg(int32_t value, int8_t S);
int32_t as_immediate_reg(int value);


#define REG_READ(r)     (arm_Ptr->registers[(r)])
#define REG_WRITE(r, v) (arm_Ptr->registers[(r)] = (v))

#define CPSR_CLR(f)    (BIT_CLR(REG_READ(CPSR), (f)))
#define CPSR_SET(f)    (BIT_SET(REG_READ(CPSR), (f)))
#define CPSR_GET(f)    (BIT_GET(REG_READ(CPSR), (f)))
#define CPSR_PUT(f, b) { if (IS_SET(b)) CPSR_SET(f); else CPSR_CLR(f); }

#define increment_PC(i) (REG_READ(PC) += (i))


//////////////////////////////CPSR Structure //////////////////////////////////
typedef struct CPSR_STRUCT {

  unsigned int _000 : 28;
  unsigned int bitV : 1;
  unsigned int bitC : 1;
  unsigned int bitZ : 1;
  unsigned int bitN : 1;
}CPSR_STRUCT;

#endif
