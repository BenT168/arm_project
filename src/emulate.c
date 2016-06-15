

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

////////////////////////////////ARM STRUCTURE//////////////////////////////////

#include "library/arm11.h"

///////////////////////// STRUCTURE OF INSTRUCTION ////////////////////////////

#include "library/instruction.h"

////////////////////////////////////MACROS/////////////////////////////////////

#include "library/bitwise.h"
#include "library/register.h"
#include "library/gpio.h"

/////////////////////////// MEMORY READ & WRITE ///////////////////////////////

/*Reading one byte (8-bits)*/
#define MEM_R_8bits(m)     (arm_Ptr->memory[m])

/*Reading 4 bytes (32-bits) LITTLE ENDIAN*/
#define MEM_R_32bits(m)   ((MEM_R_8bits(m + 3) & 0xFF) << (SIZE_OF_WORD - 8) | \
                           (MEM_R_8bits(m + 2) & 0xFF) << (SIZE_OF_WORD - 16) |\
                           (MEM_R_8bits(m + 1) & 0xFF) << (SIZE_OF_WORD - 24) |\
                           (MEM_R_8bits(m + 0) & 0xFF) << (SIZE_OF_WORD - 32))
/*Reading 4 bytes (32-bits) BIG ENDIAN*/
#define MEM_R_32bits_BE(m)((MEM_R_8bits(m + 0) & 0xFF) << (SIZE_OF_WORD - 8) | \
                           (MEM_R_8bits(m + 1) & 0xFF) << (SIZE_OF_WORD - 16) |\
                           (MEM_R_8bits(m + 2) & 0xFF) << (SIZE_OF_WORD - 24) |\
                           (MEM_R_8bits(m + 3) & 0xFF) << (SIZE_OF_WORD - 32))

/*Writing one byte(8-bits)*/
#define MEM_W_8bits(m, b) (arm_Ptr->memory[m] = b)

/*Writing 4 bytes(32-bits)*/
#define MEM_W_32bits(m, w) MEM_W_8bits(m + 0, (w >>  0) & 0xFF); \
                           MEM_W_8bits(m + 1, (w >>  8) & 0xFF); \
                           MEM_W_8bits(m + 2, (w >> 16) & 0xFF); \
                           MEM_W_8bits(m + 3, (w >> 24) & 0xFF);




///////////////////////////// FUNCTION PROTOTYPE //////////////////////////////


struct ARM_State *arm_Ptr;

void read_ARM(const char *);

void emulator(void);
int  check_cond(int32_t);
void decode_instr(int32_t);
static void decode_checker_1(int32_t);
static void decode_checker_2(int32_t);
void print_register_state(void);


int32_t as_shifted_reg(int32_t, int8_t);
/* AS SHIFT REGISTER FUNCTION */
int32_t as_immediate_reg(int);
/* AS IMMEDIATE REGISER FUNCTION */

void data_processing(int32_t);
int32_t convert();
int32_t convert2complement();
void multiply(int32_t);
void single_data_transfer(int32_t);
void branch(int32_t);

void block_data_transfer(int32_t); //extension
void software_interrupt(int32_t);  //extension


////////////////////////// BINARY FILE LOADER ////////////////////////////////


void read_ARM(const char *filename)
{
  /* open the file r = read , b = binary  */
  FILE *file = fopen(filename, "rb");

  int size_instruct = 4; // 32 bits = 4 bytes

  if (file != NULL)
  {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);  //size of file in bytes
    fseek(file, 0, SEEK_SET); //go back to start

    /* allocate memory */
    int num_instruct = size / size_instruct;

    /* Read instructions into ARM Memory */
    size_t newLen = fread(arm_Ptr->memory, size_instruct, num_instruct, file);

    /* Check if the file is read properly */
    if (newLen == 0)
    {
      fputs("Error reading file", stderr);
    } else {
       fclose(file);   // Close the file if no error occures
    }

  } else {
    perror("Error opening file");
  }
}


//////////////////////////// EMULATE CORE /////////////////////////////////////

