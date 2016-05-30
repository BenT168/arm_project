#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

//////////////////////////////////STRUCTURE//////////////////////////////////

#include "library/arm11.h"
#include "library/assembler.h"
#include "library/instruction.h"

////////////////////////////////////MACROS/////////////////////////////////////

#include "library/register.h"
#include "library/tokens.h"
#include "library/bitwise.h"



// for numerica constant it's in the form "#x" where x is a natural number
// or in the form "=x" for ldr instr (the expr can be 32 bits after =)

#define max_8bit_represented  256 // 2^8 = 256

int expr_to_num(char *expr)
{
  return (int32_t) strtol(expr, NULL, 0);
}

///////////////////////////// FUNCTION PROTOTYPE //////////////////////////////

ASSEMBLER_STRUCT *ass = NULL;
char *buffer;

TOKEN* read_Source(const char *);
void write_File(const char *);

int as_numeric_constant(char *);
int as_shifted_reg_ass(TOKEN *, int);

void funcArray(void);
int32_t assembler_func(TOKEN *line);

/* Data Processing */
int32_t ass_data_proc_result(TOKEN *);
int32_t ass_data_proc_mov(TOKEN *);
int32_t ass_data_proc_cpsr(TOKEN *);

/* Multiply */
int32_t ass_multiply_mul(TOKEN *);
int32_t ass_multiply_mla(TOKEN *);

/* Single Data Transfer */
int32_t ass_single_data_transfer(TOKEN *, int, char *);
int32_t SDT_num_const(TOKEN *, int, char *);
int32_t ass_SDT(TOKEN *);

/* Branch */
int32_t ass_branch(TOKEN *, int , char *);
int32_t ass_BRCH(TOKEN *);

/* Special */
int32_t andeq_func(TOKEN *);
int32_t lsl_func(TOKEN *);


///////////////////////Binary file reader //////////////////////////////////////

TOKEN* read_Source(const char *sourceFile) {
  FILE *file = fopen(sourceFile, "rs");

  printf("a1\n");
  //if (file != NULL) {
    fseek(file, 0, SEEK_END);
    printf("a2\n");

    long size = ftell(file); //Size of file in bytes
    printf("a3\n");

    fseek(file, 0, SEEK_SET); //go back to start

      printf("a4\n");

    buffer = (char *) malloc(size * sizeof(char));

      printf("a5\n");
    if(buffer == NULL) {
        printf("a6\n");
      perror("Malloc failed.");
    } else {
        printf("a7\n");
    fread(buffer, size, 1, file);
      printf("a8\n");
    if(ferror(file)) {
      perror("Error reading from sourceFile.\n");
    }

      printf("a9\n");


    int sizeBuffer = sizeof(buffer);

      printf("b1\n");

    buffer[sizeBuffer - 1]= '\0';

    printf("b2\n");


   return tokenise(buffer, ",");

   printf("b3\n");

  }

  TOKEN *result = NULL;
  printf("b4\n");
  return result;

}


void write_File(const char *binaryFile) {
  FILE *file = fopen(binaryFile, "wb"); //w = write b = binary

  int32_t *program = (int32_t*) assemble_generate_bin(ass);
  //get binary code from assembler program

  int size = ass->TOTAL_line * sizeof(int32_t);
  //size of each element that will be written

  fwrite(program, size, 1, file);

  fclose(file);

  free(program);
}

//////////////////////////   Core     //////////////////////////////////////////

//TODO: CHECKKKKKK!!!!!!
int as_numeric_constant(char *value){
  //int num_bit = 0;
  int to_num = expr_to_num(value);
  /*while(num_bit < 32){
    rotate_right(value, 2);
    num_bit += 2;
  }
  */
  if(to_num > max_8bit_represented) {
    perror("numerical constant cannot be represented.");
    exit(EXIT_FAILURE);
  }
  return to_num;
}

// can be either <shiftname><register> or <shiftname><#expression>
//<shiftname> can be either asr, lsl, lsr or ror
//If operand2 is a register the 12-bit is shift(11-4)+Rm(3-0)
  //first case integer(11-7)+shift type(6-5)+0(4)
  //second case shiftReg RS(11-8)+0(7)+shift type(6-5)+1(4)
