#include <stdint.h>
#include <stdio.h>
#include "registerwise.h"
#include "instruction.h"

////////////////////////// SHIFTING ////////////////////////////////////////////

/* AS IMMEDIATE REGISTER  */

int32_t as_immediate_reg(int value)
{
	int Imm = get_bits(value, 0, 7);
	int Rotate = get_bits(value, 8, 11) * 2;
	return rotate_right(Imm, Rotate);
}

/*  AS SHIFT REGISTER  */

int32_t as_shifted_reg(int32_t value, int8_t S)
{
  	ShiftReg *sreg = (ShiftReg *) &value;

  	int Flag     = sreg->Flag;
  	int Type     = sreg->Type;
  	int Rm       = sreg->Rm;
  	int amount   = sreg->Amount;
  	uint32_t reg = REG_READ(Rm);
  	int carryAmount    = 0;

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
  			if (amount != 0) carryAmount = BIT_GET(reg, 31 - amount + 1);
  			if (IS_SET(S))   CPSR_PUT(C, carryAmount);
  			break;
  		}
  		case LSR :
  		{
  			value = reg >> amount;
  			if (amount != 0) carryAmount = BIT_GET(reg, amount - 1);
  			if (IS_SET(S))   CPSR_PUT(C, carryAmount);
  			break;
  		}
  		case ASR :
  		{
  			value = reg >> amount;
  			if (amount != 0) carryAmount = BIT_GET(reg, amount - 1);
  			if (S == 1)      CPSR_PUT(C, carryAmount);
  			int bit = BIT_GET(reg, 31); // TODO move to bits set
  			for (int j = 0; j < amount; j++) BIT_PUT(value, 31 - j, bit);
  			break;
  		}
  		case ROR :
  		{
  			value = rotate_right(reg, amount);
  			break;
  		}
  		default : exit(EXIT_FAILURE);
  	}

  	return value;
}


  int look_CPSR(int i)
{
    return BIT_GET(REG_READ(CPSR), i);
}