/* emulator */
void emulator()
{
  REG_WRITE(PC, 0);

  arm_Ptr->pipeline->fetched = MEM_R_32bits(REG_READ(PC));

  INC_PC(4);
  int32_t decoded_code;
  int32_t fetched_code = arm_Ptr->pipeline->fetched;
  int cond_check = check_cond(fetched_code);

  do {
    /* If the condition matched, we can execute the instr */
    if(cond_check == 1)
    {
      decode_instr(arm_Ptr->pipeline->decoded);
    }

  arm_Ptr->pipeline->decoded = arm_Ptr->pipeline->fetched;
  arm_Ptr->pipeline->fetched = MEM_R_32bits(REG_READ(PC));
  INC_PC(4);

  fetched_code = arm_Ptr->pipeline->fetched;
  decoded_code = arm_Ptr->pipeline->decoded;

  } while (decoded_code != 0);
  /* The emulator should terminate when it executes an all-0 instr */

  /* for a cycle of pipeline, previously fetched instr is decoded
     and ancestor ints is executed.
     Upon termination, output the state of all the registers */
  print_register_state();
}


/*decode instruction */

void decode_instr(int32_t word)
{
  switch (BIT_GET(word, 27))
  {
    case 1:
      IS_SET(BIT_GET(word,26)) ? software_interrupt(word)
                               : decode_checker_1(word);
      break;
    case 0:
      IS_SET(BIT_GET(word, 26)) ? single_data_transfer(word)
                                : decode_checker_2(word);
      break;
    default:
      break;
	}
}

/* first helper function for decode_instr */
static void decode_checker_1(int32_t word)
{
  IS_SET(BIT_GET(word, 25)) ? branch(word) : block_data_transfer(word);
}

/* second helper function for decode_instr */
static void decode_checker_2(int32_t word)
{
  if(IS_SET(BIT_GET(word, 25)))
  {
    data_processing(word);
  } else {
    if(IS_CLEAR(BIT_GET(word, 4)))
    {
      data_processing(word);
    } else{
      (IS_SET(BIT_GET(word, 7))) ? multiply(word) : data_processing(word);
    }
  }
}

/* Check condiitons */
int check_cond(int32_t word)
{
  int cond = get_bits(word, 28, 31);

  switch(cond) {
    case(EQ):
      return CPSR_GET(Z);
      break;
    case(NE):
      return !CPSR_GET(Z);
      break;
    case(GE):
      return CPSR_GET(N) == CPSR_GET(V);
      break;
    case(LT):
      return CPSR_GET(N) != CPSR_GET(V);
      break;
    case(GT):
      return !CPSR_GET(Z) & (CPSR_GET(N) == CPSR_GET(V));
      break;
    case(LE):
      return CPSR_GET(Z) | (CPSR_GET(N) != CPSR_GET(V));
      break;
    case(AL):
      return 1;
      break;
    default :
      return 0;
  }
}


/* Print Register State (upon termination) */
void print_register_state()
{
  printf("Registers:\n");

  /* Register 0 - 12 are the general registers */
  for(int i = 0; i < REGISTER_COUNT - 4; i++)
  {
    int32_t reg = REG_READ(i);
    printf("$%-3i: %10d (0x%08x)\n", i, reg, reg);
  }

  /* print out the result */
  printf("PC  : %10d (0x%08x)\n", REG_READ(PC), REG_READ(PC));
  printf("CPSR: %10d (0x%08x)\n", REG_READ(CPSR), REG_READ(CPSR));

  printf("Non-zero memory:\n");
  for (int i = 0; i < MEMORY_CAPACITY; i += 4)
  {
  	if (MEM_R_32bits(i) == 0) continue;
  	printf("0x%08x: 0x%08x\n", i, MEM_R_32bits_BE(i));
  }
}

///////////////////////////// SHIFTING //////////////////////////////////////

/* as immediate register */
int32_t as_immediate_reg(int value)
{
  /* Get the immediate and rotate values */
	int Imm = get_bits(value, 0, 7);
	int Rotate = get_bits(value, 8, 11) * 2;

	return rotate_right(Imm, Rotate);
}

