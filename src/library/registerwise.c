#include <stdint.h>
#include <stdio.h>
#include "registerwise.h"

////////////////////////// SHIFTING ////////////////////////////////////////////

/* AS IMMEDIATE REGISTER  */

int32_t as_immediate_reg(int value)
{
	int Imm = bits_get(value, 0, 7);
	int Rotate = bits_get(value, 8, 11) * 2;
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
  	int carry    = 0;

  	if (IS_SET(Flag))
  	{
  		int Rs = REG_READ(bits_get(amount, 1, 4));
  		amount = bits_get(Rs, 0, 8);
  	}

  	switch (Type)
  	{
  		case LSL : // Arithmetic and logical shift left are equivalent
  		{
  			value = reg << amount;
  			if (amount != 0) carry = BIT_GET(reg, 31 - amount + 1);
  			if (IS_SET(S))   CPSR_PUT(CARRY, carry);
  			break;
  		}
  		case LSR :
  		{
  			value = reg >> amount;
  			if (amount != 0) carry = BIT_GET(reg, amount - 1);
  			if (IS_SET(S))   CPSR_PUT(CARRY, carry);
  			break;
  		}
  		case ASR :
  		{
  			value = reg >> amount;
  			if (amount != 0) carry = BIT_GET(reg, amount - 1);
  			if (S == 1)      CPSR_PUT(CARRY, carry);
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
    return get_bits(getRegister(16), i-1, i);
}
