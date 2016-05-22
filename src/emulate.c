#include <stdlib.h>
<<<<<<< HEAD
#include <stdint.h>
#include <stdio.h>

#include <library/bitwise.h>

//////////////////////////////ARM INSTRUCTION//////////////////////////////////

#include <library/arm11.h>

///////////////////////// Structure of Instruction ////////////////////////////

#include <library/instruction.h>
=======
#include <stdio.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
#include "library/bitwise.h"

//////////////////////////////ARM INSTRUCTION//////////////////////////////////

#include "library/arm11.h"

///////////////////////// Structure of Instruction ////////////////////////////

#include "library/instruction.h"

/////////////////////////////// Bin File Loader ///////////////////////////////

#include "library/binFileLoader.h"

////////////////////////////////Registers read and write////////////////////////

#include "library/registerwise.h"



struct state *arm_ptr;
#define read_memory(m) (arm_ptr->memory[m])

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

//This is for the single data transfer
void SDT(int32_t word){
  struct SDTinstr *sdtInst = (SDTinstr *) &word;
  int offsetSDT = sdtInst->Offset;
  int Rd        = sdtInst->Rd;
  int Rn        = sdtInst->Rn;
  int L         = sdtInst->L;
  int U         = sdtInst->U;
  int P         = sdtInst->P;
  int I         = sdtInst->I;

  int Pre_indexing = IS_SET(P);
  int Post_indexing = !IS_SET(P);

  offsetSDT = IS_SET(I) ? shifted_register(offsetSDT) : immediate_Offset(offsetSDT);

  if(IS_SET(U)){
Rn += offsetSDT;
  }else{
Rn -= offsetSDT;
  }
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

/////////////////////////MAIN  FUNCTION//////////////////////////////////////

=======
#include <stdint.h>
#include <stdio.h>

#include "library/bitwise.h"

//////////////////////////////ARM INSTRUCTION//////////////////////////////////

#include "library/arm11.h"

///////////////////////// Structure of Instruction ////////////////////////////

#include "library/instruction.h"
>>>>>>> edcb83e3a1a13d26ce6174906f44fc80646b215c

/////////////////////////////// Bin File Loader ///////////////////////////////

FILE *file;

void read_ARM(const char *binFile);

struct state *arm_Ptr;

void read_ARM(const char *binFile)
{
    file = fopen(binFile, "rb"); //open the file r = read , b = binary

    if (file != NULL)
    {
      fseek(file, 0, SEEK_END);
      long size = ftell(file); //Size of file in bytes
      fseek(file, 0, SEEK_SET); //go back to start

      //Allocate memory
      arm_Ptr = malloc(size+1);
      if(!arm_Ptr)
      {
        char errorMsg[] = "Memory Error!";
        fprintf(stderr, "%s\n", errorMsg);
        fclose(file);
        return;
      }

     //Read instructions into ARM Memory
      size_t newLen = fread(arm_Ptr->memory, size, 1, file);
      if (newLen == 0)
        fputs("Error reading file", stderr);
      else
       fclose(file);

      free(arm_Ptr->memory);
    } else
    {
      perror("Error opening file");
    }
}

//////////////////////////EMULATE INSTRUCTION//////////////////////////////////////

#define REG_READ(r)     (ARM->registers[(r)])
#define REG_WRITE(r, v) (ARM->registers[(r)] = (v))
#define CPSR_CLR(f)    (BIT_CLR(REG_READ(CPSR), (f)))
#define CPSR_SET(f)    (BIT_SET(REG_READ(CPSR), (f)))
#define CPSR_GET(f)    (BIT_GET(REG_READ(CPSR), (f)))
#define CPSR_PUT(f, b) { if (IS_SET(b)) CPSR_SET(f); else CPSR_CLR(f); }

<<<<<<< HEAD
#define Increase_PC(a) (ARM->registers[PC] += a);

=======
>>>>>>> edcb83e3a1a13d26ce6174906f44fc80646b215c
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
		case TST :
		case TEQ :
		case CMP :
      break;
		default  :
      REG_WRITE(Rd, result); break;
	}

	if (IS_CLEAR(S)) return;

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

int32_t convert();
int32_t addNumbers();

int CRegs[32];
int *CPSR = (int *)&CRegs;