/* as shift register */
int32_t as_shifted_reg(int32_t value, int8_t setCond)
{
  ShiftReg *sreg = (ShiftReg *) &value;

  int Rm       = sreg->Rm;
  int Flag     = sreg->Flag;
  int Type     = sreg->Type;
  int Amt      = sreg->Amount;
  uint32_t reg = REG_READ(Rm);
  int carryAmt = 0;

  /* If the first bit of shift is set, Imm is shifted by another register Rs */
  if (IS_SET(Flag))
  {
  	int Rs = REG_READ(get_bits(Amt, 1, 4));
  	Amt = get_bits(Rs, 0, 7); //getting the last byte of Rs register
  }

/* Check for different shift types */
  switch (Type)
  {
      /* Arithmetic and logical shift left are equivalent */
  	case LSL :
  	{
  		value = reg << Amt;
  		if (Amt != 0)
      {
			  carryAmt = BIT_GET(reg, SIZE_OF_WORD - Amt );
			}
      /* If CPSR flags need be set, carry output should be lauched into C bit*/
			if (IS_SET(setCond))
      {
			  CPSR_PUT(C, carryAmt);
			}
  	}
    break;


    /* LSR is basically the same as LSL */
  	case LSR :
  	{
  		value = reg >> Amt;
  		if (Amt != 0)
      {
			  carryAmt = BIT_GET(reg, Amt);
			}
  		if (IS_SET(setCond))
      {
				CPSR_PUT(C, carryAmt);
			}
  	}
    break;

    /* ASR is basically the same as LSR */
  	case ASR :
  	{
  		value = reg >> Amt;
  		if (Amt != 0)
      {
   			carryAmt = BIT_GET(reg, Amt);
			}
  		if (setCond == 1)
      {
			  CPSR_PUT(C, carryAmt);
        /* The high bits are filled with bit 31 */
        int bit = BIT_GET(reg, 31);
        for (int j = 0; j < Amt; j++){
					BIT_PUT(value, 31 - j, bit);
    		}
			}
  	}
    break;

    /* In ROR the bits are shifted in cycle */
  	case ROR :
  	{
  		value = rotate_right(reg, Amt);
  	}
		break;
  }
	return value;
}


//////////////////////////EMULATE INSTRUCTION///////////////////////////////////

/* A global variable for checking branch condition */
int resultforBranch = 0;


////////// Data Processing ///////////

void data_processing(int32_t word)
{
	DataProcessingInstruct *DPInst = (DataProcessingInstruct *) &word;

	int ImmOp    = DPInst->ImmOp;    // 25
	int OpCode   = DPInst->Opcode;   // 24-21
	int SetCond  = DPInst->SetCond;  // 20
	int Rn       = DPInst->Rn;       // 19-16
	int Rd       = DPInst->Rd;       // 15-12
	int Operand2 = DPInst->Operand2; // 11-0

  /* The first operand is always the content of register Rn */
	int Operand1 = arm_Ptr->registers[Rn];

  /* The second operand depends on I flag being set or not */
	Operand2     = IS_CLEAR(ImmOp) ? as_shifted_reg(Operand2, SetCond)
	           		                 : as_immediate_reg(Operand2);

  /* initialise the result */
	int result   = 0;

	/* calculate result by opcode */
	switch (OpCode)
	{
		case AND :
		case TST :
      result = Operand1 & Operand2; break;
		case EOR :
		case TEQ :
      result = Operand1 ^ Operand2;
      break;
		case SUB :
		case CMP :
      result = Operand1 - Operand2; break;
		case RSB :
      result = Operand2 - Operand1; break;
		case ADD :
      result = Operand1 + Operand2; break;
		case ORR :
      result = Operand1 | Operand2; break;
		case MOV :
      result = Operand2; break;
		default  :
      result = 0;
	}

  /* save results if necessary */
  switch (OpCode)
  {
  	case TEQ :
  	case TST :
    case CMP :
      resultforBranch = result;  // save result to check condition in branch
      break;
    default :
      REG_WRITE(Rd, result);
      break;
  }

  /* Update CPSR flags if set condition is set */
	if (IS_SET(SetCond))
  {
  	CPSR_PUT(Z, (result == 0));
    CPSR_PUT(N, BIT_GET(result, 31));

    /* Check result for setting flags */
    switch (OpCode)
    {
  	  case SUB :
        if(result <= 0)
        {
          CPSR_PUT(C, 0);
        } else {
          CPSR_PUT(C, 1);
        }
        break;
  	  case RSB :
  	  case CMP :
        CPSR_PUT(C, (result >= 0)); break;
    	case ADD :
        CPSR_PUT(C, CPSR_GET(V)); break;
      default  : break;
    }
	}
}


////////// Multiply ///////////

/*Converts number in register
  if 2's complement then negates and add 1, else return. */
int32_t convert(int32_t reg)
{
  int mask = 1 >> 3;
  int32_t result = reg;
  if((mask & (reg >> 3)) == 1)
  {
    result = convert2complement(reg);
  }
  return result;
}


