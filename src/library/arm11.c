#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "assembler.h"
#include "tokens.h"
#include "bitwise.h"
#include "symbolTableList.h"



int32_t get_value_SPSR(ARM_Mode Mode)
{
  switch (Mode)
  {
      case FIQ       : return SPSR_FIQ;
      case IRQ       : return SPSR_IRQ;
      case Supervisor: return SPSR_SVC;
      case Abort     : return SPSR_ABT;
      case Undefined : return SPSR_UNDEF;
  }

  return 0;
}

int32_t set_value_SPSR(ARM_Mode Mode, int value)
{
  switch (Mode)
  {
      case FIQ       : SPSR_FIQ = value; break;
      case IRQ       : SPSR_IRQ = value; break;
      case Supervisor: SPSR_SVC = value; break;
      case Abort     : SPSR_ABT = value; break;
      case Undefined : SPSR_UNDEF = value; break;
  }

  return 0;
}


ARM_Mode get_CPU_mode(ARM_Mode Mode)
{
  return (ARM_Mode) (CPSR & 0x1f);
}

void set_CPU_mode(ARM_Mode Mode, int value){
  CPSR = (uint)(CPSR & ~0x1f) | (uint) value;
}



uint access_reg(ARM_Mode Mode, int RegisterIndex){
  switch (Mode) {
    case User   :
    case System : return REG_READ[RegisterIndex]; break;
    case FIO    :
      if(RegisterIndex < 8 || RegisterIndex == 15){
        return REG_READ[RegisterIndex];
      } else {
        switch (RegisterIndex){
          case 8 : return R8_FIQ;
          case 9 : return R9_FIQ;
          case 10: return R10_FIQ;
          case 11: return R11_FIQ;
          case 12: return R12_FIQ;
          case 13: return R13_FIQ;
          case 14: return R14_FIQ;
        }
      }
    break;
    case IRQ       :
      if(RegisterIndex < 13 || RegisterIndex == 15){
        return REG_READ[RegisterIndex];
      } else {
        switch (RegisterIndex){
        case 13; return R13_IRQ;
        case 14: return R14_IRQ;
        }
      }
    break;
    case Supervisor:
      if(RegisterIndex < 13 || RegisterIndex == 15){
        return REG_READ[RegisterIndex];
      } else {
        switch (RegisterIndex){
        case 13; return R13_SVC;
        case 14: return R14_SVC;
        }
      }
    break;
    case Abort     :
      if(RegisterIndex < 13 || RegisterIndex == 15){
        return REG_READ[RegisterIndex];
      } else {
        switch (RegisterIndex){
        case 13; return R13_ABT;
        case 14: return R14_ABT;
      }
    }
    break;
    case Undefined :
    if(RegisterIndex < 13 || RegisterIndex == 15){
      return REG_READ[RegisterIndex];
    } else {
      switch (RegisterIndex){
      case 13; return R13_UNDEF;
      case 14: return R14_UNDEF;
    }
  }
  break;

}