//TOKEN *elem is a pointer to elems in tokenized line
int as_shifted_reg_ass(TOKEN *line, int Rm)
{
  	char *shift_name = line->tokens[Rm + 1];
  	char *Operand2 = line->tokens[Rm + 2];
  	int  result = 0;

	ShiftReg 	 shiftReg;
  	ShiftRegOptional regOp;
  	int shiftType = expr_to_num(shift_name);

	//in the form <shiftname><#expression>
	if(Is_Expression(Operand2))
	{
  	//+1 to git rid of 'r' but just getting the reg number
 		  shiftReg.Rm = PARSE_REG(Rm - 1);
  		shiftReg.Flag = 0;
  		shiftReg.Type = shiftType;
  		shiftReg.Amount = atoi(Operand2);

  		result = *((int *) &shiftReg);

	} else { //in the form <shiftname><register>
	  //CHECK THE STRUC?!??!
  		regOp.Rm = PARSE_REG(Rm + 2);
  		regOp.Flag1 = 0;
  		regOp.Type = shiftType;
	  	regOp.Flag2 = 0;
  		regOp.Rs = PARSE_REG(Rm) | (1 << 4);

  //regOp.Rs = atoi(line->tokens[Rm] + 1) << 3; //getting the last bit of Rs

  		result = *((int *) &regOp);
	}

	return result;
}

//to check if operand2 is an expression or a register
int check_op2(TOKEN *line, int op2){
  char *operand2 = line->tokens[op2 + 2];

  if(Is_Expression(operand2)){
    return as_numeric_constant(operand2);
  }
  return as_shifted_reg_ass(line, op2);

}


function_assPtr function_Array[9];

int32_t assembler_func(TOKEN *line) {
  char *mnemonic = line->tokens[0];
  int i = str_to_mnemonic(mnemonic);
  return function_Array[i](line);
}

void funcArray(void) {
  function_Array[0] = ass_data_proc_result;
  function_Array[1] = ass_data_proc_cpsr;
  function_Array[2] = ass_data_proc_mov;
  function_Array[3] = ass_multiply_mul;
  function_Array[4] = ass_multiply_mla;
  function_Array[5] = ass_BRCH;
  function_Array[6] = ass_SDT;
  function_Array[7] = lsl_func;
  function_Array[8] = andeq_func;

}





/////////////////////// Assemble Instructions //////////////////////////////////

///////* Data Processing *////////

/* Data processing can be broken down into three main types:
*   1. instructions that compute results
*   2. single operand assignment
*   3. instructions that dont compute results but set the CPSR flags
*
* The syntax for the three main 'types' of data processing instructions is:
*   <opcode>   - Is the instruction mnemonic associated with an integer OpCode
*   Rd, Rn, Rm - Represent registers (r0, r1, PC, ...)
*   <Operand2> - Represents an operand. It can be interpreted as a numeric
*                constant <#expression> or a shifted register <shift>
*
* The parameters that the function takes into account:
* line     - Contains the tokens forming this instruction
* SetCond  - Consider the case where the CPRS flags are to be set
* Rn       - Is the position of the Rn token in the line->tokens array
* Rs       - same as Rn
* Operand2 - same as Rn
*
*/


static int32_t ass_data_proc(TOKEN *line, int SetCond, int Rn, int Rd, int Operand_2)
{
	char *Operand2 = line->tokens[Operand_2];
	char *mnemonic = line->tokens[0];

	static DataProcessingInstruct *DPInst;

	DPInst->Cond	   = AL;
	DPInst->_00	     = 0;
	DPInst->ImmOp	   = Is_Expression(Operand2);
	DPInst->Opcode	 = str_to_mnemonic(mnemonic);
	DPInst->SetCond	 = SetCond;
	DPInst->Rn       = PARSE_REG(Rn);
	DPInst->Rd	     = PARSE_REG(Rd);
	DPInst->Operand2 = check_op2(line, Operand_2);

	return *((int32_t *) &DPInst);
}

int32_t ass_data_proc_result(TOKEN *line)
{
  return ass_data_proc(line, 0, -1, 1, 2);
}

int32_t ass_data_proc_mov(TOKEN *line)
{
  return ass_data_proc(line, 0, -1, 1, 2);
}

