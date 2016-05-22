#include <stdlib.h>
<<<<<<< HEAD
#include <stdint.h>
#include <stdio.h>

//////////////////////////////ARM INSTRUCTION//////////////////////////////////

#include <library/arm11.h>

///////////////////////// Structure of Instruction ////////////////////////////

#include <library/instruction.h>

////////////////////////////////////////////////////////////////////////////////

#include "library/bitwise.h"

//////////////////////////////ARM INSTRUCTION//////////////////////////////////

#include "library/arm11.h"

/////////////////////////////// Bin File Loader ///////////////////////////////

#include "library/binFileLoader.h"

////////////////////////////////Registers read and write////////////////////////

#include "library/registerwise.h"



struct state *arm_ptr;
#define read_memory(m) (arm_ptr->memory[m])
#define write_memory(m, w) (arm_ptr->memory[m] = w)

//////For Branch /////////
#define branchOffset 24
#define branchShift 2

//////////////////////////////functions/////////////////////////////////////////
int32_t get_bits(int32_t word, int start, int end) {
if(end - start == (SIZE_OF_WORD - 1)){return word;}

//create a mask that matches the bits between start and end
int32_t mask = (((1 << (end - start + 1)) - 1)  >> ((SIZE_OF_WORD) - end)) << start;
//use the & operation to extract the required bits
int32_t result = mask & word;
return result >> start;

}

///////Check condiitons ////////////////////////////////////////////////////////
int checkCond(int32_t word){
  int cond = get_bits(word, 28, 31);

  switch(cond){
  case(eq): return look_CPSR(Z); break;
  case(ne): return !look_CPSR(Z); break;
  case(ge): return look_CPSR(N) == look_CPSR(V); break;
  case(lt): return look_CPSR(N) != look_CPSR(V); break;
  case(gt): return look_CPSR(Z) & (look_CPSR(N) == look_CPSR(V)); break;
  case(le): return look_CPSR(Z) | (look_CPSR(N) != look_CPSR(V)); break;
  case(al): return 1; break;

  }
  return 0;
}

/////////////upon termination print the state of the registers//////////////////
void print_register_state(){
  printf("Registers state: \n");
  printf("General registers: \n");
  //Register 0 - 12 are the general registers
  for(int i = 0; i <= numRegisters - 4; i++){
printf("%08x \n", getRegister(i));
  }

  printf("PC: %08x \n", getRegister(15));
  printf("CPSR: %08x \n", getRegister(16));

}

/*emulator */

void emulator(){

  arm_ptr->pipeline->fetched = read_memory(getRegister(15)); //USE PC OR 15?????
  increment_PC(4);

  int32_t fetched_code = arm_ptr->pipeline->fetched;

  while(fetched_code != 0){
  //TODO : NEED A LOOP HERE WHAT I THE CONDITION????
  //for a cycle of pipeline, previously fetched instr is decoded and ancestor ints is executed.
  arm_ptr->pipeline->decoded = arm_ptr->pipeline->fetched;
  arm_ptr->pipeline->fetched = read_memory(getRegister(15));
  increment_PC(4);


  int checked_cond = checkCond(fetched_code);

  //If the condition matched, we can execute the instr
    if(checked_cond == 1){
    //decode instru // Has Ben implemented?
    }

  }

  //the emulator should terminate when it executes an all-0 instr
  //Upon termination, output the state of all the registers
print_register_state();
}


//////////////////////////EMULATE INSTRUCTION//////////////////////////////////////


/*data processing */

