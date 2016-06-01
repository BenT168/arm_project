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

/* Memory Read/Write */

//reading one byte (8-bits)
#define MEM_R_8bits(m)     (arm_Ptr->memory[m])
//reading 4 bytes (32-bits) LITTLE ENDIAN
#define MEM_R_32bits(m)   ((MEM_R_8bits(m + 3) & 0xFF) << (SIZE_OF_WORD - 8) |\
                           (MEM_R_8bits(m + 2) & 0xFF) << (SIZE_OF_WORD - 16) |\
                           (MEM_R_8bits(m + 1) & 0xFF) << (SIZE_OF_WORD - 24) |\
                           (MEM_R_8bits(m + 0) & 0xFF) << (SIZE_OF_WORD - 32))
//reading 4 bytes (32-bits) BIG ENDIAN
#define MEM_R_32bits_BE(m)((MEM_R_8bits(m + 0) & 0xFF) << (SIZE_OF_WORD - 8) |\
                           (MEM_R_8bits(m + 1) & 0xFF) << (SIZE_OF_WORD - 16) |\
                           (MEM_R_8bits(m + 2) & 0xFF) << (SIZE_OF_WORD - 24) |\
                           (MEM_R_8bits(m + 3) & 0xFF) << (SIZE_OF_WORD - 32))

//writing one byte(8-bits)
#define MEM_W_8bits(m, b) (arm_Ptr->memory[(m)] = (b))
//writing 4 bytes(32-bits)
#define MEM_W_32bits(m, w)   MEM_W_8bits(m + 0, ((w) >>  0 ) & 0xFF); \
                             MEM_W_8bits(m + 1, ((w) >>  8 ) & 0xFF); \
                             MEM_W_8bits(m + 2, ((w) >> 16 ) & 0xFF); \
                             MEM_W_8bits(m + 3, ((w) >> 24 ) & 0xFF);


///////////////////////////// FUNCTION PROTOTYPE //////////////////////////////


struct ARM_State *arm_Ptr;

void read_ARM(const char *);

void emulator(void);
int  check_cond(int32_t);
void decode_instr(int32_t);
void decode_checker(int32_t);
void print_register_state(void);


int32_t as_shifted_reg(int32_t value, int8_t setCond);
/* AS SHIFT REGISTER FUNCTION */
int32_t as_immediate_reg(int value);
/* AS IMMEDIATE REGISER FUNCTION */

void data_processing(int32_t);
int32_t convert();
int32_t convert2complement();
void multiply(int32_t);
void single_data_transfer(int32_t);
void branch(int32_t);


////////////////////////// BINARY FILE LOADER ////////////////////////////////