int32_t convert2complement(int32_t reg)
{
  int32_t negatedReg = ~reg;
  return negatedReg + 1;
}


void multiply(int32_t word)
{
  MultiplyInstruct *MultiInst = (MultiplyInstruct *) &word;

  int Acc     = MultiInst->Acc;      // 21
  int SetCond = MultiInst->SetCond;  // 20
  int Rd      = MultiInst->Rd;       // 19-16
  int Rn      = MultiInst->Rn;       // 15-12
  int Rs      = MultiInst->Rs;       // 11-8
  int Rm      = MultiInst->Rm;       // 3-0

  int32_t dataRm = convert(REG_READ(Rm));
  int32_t dataRs = convert(REG_READ(Rs));

  /* multiplication */
  int32_t mulResult = dataRm * dataRs;

  /* If accumulate bit is set, an accumulate should also be done */
  if(IS_SET(Acc))
  {
    int32_t dataRn = convert(REG_READ(Rn));
    mulResult += dataRn;
  }

  /* Update CPSR flags if set condition is set */
  REG_WRITE(Rd, mulResult);

  if(IS_SET(SetCond))
  {
    int bit31 = BIT_GET(mulResult, 31); //N is the 31 bit of result
    CPSR_PUT(N, bit31);
    (mulResult == 0) ? (CPSR_SET(Z)) : !(CPSR_SET(Z));
  }
}

////////// Single Data Transfer //////////

void single_data_transfer(int32_t word)
{
  SDTInstruct *SDTInst = (SDTInstruct *) &word;

  int dataI      = SDTInst->ImmOff;  // 25
  int dataP      = SDTInst->P;       // 24
  int dataU      = SDTInst->Up;      // 23
  int dataL      = SDTInst->L;       // 20
  int dataRn     = SDTInst->Rn;      // 19-16
  int dataRd     = SDTInst->Rd;      // 15-12
  int dataOffset = SDTInst->Offset;  // 11-0

  int RegRn = arm_Ptr->registers[dataRn];
  int RegRd = arm_Ptr->registers[dataRd];

  /* Check I flag in order to compute offset */
 if (IS_SET(dataI))
 {
   dataOffset = as_shifted_reg(dataOffset, 0);  // as a shifted register
 } else {
   dataOffset = as_immediate_reg(dataOffset);   // as immediate offset
 }

  /* Pre-Indexing */
  if (IS_SET(dataP))
  {
    RegRn += (IS_SET(dataU) ? dataOffset : -dataOffset);
  }

  /* in the case gpio */
  if (is_GPIO_addr(RegRn))
  {
    print_GPIO_addr(RegRn);
    if(IS_SET(dataL))
    {
      REG_WRITE(dataRd, RegRn);
    }

  /* NOT in the case gpio */
  } else {
    /* Check if the address is out of boundry */
    if (RegRn < 0 || RegRn >= MEMORY_CAPACITY) {
      printf("Error: Out of bounds memory access at address 0x%08x\n", RegRn);
      return;
    }

    /* If L bit is set we have ldr, else we have str */
    if(IS_SET(dataL))
    {
      REG_WRITE(dataRd, MEM_R_32bits(RegRn));
    } else {
      MEM_W_32bits(RegRn, RegRd);
    }
  }

  /* Post-indexing, contents of base register need to be changed */
  if (IS_CLEAR(dataP)) {
    REG_WRITE(dataRn, RegRn += (IS_SET(dataU) ? dataOffset : -dataOffset));
  }
}

////////// Branch ///////////

void branch(int32_t word)
{
  int branchShift  =  2;  // offset needs to be shifted by 2 bits
  int branchOffset =  6;  // 32-24-2=6 bits left that is needed to get 32 bits

  BranchInstruct *BranchInst = (BranchInstruct *) &word;
  int Cond = BranchInst->Cond;   // 31-28

  /* Check comditions for bne and bqe */
  if(resultforBranch == 0 && Cond == 0)  //beq, equal
  {
    goto branchCode;
  } else if(resultforBranch != 0 && Cond == 1) {  //bne, not equal
    goto branchCode;
  } else if(Cond == 14) {  // al or b, nothing should be done
    goto branchCode;
  }
  /* stop for other mnemonics or everything is done */
  goto end;

  /* do shifting to jump in branch */
  branchCode: ;
    int32_t signed_bits = (((BranchInst->Offset) << \
    branchShift) << branchOffset) >> branchOffset;

    /* Add the signed bits to PC */
    INC_PC(signed_bits);
    arm_Ptr->pipeline->fetched = MEM_R_32bits(REG_READ(PC));

    INC_PC(4); //PC = PC + 4;
  end: ;
}