void data_processing(int32_t word)
{
	DataProcessingInstr *inst = (DataProcessingInstr *) &word;

	int ImmOp    = (*inst).immOperad;        // 25
	int OpCode   = (*inst).OpCode;   // 24-21
	int SetCond  = (*inst).setcond;
	int Rn       = (*inst).Rn;
	int Rd       = (*inst).Rd;
	int Operand2 = (*inst).Operand2; // 11-0

	int Operand1 = (*arm11).registers[Rn];

	Operand2     = IS_CLEAR(I) ? as_shifted_reg(Operand2, S)
	           		           : as_immediate_reg(Operand2);
	int result   = 0;

	// calculate result by opcode
	switch (OpCode)
	{
		case AND :
		case TST :
      result = Operand1 & Operand2; break;
		case EOR :
		case TEQ :
      result = Operand1 ^ Operand2; break;
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
  	// save results if necessary
  if(Opcode != TST || Opcode != TEQ || Opcode != CMP) {
      REG_WRITE(Rd, result);
  }


	if (IS_SET(S)) {
	// set flags
  	CPSR_PUT(ZERO, (result == 0));
  	CPSR_PUT(NEGATIVE, BIT_GET(result, 31));
  	switch (OpCode)
  	{
  		case 2  :
  		case 3  :
  		case 10 :
        CPSR_PUT(CARRY, (result >= 0));
        break;
  		case 4  :
        CPSR_PUT(CARRY, CPSR_GET(OVERFLOW));
        break;
      }
	}
}


/* multiply*/

int32_t convert();
int32_t addNumbers();
//Converts number in register
//if 2's complement then negates and add 1, else return;
int32_t convert2complement();


void multiply(int32_t word) {
  int CPSR = arm_ptr->registers[CPSR];
  struct multiplyStruct *mStruct = (multiplyStruct *)&word;
  struct CPSR_STRUCT *cpsrStruct;

  int A  = mStruct->A;
  int Rm = mStruct->Rm;
  int Rs = mStruct->Rs;
  int Rd = mStruct->Rd;
  int S = mStruct->S;

  int32_t dataRm = convert(REG_READ(Rm));
  int32_t dataRs = convert(REG_READ(Rs));
  int32_t mulResult = dataRm * dataRs;

  if(IS_SET(A)) {
    int Rn = mStruct->Rn;
    int32_t dataRn = convert(REG_READ(Rn));
    mulResult += dataRn;
  }

  REG_WRITE(Rd, mulResult);

  if(IS_SET(S)) {
    int bit31 = get_bits(mulResult, 30, 31); //N is the 31 bit of result
    cpsrStruct->bitN = bit31;
    if(mulResult == 0) {
    cpsrStruct->bitZ = 1; //Z is set
    } else {
      cpsrStruct->bitZ = 0;
    }
  }
}


int32_t convert(int32_t reg) {
  int mask = 1 >> 3;
  int32_t result = reg;
  if((mask & (reg >> 3)) == 1) {
    result = convert2complement(reg);
  }
  return result;
}

int32_t convert2complement(int32_t reg) {
  int32_t negatedReg = ~reg;
  return negatedReg + 1;
}


/*Single data transfer */

void singleDataTransfer(int32_t word) {

	struct SDTinstr *instr = (SDTinstr *) &word;

  int32_t dataRn     = instr->Rn;
	int32_t dataOffset = instr->Offset;
	int32_t dataRd     = instr->Rd;

	//Check if I is set
  dataOffset = IS_SET(instr->I)? as_shifted_reg(dataOffset, 1)
                               : as_immediate_reg(dataOffset);

  //Pre-indexing
	if (IS_SET(instr->P)) {
    dataRn += (IS_SET(instr->U)? dataOffset : -dataOffset);

		if (IS_SET(instr->L)) {
      word = read_memory(dataRn);
		} else {
      write_memory(dataRn, word);
		}

  //Post-indexing
	} else {
    if (IS_SET(instr->L)) {
      word = read_memory(dataRn);
		} else {
      write_memory(dataRn, word);
		}

		dataRn += (IS_SET(instr->U)? dataOffset : -dataOffset);

		instr->Rn = dataRn;
	}

  /*
  		if (dataRn < 0 || dataRn >= MEMORY_CAPACITY) {
  			printf("The address is out of boundary");
  			return;
  		}
  */
}

/*branch */

void branch(int32_t word){
  struct Branchinstr *branchStrc = (Branchinstr *) &word;

  //offset is between in bits 0-23
  //branchStrc -> Offset = get_bits(word, 0, 23);

  int32_t offsetb = branchStrc-> Offset;

  //offset is shifted left 2 bits
  int32_t offsetNewb = offsetb << branchShift;

  //signed bits extended to 32 bits
  int signed_bits = (offsetNewb >> (branchOffset - 1)) << (branchOffset - 1);

  //add the signed bits to PC
  increment_PC(signed_bits);

  //CHECK ABOUT THE PC AND PIPELINE!!

  //PC = PC + 4;
  increment_PC(4);
}


/////////////////////////MAIN  FUNCTION//////////////////////////////////////

int main(int argc, char **argv)
{
    char errorMsg[] = "No arguments in input!\n";

    if(argc == 0)
    {
      printf( "%s", errorMsg);
      printf("Please type in a bin file\n");
      return -1;
    } else if(argc > 1) {
       char errorMsg2[] = "Too many arguments in input!\n";
       printf("%s", errorMsg2);
       printf("Please type in a bin file\n");
       return -1;
    }
    readARM(argv[0]);

    return EXIT_SUCCESS;