void read_ARM(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    //file = fopen(binFile, "rb"); //open the file r = read , b = binary

    int size_instruct = 4; /* 32 bits = 4 bytes */

    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        long size = ftell(file); //Size of file in bytes
        fseek(file, 0, SEEK_SET); //go back to start

        //Allocate memory
        int num_instruct = size / size_instruct;
        //Read instructions into ARM Memory
        // for(int i = 0; i < MEMORY_CAPACITY; i++)
        size_t newLen = fread(arm_Ptr->memory, size_instruct, num_instruct, file);
        if (newLen == 0)
        {
           fputs("Error reading file", stderr);
        } else {
           fclose(file);
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

    //the emulator should terminate when it executes an all-0 instr

    do {
      //If the condition matched, we can execute the instr
      if(cond_check == 1) {
        decode_instr(arm_Ptr->pipeline->decoded);
      }

      arm_Ptr->pipeline->decoded = arm_Ptr->pipeline->fetched;
      arm_Ptr->pipeline->fetched = MEM_R_32bits(REG_READ(PC));
      INC_PC(4);

      fetched_code = arm_Ptr->pipeline->fetched;
      decoded_code = arm_Ptr->pipeline->decoded;

    } while(decoded_code != 0);

     //for a cycle of pipeline, previously fetched instr is decoded and ancestor ints is executed.
     //the emulator should terminate when it executes an all-0 instr
     //Upon termination, output the state of all the registers
    print_register_state();
}


/*decode instruction */
void decode_instr(int32_t word)
{
  int code = BIT_GET(word, 27);

  switch (code)
  {
    case 1:
      branch(word);
      break;
    case 0:
      IS_SET(BIT_GET(word, 26)) ? single_data_transfer(word)
                                         : decode_checker(word);
      break;
    default:
      break;

	}
}

/* helper function for decode_instr */
void decode_checker(int32_t word)
{
  if(IS_SET(BIT_GET(word, 25))) {
    data_processing(word);
  } else {
    if(IS_SET(BIT_GET(word, 6)) || IS_SET(BIT_GET(word, 5))) {
      data_processing(word);
    } else{
      (IS_SET(BIT_GET(word, 4)) && IS_SET(BIT_GET(word, 7)))
                              ? multiply(word) : data_processing(word);
    }
  }
}

/* Check condiitons */
int check_cond(int32_t word)
{
  int cond = get_bits(word, 28, 31);

  switch(cond){
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
    //Register 0 - 12 are the general registers
    for(int i = 0; i < REGISTER_COUNT - 4; i++)
    {
        int32_t reg = REG_READ(i);
        printf("$%-3i: %10d (0x%08x)\n", i, reg, reg);
    }

    printf("PC  : %10d (0x%08x)\n", REG_READ(PC), REG_READ(PC));
    printf("CPSR: %10d (0x%08x)\n", REG_READ(CPSR),  REG_READ(CPSR));

    printf("Non-zero memory:\n");
    for (int i = 0; i < MEMORY_CAPACITY; i += 4)
    {
    	if (MEM_R_32bits(i) == 0) continue;
    	printf("0x%08x: 0x%08x\n", i, MEM_R_32bits_BE(i));
    }

}

///////////////////////////// SHIFTING //////////////////////////////////////

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

    int Rm       = sreg->Rm;
  	int Flag     = sreg->Flag;
  	int Type     = sreg->Type;
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


//////////////////////////EMULATE INSTRUCTION///////////////////////////////////


/* data processing */

void data_processing(int32_t word)
{
	DataProcessingInstruct *DPInst = (DataProcessingInstruct *) &word;

	int ImmOp    = DPInst->ImmOp;        // 25
	int OpCode   = DPInst->Opcode;   // 24-21
	int SetCond  = DPInst->SetCond;
	int Rn       = DPInst->Rn;
	int Rd       = DPInst->Rd;
	int Operand2 = DPInst->Operand2; // 11-0

	int Operand1 = arm_Ptr->registers[Rn];

	Operand2     = IS_CLEAR(ImmOp) ? as_shifted_reg(Operand2, SetCond)
	           		           : as_immediate_reg(Operand2);
	int result   = 0;


	// calculate result by opcode
	switch (OpCode)
	{
		case AND :
		case TST :
                    result = Operand1 & Operand2;
                    break;
		case EOR :
		case TEQ :
                    result = Operand1 ^ Operand2;
                    break;
		case SUB :
		case CMP :
                    result = Operand1 - Operand2;
                    break;
		case RSB :
                    result = Operand2 - Operand1;
                    break;
		case ADD :
                    result = Operand1 + Operand2;
                    break;
		case ORR :
                    result = Operand1 | Operand2;
                    break;
		case MOV :
                    result = Operand2;
                    break;
		default  :
                    result = 0;
	}
  	// save results if necessary
    switch (OpCode)
  	{
  		case TEQ :
  		case TST :
      case CMP :
                  break;
      default :
              REG_WRITE(Rd, result);
              break;
    }


	if (IS_SET(SetCond)) {
	// set flags
  	CPSR_PUT(Z, (result == 0));
    CPSR_PUT(N, BIT_GET(result, 31));

    switch (OpCode)  {
      case AND :
    	case TST :
    	case EOR :
    	case TEQ :
    	case ORR :
    	case MOV :
        CPSR_PUT(C, result);
        break;
  	  case SUB :
  	  case RSB :
  	  case CMP :
        CPSR_PUT(C, (result >= 0));
        break;
    	case ADD  :
          CPSR_PUT(C, CPSR_GET(V));
          break;
      default  :
          break;
    }
	}
}


/* multiply */

//Converts number in register
//if 2's complement then negates and add 1, else return;
int32_t convert(int32_t reg)
{
    int mask = 1 >> 3;
    int32_t result = reg;
    if((mask & (reg >> 3)) == 1) {
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
    //int cpsrReg = arm_Ptr->registers[CPSR];
    MultiplyInstruct *MultiInst = (MultiplyInstruct *) &word;
    //CPSR_STRUCT *cpsrStruct = (CPSR_STRUCT *) &cpsrReg;

    int Acc     = MultiInst->Acc;
    int SetCond = MultiInst->SetCond;
    int Rd      = MultiInst->Rd;
    int Rn      = MultiInst->Rn;
    int Rs      = MultiInst->Rs;
    int Rm      = MultiInst->Rm;


    int32_t dataRm = convert(REG_READ(Rm));
    int32_t dataRs = convert(REG_READ(Rs));

    int32_t mulResult = dataRm * dataRs;

    if(IS_SET(Acc))
    {
        int32_t dataRn = convert(REG_READ(Rn));
        mulResult += dataRn;
    }

    REG_WRITE(Rd, mulResult);

    if(IS_SET(SetCond))
    {

        int bit31 = BIT_GET(mulResult, 31); //N is the 31 bit of result
        CPSR_PUT(N, bit31);
        //arm_Ptr->registers[CPSR] = bit31;
        (mulResult == 0) ? (CPSR_SET(Z)) : !(CPSR_SET(Z));
        //(mulResult == 0) ? (cpsrStruct->bitZ = 1) : (cpsrStruct->bitZ = 0);
    }
}


/*Single data transfer */

void single_data_transfer(int32_t word)
{
    SDTInstruct *SDTInst = (SDTInstruct *) &word;

    int dataRn     = SDTInst->Rn;         // base register
    int dataRd     = SDTInst->Rd;
    int dataOffset = SDTInst->Offset;
    int dataI      = SDTInst->I;
    int dataP      = SDTInst->P;
    int dataU      = SDTInst->U;
    int dataL      = SDTInst->L;

    int _Rn = arm_Ptr->registers[dataRn];
    int _Rd   = arm_Ptr->registers[dataRd];
	//Check if I is setbranchOffset
  if (IS_SET(dataI))
  {
    dataOffset = as_shifted_reg(dataOffset, 0);
  } else {
    dataOffset = as_immediate_reg(dataOffset);
  }

  if (IS_SET(dataP)) _Rn += (IS_SET(dataU) ? dataOffset : -dataOffset);

  if (_Rn < 0 || _Rn >= MEMORY_CAPACITY ) {
    printf("Error: Out of bounds memory access at address 0x%08x\n", _Rn);
    return;
  }
  IS_SET(dataL) ? (REG_WRITE(dataRd, MEM_R_32bits(_Rn))) : MEM_W_32bits(_Rn, _Rd);

  if (IS_CLEAR(dataP)) REG_WRITE(dataRn, _Rn += (IS_SET(dataU) ? dataOffset : -dataOffset));
}
/*  //Pre-indexing
  if (IS_SET(dataP))
  {
    REG_WRITE(dataRn, _Rn += (IS_SET(dataU) ? dataOffset : -dataOffset));
    IS_SET(dataL) ? (REG_WRITE(dataRd, MEM_R_32bits(_Rn))) : MEM_W_32bits(_Rn, _Rd);
  //Post-indexing
  } else {
    IS_SET(dataL) ? (REG_WRITE(dataRd, MEM_R_32bits(_Rn))) : MEM_W_32bits(_Rn, _Rd);
    REG_WRITE(dataRn, _Rn += (IS_SET(dataU) ? dataOffset : -dataOffset));
    }
}
*/
  //int PostIndexing = IS_CLEAR(P);
	//int PreIndexing  = !PostIndexing;

	//int address = ARM->registers[Rn];
	//int value   = ARM->registers[Rd];

//	if (PreIndexing) address += (IS_SET(U) ? Offset : -Offset);


	//if (address < 0 || address >= MEMORY_CAPACITY) goto moob;
	//if (IS_SET(L)) REG_WRITE(Rd, MEM_WORD_READ(address));
	//else           MEM_WORD_WRITE(address, value);


	//if (PostIndexing) REG_WRITE(Rn, address += (IS_SET(U) ? Offset : -Offset));


//moob:
    //printf("Error: Out of bounds memory access at address 0x%08x\n", address);
  //  return;


/*branch */

void branch(int32_t word)
{
     int branchOffset =  23;
     int branchShift  =  2;
     //int branchPC     =  8;

     BranchInstruct *BranchInst = (BranchInstruct *) &word;

     //offset is between in bits 0-23
     //branchStrc -> Offset = get_bits(word, 0, 23);

     int offsetb = BranchInst-> Offset;

     //offset is shifted left 2 bits
     int offsetNewb = offsetb << branchShift;
     int mask = 0x00ffffff;
     int32_t offsetNew32 = mask & offsetNewb;
     //signed bits extended to 32 bits
     int32_t signed_bits = (offsetNew32 >> branchOffset) << (SIZE_OF_WORD - 1);
     //int32_t signed_bits = (offsetNewb << (branchPC - branchShift + 1)) >> (SIZE_OF_WORD - 1);

     //add the signed bits to PC
     INC_PC(signed_bits);

     arm_Ptr->pipeline->fetched = MEM_R_32bits(REG_READ(PC));
     //PC = PC + 4;
     INC_PC(4);
}



/////////////////////////MAIN  FUNCTION//////////////////////////////////////


int main(int argc, char **argv)
{
    if(argc < 2)
    {
      // not in form of .bin '.','b','i','n','/0';
        printf( "No argument in input\n");
        exit(EXIT_FAILURE);
    }


    arm_Ptr = calloc (1, sizeof(ARM_State));
    arm_Ptr->pipeline = calloc(1, sizeof(arm_Ptr->pipeline));

    if(arm_Ptr == NULL)
    {
      printf( "No argument in input\n");
      printf("Please type in a bin file\n");
      exit(EXIT_FAILURE);
    }
    //Read input file and emulate
    read_ARM(argv[1]);
    emulator();

    free(arm_Ptr -> pipeline);
    free(arm_Ptr);

    return EXIT_SUCCESS;

}
