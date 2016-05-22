#include <stdlib.h>
#include <stdio.h>
//#include <string.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////

#define BIT_SET(x, n)       ((x) |=   1 << (n))
#define BIT_CLR(x, n)       ((x) &= ~(1 << (n)))
#define BIT_GET(x, n)    (!!((x) &   (1 << (n))))
#define BIT_PUT(x, n, b) { if (IS_SET(b)) BIT_SET(x, n); else BIT_CLR(x, n); }

#define IS_SET(x)   (x == 1)
#define IS_CLEAR(x) (x == 0)

#define SIZE_OF_WORD 32
#define memoryCapacity 65536
#define numRegisters 17

////for arm instruction////////


struct state *arm_ptr;
#define read_memory(m) (arm_ptr->memory[m])

#define getRegister(r) (arm_ptr->registers[r])

//!!! TODO: Check!!! 15 or PC???////if change change the print reg state bit as well//
#define increment_PC(i) (getRegister(PC) += (i))


//////For Branch /////////
#define branchOffset 24
#define branchShift 2

////////////////////////////////////////////////////////////////////////////////
typedef enum Registers { //Register defs
  R0, R1, R2, R3, R4,
  R5, R6, R7, R8, R9,
  R10, R11, R12, R13,
  R14, PC, CPSR
} registers;

typedef enum {
  eq = 0,
  ne = 1,
  ge = 10,
  lt = 11,
  gt = 12,
  le = 13,
  al = 14,
} suffix;

typedef struct Pipeline {
  int32_t fetched;
  int32_t decoded;
} Pipeline;

typedef struct state {
  int8_t memory[memoryCapacity];
  int32_t registers[numRegisters];
  struct Pipeline *pipeline;
} state;

typedef enum CPSR { // CPSR register defs
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

typedef struct SDTinstr
{
    unsigned int Offset : 12;
    unsigned int Rd     : 4;
    unsigned int Rn     : 4;
    unsigned int L      : 1;
    unsigned int _00    : 2;
    unsigned int U      : 1;
    unsigned int P      : 1;
    unsigned int I      : 1;
    unsigned int _01    : 2;
    unsigned int Cond   : 4;
} SDTinstr;

typedef struct Branchinstr
{
    unsigned int Offset : 24;
    unsigned int _0     : 1;
    unsigned int _101   : 3;
    unsigned int Cond   : 4;
} Branchinstr;

//////////////////////////////functions/////////////////////////////////////////
int32_t get_bits(int32_t word, int start, int end){
if(end - start == (SIZE_OF_WORD - 1)){return word;}

//create a mask that matches the bits between start and end
int32_t mask = (((1 << (end - start + 1)) - 1)  >> ((SIZE_OF_WORD) - end)) << start;
//use the & operation to extract the required bits
int32_t result = mask & word;
return result >> start;

}

void printBits(uint32_t x) {
  int i;
  uint32_t mask = 1 << 31;

  for(i = 0; i < 32; ++i) {
    printf("%i", (x & mask) != 0);
    x <<= 1;
    }

  printf("\n");
}

int32_t rotateRight(int amount, int32_t value){
  int rotated = value << (32 - amount);
  int remaining = value >> amount;
  return rotated + remaining;

}

int immediate_Offset(int bitsValue){
  uint32_t Imm = get_bits(bitsValue, 0, 7);
  int rotate_amount = get_bits(bitsValue, 8, 11) * 2;

  return rotateRight(rotate_amount, Imm);
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


int look_CPSR(int i){
  return get_bits(getRegister(16), i-1, i);
}

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

///////////////////emulator//////////////////////////////////////////////////////
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


//////////////////////////////Main//////////////////////////////////////////////

int main(int argc, char **argv) {

  printf("Hello\n");
  /*uint32_t i = 33;
  printBits(i);
  get_bits(i, 4, 7);
*/
  return EXIT_SUCCESS;
}
