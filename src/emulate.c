#include <stdlib.h>
<<<<<<< HEAD
#include <stdio.h>
#include <stdint.h>

#define memoryCapacity 65536
#define numRegisters 17

#define IS_SET(x)   (x == 1)
#define IS_CLEAR(x) (x == 0)

typedef struct Pipeline {
  int32_t fetched;
  int32_t decoded;
} Pipeline;


typedef struct state {
  int8_t memory[memoryCapacity];
  int32_t registers[numRegisters];
  struct Pipeline *pipeline;
} state;

typedef enum Registers { //Register defs
  R0, R1, R2, R3, R4,
  R5, R6, R7, R8, R9,
  R10, R11, R12, R14,
  R15, R16, PC
} registers;

typedef enum { // CPSR register defs
   N = 31, /*Negative*/
   Z = 30, /*zero*/
   C = 29, /*carried-out bit*/
   V = 28  /*overflow*/
} CPSRRegister;

typedef struct multiplyStruct {
  unsigned int Cond : 4; /*Condition field 4-bits*/
  unsigned int A : 1; /*Accumulate 1-bit*/
  unsigned int S : 1; /*Set Condition codes  1-bit*/
  unsigned int Rd : 4; /* Destination registers 4-bits*/

  //Operand registers 4-bits each
  unsigned int Rn : 4;
  unsigned int Rs : 4;
  unsigned int Rm : 4;
} multyplyStructure;

FILE *file;
void readARM();

int main(int argc, char **argv) {

 char errorMsg[] = "No arguments in input!\n";

  if(argc == 0) {
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
#include <stdint.h>
#include <stdio.h>

#include "library/bitwise.h"

//////////////////////////////ARM INSTRUCTION//////////////////////////////////

#include "library/arm11.h"

///////////////////////// Structure of Instruction ////////////////////////////

#include "library/instruction.h"

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
>>>>>>> 70937aca35cabdfe421ae4c46a9cc1fb0adaac25
}

struct state *arm_Ptr;

void readARM(const char *binFile) {

  file = fopen(binFile, "rb"); //open the file r = read , b = binary

  if (file != NULL) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file); //Size of file in bytes
    fseek(file, 0, SEEK_SET); //go back to start

    //Allocate memory
    arm_Ptr = malloc(size+1);
    if(!arm_Ptr) {
      char errorMsg[] = "Memory Error!";
      fprintf(stderr, "%s\n", errorMsg);
      fclose(file);
      return;
    }

    //Read instructions into ARM Memory
    size_t newLen = fread(arm_Ptr->memory, size, 1, file);
    if (newLen == 0) {
     fputs("Error reading file", stderr);
    }
    else {
      fclose(file);
   }
   free(arm_Ptr->memory);
 }
 else {
  perror("Error opening file");
 }
}

#define SIZE_OF_WORD 32;

int32_t get_bits(int32_t word, int start, int end){
  if(end - start == 31){
    return word;
  }
  //create a mask that matches the bits between start and end
  int32_t mask = (((1 << (end - start + 1)) - 1)  >> (31 - end)) << start;

  //use the & operation to extract the required bits
  int32_t result = mask & word;

  return result >> start;
}

int32_t convert();
int32_t addNumbers();

int CRegs[32];
int *CPSR = (int *)&CRegs;

void multiply(int32_t word) {
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
