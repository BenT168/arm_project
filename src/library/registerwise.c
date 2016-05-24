#include <stdint.h>
#include <stdio.h>

#include "registerwise.h"
#include "instruction.h"

<<<<<<< HEAD
extern ARM_State arm_Ptr;


///////////////////////////// MACROS /////////////////////////////////////////


/* REGISTER READ/WRITE */
#define REG_READ(r)     (arm_Ptr.registers[(r)])
#define REG_WRITE(r, v) (arm_Ptr.registers[(r)] = (v))

/* CPSR READ/WRITE */
#define CPSR_CLR(posf)    (BIT_CLR(REG_READ(CPSR), (posf)))
#define CPSR_SET(posf)    (BIT_SET(REG_READ(CPSR), (posf)))
#define CPSR_GET(posf)    (BIT_GET(REG_READ(CPSR), (posf)))
#define CPSR_PUT(posf, i) {(IS_SET(i)) ? CPSR_SET(posf): CPSR_CLR(posf);}
/* PC INCREMENT */
#define INC_PC(i) (REG_READ(PC) += (i))




=======
>>>>>>> 25e3d1847907448d33aabb2d502767e1ddfa6362
/* AS IMMEDIATE REGISTER  */

int32_t as_immediate_reg(int value)
{
	int Imm = get_bits(value, 0, 7);
	int Rotate = get_bits(value, 8, 11) * 2;
	return rotate_right(Imm, Rotate);
}

/*  AS SHIFT REGISTER  */

int32_t as_shifted_reg(int32_t value, int8_t setCond)
{
  	ShiftReg *sreg = (ShiftReg *) &value;

  	int Flag     = sreg->Flag;
  	int Type     = sreg->Type;
  	int Rm       = sreg->Rm;
  	int amount   = sreg->Amount;
  	uint32_t reg = REG_READ(Rm);
  	int carryAmt = 0;

  	if (IS_SET(Flag))
  	{
  		int Rs = REG_READ(get_bits(amount, 1, 4));
  		amount = get_bits(Rs, 0, 8);
  	}

  	switch (Type)
  	{
  		case LSL : // Arithmetic and logical shift left are equivalent
  		{
  			value = reg << amount;
  			if (amount != 0) {
				  carryAmt = BIT_GET(reg, 31 - amount + 1);
				}
  			if (IS_SET(setCond)) {
				  CPSR_PUT(C, carryAmt);
				}
  		}
			break;

  		case LSR :
  		{
  			value = reg >> amount;
  			if (amount != 0) {
				  carryAmt = BIT_GET(reg, amount - 1);
				}
  			if (IS_SET(setCond)){
					CPSR_PUT(C, carryAmt);
				}
  		}
			break;

  		case ASR :
  		{
  			value = reg >> amount;
  			if (amount != 0){
   				carryAmt = BIT_GET(reg, amount - 1);
				}
  			if (setCond == 1){
				  CPSR_PUT(C, carryAmt);
          int bit = BIT_GET(reg, 31); // TODO move to bits set
            for (int j = 0; j < amount; j++){
							BIT_PUT(value, 31 - j, bit);
						}
				}
  		}
			break;

  		case ROR :
  		{
  			value = rotate_right(reg, amount);
  		}
			break;

  	}

		return value;
}