int32_t ass_data_proc_cpsr(TOKEN *line)
{
  int CPSR_SET   =  1;
  int RD_IGNORED = -1;
  int POS_OF_RN  =  1;
  int POS_OF_OP2 =  2;

  return ass_data_proc(line, CPSR_SET, POS_OF_RN, RD_IGNORED, POS_OF_OP2);
}



////////* Multiply *////////

/* Multiply instructions can be broken down into two types:
*   1. multiply with syntax
*   2. multiply with accummulate, with syntax
*
* The syntax for the multiply instructions is:
* Rd, Rs, Rn, Rm - Represent registers (r0, r1, PC, ...)
*
* The parameters that the function take into account:
* line     - Contains the tokens forming this instruction
* Acc      - Consider the case where the instruction has an accumulate
* Rn       - Is the position of the Rn token in the line->tokens array
* Rs       - same as Rn
* Operand2 - same as Rn
*
*/


static int32_t ass_multiply(TOKEN *line, int Acc, int Rd, int Rm, int Rs, int Rn)
{
	static MultiplyInstruct *MulInst;

	MulInst->Cond	   = AL;
	MulInst->_000000   = 0;
	MulInst->Acc	   = Acc;
	MulInst->SetCond = 0;
	MulInst->Rd	     = PARSE_REG(Rd);
	MulInst->Rn	     = PARSE_REG(Rn);
	MulInst->Rs	     = PARSE_REG(Rs);
	MulInst->_1001	 = 9 % (1 << sizeof(char));
	MulInst->Rm	     = PARSE_REG(Rm);

	return *((int32_t *) &MulInst);
}

int32_t ass_multiply_mul(TOKEN *line)
{
  int N_ACC       =  0;
  int POS_OF_RD   =  1;
  int POS_OF_RM   =  2;
  int POS_OF_RS   =  3;
  int RN_IGNORED  = -1;

  return ass_multiply(line, N_ACC, POS_OF_RD, POS_OF_RM, POS_OF_RS, RN_IGNORED);
}

int32_t ass_multiply_mla(TOKEN *line)
{
  int _ACC        = 1;
  int POS_OF_RD   = 1;
  int POS_OF_RM   = 2;
  int POS_OF_RS   = 3;
  int POS_OF_RN   = 4;

  return ass_multiply(line, _ACC, POS_OF_RD, POS_OF_RM, POS_OF_RS, POS_OF_RN);
}


int32_t SDT_num_const(TOKEN *line, int Rd, char *address) {
  char *Regd = line->tokens[1];
  char *adr = line->tokens[2];
  int newAddress = expr_to_num(adr++);

  if (newAddress <= 0xFF) {                  // Treat as mov Instruction
    adr[0] = '#';
    return ass_data_proc_mov(line);

  } else {
    // use PC to calculate new address
    int offset = newAddress - ass->current_address - 8;  // off-by-8 bytes effect
    //generate ldr r0,[PC, offset]
    char *newline = (char*)malloc(MAX_CHAR_inLINE * sizeof(char));

    sprintf(newline, "ldr %s, [15, #%d]", Regd, offset); // PC =15
    TOKEN *newtoken = malloc(sizeof(TOKEN));
    newtoken = tokenise(newline, ", ");
      return ass_SDT(newtoken);
  }
}

int32_t ass_single_data_transfer(TOKEN *line, int Rd, char *address)
{
//  int Rn     = PARSE_REG(expr_to_num(line->tokens[1] + 1));
  char *adr  = line->tokens[2];
  char *mnem = line->tokens[0];

  //It's Pre-indexed address if the expression ends with ']'
  int Pre_index = (string_endc(adr) == ']') ? 1 : 0;
  //initialise I, U, Offset
  int Imm    = 0;
  int Up     = 1;
  int offset = 0;

  if (Is_Expression(adr)) {                   // In <=expression> form
    return SDT_num_const(line, Rd, address);
  }

  TOKEN *newline = malloc(sizeof(TOKEN));
  newline = tokenise(adr, "[], ");           // get arguements from <address>
  char *expr = newline->tokens[1];

  if (newline->tokenCount == 1) {            // Case [Rn]
    offset = 0;

  } else if (Is_Expression(expr)) {          // Case [Rn, <#expression>]
    offset = expr_to_num(expr++);

  } else {
    if (expr[0] == '+' || expr[0] == '+') {  // Check if there is sign
      Up = (expr[0] == '+') ? 1 : 0;         // If U is set then + else -
      expr++;                                // Remove the sign
    }
    int Rm = expr_to_num(expr);
	  offset = as_shifted_reg_ass(newline, Rm);

    Imm = 1;                                 // As shifted register
}

  static SDTInstruct *SDTinstr;

  SDTinstr->Cond   = AL;
  SDTinstr->_01	   = 1;
  SDTinstr->P	     = Pre_index;
  SDTinstr->I	     = Imm;
  SDTinstr->U	     = Up;
  SDTinstr->_00	   = 0;
  SDTinstr->L	     = (strcmp(mnem, "ldr") == 0) ? 1 : 0;  //ldr --> L is set
  SDTinstr->Rn     = PARSE_REG(2);
  SDTinstr->Rd	   = PARSE_REG(1);
  SDTinstr->Offset = offset;

  return *(int32_t *) &SDTinstr;

}