// extension //

/* Block Data Transfer */

uint get_address_for_BDT(int32_t word){

  BDTInstruct *BDTInst = (BDTInstruct *) &word;

  int dataRn      = BDTInst->Rn;         // base register
  int dataU       = BDTInst->Up;
  int dataP       = BDTInst->P;

  int RegRn = arm_Ptr->registers[dataRn];

//If P is set(pre-indexing), offset is +- to Rn BEFORE transferring data.
//If P is not set(post-indextin), offset is +- to rn AFTER transfering data.
// So if P is set, we increment addr by 4 so we can deal with the offset first
//(as illustrated in the diagram p.41, 42 http://bear.ces.cwru.edu/eecs_382/ARM7-TDMI-manual-pt2.pdf)
uint Address;
if(IS_SET(dataU)){
  Address = RegRn + (IS_SET(dataP) ? 4 : 0);
} else {
  Address = RegRn - (IS_SET(dataP) ? 0 : 4);
}

  return  Address;

}


void LDM(int32_t word){
  BDTInstruct *BDTInst = (BDTInstruct *) &word;

  int dataRegList = BDTInst->RegList;  //each bit corresponding to a register
  int dataRn      = BDTInst->Rn;         // base register
  int dataS       = BDTInst->S;

  int RegRn = arm_Ptr->registers[dataRn];

  int PC_bit = BIT_GET(dataRegList, 15);

  uint Address = get_address_for_BDT(word);
  //The registers are transferred in the order lowest to highest
    for(int reg = 0; reg < 15; reg++){
      REG_WRITE(RegRn, Address);
      Address += 4;
    }

  if(IS_SET(PC_bit) && IS_SET(dataS)){
    REG_WRITE(arm_Ptr->registers[PC], MEM_R_32bits(Address)); //R15 is loaded
    arm_Ptr->registers[CPSR] = arm_Ptr->SPSR;
  }
}


void block_data_transfer(int32_t word){
  BDTInstruct *BDTInst = (BDTInstruct *) &word;
  int dataL       = BDTInst->L;
  int dataS       = BDTInst->S;

  if(IS_SET(dataL)){ //When L is set, do LDM else do STM
    LDM(word);
  } else {
    if(IS_SET(dataS)){
      arm_Ptr->mode = User;
    }
  }

}

/* software interrupt */
void software_interrupt(int32_t word)
{
  int32_t SPSR = arm_Ptr->registers[CPSR];
  arm_Ptr->mode = Supervisor;
  arm_Ptr->registers[14]  = arm_Ptr->registers[PC] - 2;
  arm_Ptr->SPSR = SPSR;

  arm_Ptr->registers[PC] = 0xffff0008;
}
/*
int32_t *OnSoftwareInterrupt;

  FLAG_PUT(T, 0);
  FLAG_PUT(I, 1);
  FLAG_PUT(E, 0);

  arm_Ptr->registers[15] = 0xffff0008; // Registers[15] = PC

  if(OnSoftwareInterrupt != NULL)
  {
    int Code = Opcode & 0xff;
    REG_WRITE(arm_Ptr, code);
  }
*/

/////////////////////////MAIN  FUNCTION//////////////////////////////////////

int main(int argc, char **argv)
{
  /* Check input */
  if(argc < 2)
  {
    /* not in form of .bin '.','b','i','n','/0'; */
    printf( "No argument in input\n");
    exit(EXIT_FAILURE);
  }
  arm_Ptr = calloc (1, sizeof(ARM_State));
  arm_Ptr->pipeline = calloc(1, sizeof(Pipeline));
  /* Check input */
  if(arm_Ptr == NULL)
  {
    printf("No argument in input\n");
    printf("Please type in a bin file\n");
    exit(EXIT_FAILURE);
  }
  /* Read input file and emulate */
  read_ARM(argv[1]);
  emulator();

  free(arm_Ptr -> pipeline);
  free(arm_Ptr);

  return EXIT_SUCCESS;
}