void multiply(int32_t word)
{
    struct multiplyStruct *mStruct = (struct multiplyStruct *)&word;
    mStruct->A  = get_bits(word, 20, 21); // Accumulate is bit-21

    mStruct->Rm = get_bits(word,0,3); // Rm is bit-0 to bit-3
    int32_t newRm = convert(mStruct->Rm);
    mStruct->Rs = get_bits(word,8, 11); //Rs is bit-8 to bit-11
    int32_t newRs = convert(mStruct->Rs);
    mStruct->Rd = get_bits(word, 16, 19); // Rd is bit-16 to bit-19

    mStruct->Rd = newRs & newRm;

    if(IS_SET(mStruct->A)) {
      mStruct->Rn = get_bits(word, 12, 15); //Rn is bit-12 to bit-15
      int32_t newRn = convert(mStruct->Rn);
      int32_t mul = mStruct->Rd;
      mStruct->Rd = addNumbers(mul, newRn);
    }

    if(IS_SET(mStruct->S)) {
    int bit31 = get_bits(mStruct->Rd, 30, 31); //N is the 31 bit of result
    CPSR[N] = bit31;
    if(mStruct->Rd == 0) {
      CPSR[Z] = 1; //Z is set
    } else {
      CPSR[Z] = 0;
    }
  }
}


int32_t convert2complement();
//Converts number in register
//if 2's complement then negates and add 1, else return;
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

int32_t addNumbers(int32_t reg1, int32_t reg2) {
  int32_t carry = reg1 & reg2;
  int32_t result = reg1 ^ reg2;

  while(carry != 0) {
    int32_t shift = carry << 1;
    carry = result & shift;
    result ^= shift;
  }
  return result;
}
<<<<<<< HEAD

void singleDataTransfer(int32_t word) {

	struct SDTinstr *instr = (SDTinstr *) &word;

  int32_t dataRn     = instr->Rn;
	int32_t dataOffset = instr->Offset;
	int32_t dataRd     = instr->Rd;
	//int32_t dataI      = instr->I;
	//int32_t dataP      = instr->P;
	//int32_t dataU      = instr->U;
	//int32_t dataL      = instr->L;


	//Check if I is set
  dataOffset = IS_SET(instr->I)? as_shifted_reg(dataOffset, 1)
                               : as_immediate_reg(dataOffset);

  //Pre-indexing
	if (IS_SET(instr->P)) {
    dataRn += (IS_SET(instr->U)? dataOffset : -dataOffset);

		if (IS_SET(instr->L)) {
			REG_READ(dataRn) = dataRd;
		} else {
			REG_WRITE(dataRn, dataRd);
		}

  //Post-indexing
	} else {
    if (IS_SET(instr->L)) {
    	REG_READ(dataRn) = dataRd;
    } else {
	    REG_WRITE(dataRn, dataRd);
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

void branch(int32_t word)
{
	Braninstr *instr = (Braninstr *) &word;

	//Shift left by 2;
	int32_t dataOffset = (instr->Offset) << 2;

	//Extend to 32 bits;
	int32_t extendOffset = (dataOffset << 6) >> 6;

  Increase_PC(extendOffset);

  //PC = PC +4
  Increase_PC(4);

}

=======
>>>>>>> edcb83e3a1a13d26ce6174906f44fc80646b215c
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

<<<<<<< HEAD
  int look_CPSR(int i)
{
    return get_bits(getRegister(16), i-1, i);
}

int checkCond(int32_t word)
{
    int cond = get_bits(word, 28, 31);

    switch(cond){
      case(EQ):
        return look_CPSR(Z);
        break;
      case(NE):
        return !look_CPSR(Z);
        break;
      case(GE):
        return look_CPSR(N) == look_CPSR(V);
        break;
      case(LT):
        return look_CPSR(N) != look_CPSR(V);
        break;
      case(GT):
        return look_CPSR(Z) & (look_CPSR(N) == look_CPSR(V));
        break;
      case(LE):
        return look_CPSR(Z) | (look_CPSR(N) != look_CPSR(V));
        break;
      case(AL):
        return 1;
        break;
      default;

    }
    return 0;
}



struct state *arm_ptr;

#define getRegister(r) (arm_ptr->registers[r])

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
=======
////////////////////////////////// Main ////////////////////////////////////////


int main(int argc, char **argv) {
  //int argc mumber of chars

 char errorMsg[] = "No arguments in input!\n";

  if(argc < 6) {
    // not in form of .bin '.','b','i','n','/0';
    printf( "%s", errorMsg);
    printf("Please type in a bin file\n");
    return -1;

  readARM(argv[0]);
  emulator();

  return EXIT_SUCCESS;
  }
>>>>>>> edcb83e3a1a13d26ce6174906f44fc80646b215c
}