int32_t ass_SDT(TOKEN *line) {
  return ass_single_data_transfer(line, 1, 0);
}

int32_t ass_branch(TOKEN *line, int Cond, char *expr) {
	char *suffix = (strcmp(line->tokens[0], "b") == 0) ? "al" : (line->tokens[0] + 1);

	int address   = expr_to_num(line->tokens[1]);
 //	char *address = PARSE_REG(expr_to_num(label));

	int offset = (ass->current_address  - address + 8) >> 2;  // compute offet

	BranchInstruct *Branchinstr;
	Branchinstr->Cond   = str_to_cond(suffix);
	Branchinstr->_101   = 5 % (1 << sizeof(char));
	Branchinstr->_0     = 0;
	Branchinstr->Offset = offset;

	return *(int32_t *) &Branchinstr;
}

int32_t ass_BRCH(TOKEN *line) {
  return ass_branch(line, 1, 0);
}

//////////////////Special Instruction //////////////////////////////////////////

/*andeq func */
//for instr that compute results, the syntax is <opcode> Rd, Rn, <Operand 2>
//andeq is similar to and with cond set to 0000 (eq condition)
//andeq r0, r0, r0
//TOKEN *line is to get 'andeq from the line read'
int32_t andeq_func(TOKEN *line){
  return 0x00000000;
}

/*lsl func */
//note: asprintf() cal the length of the string, allocate that amount of mem and
//write the string into it. it is an implicit malloc need to free afterward
//Compile lsl Rn,<#expression> as mov Rn, Rn, lsl <#expression>
int32_t lsl_func(TOKEN *token_line){
 char *new_token_line = (char* )malloc(MAX_CHAR_inLINE * sizeof(char));
 sprintf(new_token_line, "mov %s, %s, lsl %s", token_line->tokens[1],
                                                token_line->tokens[1],
                                                token_line->tokens[2]);
TOKEN *new_token = (TOKEN*) malloc(sizeof(TOKEN));
new_token = tokenise(new_token_line, " ,");
return ass_data_proc_mov(new_token);

//free(new_token_line); TODO: Remeber to free

}

////////////////////A factorial program ////////////////////////////////////////
//ARM stores instructions using Little-endian


///////////////////////// Main /////////////////////////////////////////////////
int main(int argc, char **argv)
{

  if(argc < 3) { // Need two files (+ executer)
    printf("Incomplete number of arguments in input!\n");
    printf("Please type in as first argument : ARM source file\n");
    printf("And as aecond argument : an output ARM binary code file\n");
    exit(EXIT_FAILURE);
  }

  printf("before function_Array\n");
  funcArray();
  printf("after funcArray\n");

  TOKEN *lines = (TOKEN*) malloc(sizeof(TOKEN));
  printf("after token\n");

  lines = read_Source(argv[1]);
  printf("after read_source main\n");
  tokens_print(lines);

  //get lines of assembly codes

  ass = assemble(lines, &assembler_func, ",");
  printf("after ass main\n");

   //assemble lines using assembler and get output to write to file

  write_File(argv[2]);
  printf("after write main\n");


  tokens_free(lines);
  printf("after token_free main\n");

  assemble_free(ass);
  printf("after ass_free main\n");


  return EXIT_SUCCESS;
}
