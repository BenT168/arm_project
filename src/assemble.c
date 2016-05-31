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
#include "library/symbolTableList.h"



// for numerica constant it's in the form "#x" where x is a natural number
// or in the form "=x" for ldr instr (the expr can be 32 bits after =)

#define max_8bit_represented  256 // 2^8 = 256
#define range_bit 32

#define expr_to_num(expr) ((int) strtol((expr) + 1, NULL, 0))

#define endian 0xFF


///////////////////////////// FUNCTION PROTOTYPE //////////////////////////////

ASSEMBLER_STRUCT *ass = NULL;

TOKEN *read_Source(const char *);
void write_File(ASSEMBLER_STRUCT *, const char *);

int as_numeric_constant(int);
int as_shifted_reg_ass(TOKEN *, int);

void funcArray(void);
int32_t assembler_func(TOKEN *, ASSEMBLER_STRUCT *);

/* Data Processing */
int32_t ass_data_proc_result(TOKEN *, ASSEMBLER_STRUCT *);
int32_t ass_data_proc_mov(TOKEN *, ASSEMBLER_STRUCT *);
int32_t ass_data_proc_cpsr(TOKEN *, ASSEMBLER_STRUCT *);

/* Multiply */
int32_t ass_multiply_mul(TOKEN *, ASSEMBLER_STRUCT *);
int32_t ass_multiply_mla(TOKEN *, ASSEMBLER_STRUCT *);

/* Single Data Transfer */
int32_t SDT_num_const(TOKEN *, ASSEMBLER_STRUCT *);
int32_t ass_single_data_transfer(TOKEN *, ASSEMBLER_STRUCT *);

/* Branch */
int32_t ass_branch(TOKEN *, ASSEMBLER_STRUCT *);

/* Special */
int32_t andeq_func(TOKEN *, ASSEMBLER_STRUCT *);
int32_t lsl_func(TOKEN *, ASSEMBLER_STRUCT *);


///////////////////////Binary file reader //////////////////////////////////////

//Add comma to buffer
char* writeBuffer(char* buffer) {
  for(int i = 0; i < strlen(buffer); i++) {
    if(buffer[i] == ' ') {
      buffer[i] = ',';
    }
  }
  return buffer;
}


TOKEN *read_Source(const char *sourceFile)
{
  FILE *file = fopen(sourceFile, "rt");

    fseek(file, 0, SEEK_END);
    long size = ftell(file); //Size of file in bytes
    rewind(file); //go back to start
    char buffer[size];

    fread(buffer, 1, size, file);
      if(ferror(file)) {
        perror("Error reading from sourceFile.\n");
      }
      buffer[size - 1]= '\0';

      printf("%s\n", buffer);


   return tokenise(buffer, "\n");
}



void write_File(ASSEMBLER_STRUCT *ass, const char *binaryFile)
{
  printf("writefile\n");
  FILE *file = fopen(binaryFile, "wb"); //w = write b = binary

  printf("after opening file (in write_file)\n");
  int32_t *program = assemble_generate_bin(ass);
  //get binary code from assembler program
  printf("program (in write_file)\n");
  size_t size = ass->TOTAL_line * sizeof(int32_t);
  //size of each element that will be written
  printf("size: %zu\n",size );
  assert(fwrite(program, 1, size, file) == size);

  printf("after assertion (in write_file)\n");
  fclose(file);

  free(program);
}

//////////////////////////   Core     //////////////////////////////////////////

//TODO: CHECKKKKKK!!!!!!
int as_numeric_constant(int value){
  //int num_bit = 0;
  int to_num = 0;
  /*while(num_bit < 32){
    rotate_right(value, 2);
    num_bit += 2;
  }
  */
  while (get_bits(value, 8 , range_bit - 1 ) != 0 && to_num < range_bit )
  {
     for (int i = 0; i < 2; i++)
      {
        int last = BIT_GET(value, 31);
        value <<= 1;
        BIT_PUT(value, 0, last);
      }
    	to_num += 2;
  }

  if(to_num == range_bit) {
    perror("numerical constant cannot be represented.");
    exit(EXIT_FAILURE);
  }
  return ((to_num / 2) << 8) | value;
}

// can be either <shiftname><register> or <shiftname><#expression>
//<shiftname> can be either asr, lsl, lsr or ror
//If operand2 is a register the 12-bit is shift(11-4)+Rm(3-0)
  //first case integer(11-7)+shift type(6-5)+0(4)
  //second case shiftReg RS(11-8)+0(7)+shift type(6-5)+1(4)
//TOKEN *elem is a pointer to elems in tokenized line
int as_shifted_reg_ass(TOKEN *line, int Rm)
{
        if(line->tokenCount == Rm + 1)
        {
              return PARSE_REG(Rm);
        }

  	char *shift_name = line->tokens[Rm + 1];
  	char *Operand2 = line->tokens[Rm + 2];
  	int  result = 0;

	ShiftReg 	 shiftReg;
  	ShiftRegOptional regOp;
  	int shiftType = str_to_shift(shift_name);

	//in the form <shiftname><#expression>
	if(Is_Expression(Operand2))
	{
  	//+1 to git rid of 'r' but just getting the reg number
 		shiftReg.Rm = PARSE_REG(Rm - 1);
  		shiftReg.Flag = 0;
  		shiftReg.Type = shiftType;
  		shiftReg.Amount = expr_to_num(Operand2);

  		result = *((int *) &shiftReg);

	} else { //in the form <shiftname><register>
	  //CHECK THE STRUC?!??!
  		regOp.Rm = PARSE_REG(Rm + 2);
  		regOp.Flag = 0;
  		regOp.Type = shiftType;
  		regOp.Amount = PARSE_REG(Rm) | (1 << 4);

  //regOp.Rs = atoi(line->tokens[Rm] + 1) << 3; //getting the last bit of Rs

  		result = *((int *) &regOp);
	}

	return result;
}

//to check if operand2 is an expression or a register
int check_op2(TOKEN *line, int op2){
  char *operand2 = line->tokens[op2];

  if(Is_Expression(operand2)){
    return as_numeric_constant(expr_to_num(operand2));
  }
  return as_shifted_reg_ass(line, op2);

}


function_assPtr function_Array[9];

int32_t assembler_func(TOKEN *line, ASSEMBLER_STRUCT *ass) {
  char *mnemonic = line->tokens[0];
  int i = str_to_mnemonic(mnemonic);
  printf("mnemonic: %i then going to funcArray\n", i);
  return function_Array[i](line, ass);
}

void funcArray(void) {
  function_Array[0] = ass_data_proc_result;
  function_Array[1] = ass_data_proc_mov;
  function_Array[2] = ass_data_proc_cpsr;

  function_Array[3] = ass_multiply_mul;
  function_Array[4] = ass_multiply_mla;

  function_Array[5] = ass_single_data_transfer;

  function_Array[6] = ass_branch;

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
  printf("operand_2: %i\n", Operand_2);
  printf("Operand2 is....%s\n",Operand2);
	char *mnemonic = line->tokens[0];
  printf("%s\n", mnemonic);
  printf("%s\n", line->tokens[1]);

 DataProcessingInstruct DPInst;

	DPInst.Cond	   = AL;
	DPInst._00	     = 0;
	DPInst.ImmOp	   = Is_Expression(Operand2);
	DPInst.Opcode	 = str_to_mnemonic(mnemonic);
	DPInst.SetCond	 = SetCond;
	DPInst.Rn       = PARSE_REG(Rn);
  printf("parse reg rn: %i\n", PARSE_REG(Rn));
	DPInst.Rd	     = PARSE_REG(Rd);
    printf("parse reg rd: %i\n", PARSE_REG(Rn));
	DPInst.Operand2 = check_op2(line, Operand_2);
  printf("data processing op2: %c\n", DPInst.Operand2);

	return *((int32_t *) &DPInst);
}
/* 1. instructions that compute results: and, eor, sub, rsb, add,  orr
*     Syntax : <opcode> Rd Rn <Operand2>
*
*     CPRS flags are not set, hence SetCond is clear or SetCond = 0.
*     Positions of tokens Rd, Rn and Operand2 are 1, 2, and 3 respectively.
*/

int32_t ass_data_proc_result(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  int CPSR_CLEAR  = 0;
  int POS_OF_RD   = 1;
  int POS_OF_RN   = 2;
  int POS_OF_OP2  = 3;

  return ass_data_proc(line, CPSR_CLEAR, POS_OF_RN, POS_OF_RD, POS_OF_OP2);
}

/* 2. single operand assignment: mov
      Syntax : mov Rd, <Operand2>
*
*     CPRS flags are not set, hence SetCond is clear or SetCond = 0.
*     Positions of tokens Rd and Operand2 are 1 and 2 respectively.
*     Rn is ignored therefore -1 is passed as the 'position' of token Rn.
*/

int32_t ass_data_proc_mov(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  int CPSR_CLEAR =  0;
  int POS_OF_RD  =  1;
  int RN_IGNORED = -1;
  int POS_OF_OP2 =  2;

  return ass_data_proc(line, CPSR_CLEAR, RN_IGNORED, POS_OF_RD, POS_OF_OP2);
}

/* 3. instructions that dont compute result but set CPSR flags: tst, teq, cmp
*     Syntax : <opcode> Rn, <Operand_2>
*
*     CPRS flags ARE set, hence a SetCond is set or SetCond = 1.
*     Positions of tokens Rn and Operand2 are 1 and 2 respectively.
*     Rd is ignored therefore -1 is passed as the 'position' of token Rd.
*/
int32_t ass_data_proc_cpsr(TOKEN *line, ASSEMBLER_STRUCT *ass)
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

  printf("%s\n", line->tokens[0] );
  printf("%s\n", line->tokens[1] );
	MultiplyInstruct MulInst;
  printf("ass_multiply\n");
	MulInst.Cond	   = AL;
	MulInst._000000  = 0;
	MulInst.Acc	     = Acc;
	MulInst.SetCond  = 0;
	MulInst.Rd	     = PARSE_REG(Rd);
  printf("rd: %i\n", PARSE_REG(Rd));
  printf("after parse reg rd\n");
	MulInst.Rn	     = PARSE_REG(Rn);
  printf("rn: %i\n", PARSE_REG(Rn));
  printf("in mul reg rn\n");
  printf("Rs: %i\n",Rs );
	MulInst.Rs	     = PARSE_REG(Rs);
  printf("in mul reg rs\n");
	MulInst._1001	 = 9 % (1 << sizeof(char));
  printf("in mul 1001\n");
	MulInst.Rm	     = PARSE_REG(Rm);
  printf("before return in ass_multiply\n");
	return *((int32_t *) &MulInst);
}

int32_t ass_multiply_mul(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  int N_ACC       =  0;
  int POS_OF_RD   =  1;
  int POS_OF_RM   =  2;
  int POS_OF_RS   =  3;
  int RN_IGNORED  = -1;

  return ass_multiply(line, N_ACC, POS_OF_RD, POS_OF_RM, POS_OF_RS, RN_IGNORED);
}

int32_t ass_multiply_mla(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  printf("inside multiply\n");
  int _ACC        = 1;
  int POS_OF_RD   = 1;
  int POS_OF_RM   = 2;
  int POS_OF_RS   = 3;
  int POS_OF_RN   = 4;
  printf("before return multiply\n");
  return ass_multiply(line, _ACC, POS_OF_RD, POS_OF_RM, POS_OF_RS, POS_OF_RN);
}


int32_t SDT_num_const(TOKEN *line, ASSEMBLER_STRUCT *ass) {
  char *Regd = line->tokens[1];
  char *adr = line->tokens[2];
  int newAddress = expr_to_num(adr);

  if (newAddress <= endian) {                  // Treat as mov Instruction
    adr[0] = '#';
    line->tokens[0] = strdup("mov");
    return ass_data_proc_mov(line, ass);

  } else {
    // use PC to calculate new address
    uint16_t last_address = assemble_append(ass, newAddress);
    int offset = last_address - ass->current_address - 8;  // off-by-8 bytes effect
    //generate ldr r0,[PC, offset]
    char *newline = NULL;

    asprintf(&newline, "ldr %s, [PC, #%d]", Regd, offset); // PC =15
    TOKEN *newtoken = tokenise(newline, " ,");
      return ass_single_data_transfer(newtoken, ass);
  }
}

int32_t ass_single_data_transfer(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
//  int Rn     = PARSE_REG(expr_to_num(line->tokens[1] + 1));
  char *adr  = line->tokens[2];
  char *mnem = line->tokens[0];

  //It's Pre-indexed address if the expression ends with ']'
  int Pre_index = tokens_endc(line) == ']';
  //initialise I, U, Offset
  int Imm    = 0;
  int Up     = 1;
  int offset = 0;

  if (Is_Expression(adr)) {                   // In <=expression> form
    return SDT_num_const(line, ass);
  }

  TOKEN *newline = tokenise(strdup(line->line), " ,[]");  // get arguements from <address>
  char *expr = newline->tokens[3];

  if (newline->tokenCount == 3) {            // Case [Rn]
    offset = 0;

  } else if (Is_Expression(expr)) {          // Case [Rn, <#expression>]
    offset = expr_to_num(expr);
    Up = offset >= 0;

  } else {
    Imm = 1;

    if (expr[0] == '+' || expr[0] == '-') {  // Check if there is sign
      Up = (expr[0] == '+') ;         // If U is set then + else -
      expr++;                                // Remove the sign
    }
    offset = as_shifted_reg_ass(newline, 3);

    Up = (expr[0] == '+' || expr[0] == '-' ) ? Up : ( offset >= 0 ); // As shifted register
}

  SDTInstruct SDTinstr;

  SDTinstr.Cond   = AL;
  SDTinstr._01	   = 1;
  SDTinstr.I	  = Imm;
  SDTinstr.P	     = Pre_index;
  SDTinstr.U	     = Up;
  SDTinstr._00	   = 0;
  SDTinstr.L	     = (strcmp(mnem, "ldr") == 0);  //ldr --> L is set
  SDTinstr.Rn     = PARSE_REG(2);
  SDTinstr.Rd	   = PARSE_REG(1);
  SDTinstr.Offset = offset;

  //tokens_free(line);

  return *((int32_t *) &SDTinstr);

}



int32_t ass_branch(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  printf("line->tokens[0]...%s\n", line->tokens[0]);
  printf("line->tokens[0]...%s\n", line->tokens[0] + 1);

char first_letter_token = line->tokens[0][0];
printf("%c\n", first_letter_token );

char *suffix = (first_letter_token != 'b') ? "AL" : (line->tokens[0] +  1);
	//char *suffix = (strcmp(line->tokens[0], "b") == 0) ? "al" : (line->tokens[0] + 1);

	char *lbl   = line->tokens[1];
 //	char *address = PARSE_REG(expr_to_num(label));
        uint16_t lbl_address = *(uint16_t *) map_get(ass->symbolTable,lbl);

        int sign   = (lbl_address > ass->current_address) ? -1 : 1;
	int offset = ((ass->current_address - lbl_address + 8) * sign )  >> 2;  // compute offet

	BranchInstruct Branchinstr;
	Branchinstr.Cond   = str_to_cond(suffix);
	Branchinstr._101  = 5 % (1 << sizeof(char));
        Branchinstr._0     = 0;
	Branchinstr.Offset = offset;

	return *(int32_t *) &Branchinstr;
}


//////////////////Special Instruction //////////////////////////////////////////

/*andeq func */
//for instr that compute results, the syntax is <opcode> Rd, Rn, <Operand 2>
//andeq is similar to and with cond set to 0000 (eq condition)
//andeq r0, r0, r0
//TOKEN *line is to get 'andeq from the line read'
int32_t andeq_func(TOKEN *line, ASSEMBLER_STRUCT *ass){
  return 0x00000000;
}

/*lsl func */
//note: asprintf() cal the length of the string, allocate that amount of mem and
//write the string into it. it is an implicit malloc need to free afterward
//Compile lsl Rn,<#expression> as mov Rn, Rn, lsl <#expression>
int32_t lsl_func(TOKEN *line, ASSEMBLER_STRUCT *ass){
 char *new_line = NULL;
 asprintf(&new_line, "mov %s, %s, lsl %s", line->tokens[1],
                                                line->tokens[1],
                                                line->tokens[2]);
TOKEN *new_token = (TOKEN*) malloc(sizeof(TOKEN));
new_token = tokenise(new_line, " ,");
return ass_data_proc_mov(new_token, ass);

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

  TOKEN *lines = read_Source(argv[1]);
  printf("after read_source main\n");

  //get lines of assembly codes

  ASSEMBLER_STRUCT *ass = assemble(lines, &assembler_func, " ,");
  printf("after ass main\n");

   //assemble lines using assembler and get output to write to file

  write_File(ass, argv[2]);
  printf("after write main\n");


  tokens_free(lines);
  printf("after token_free main\n");

  free(ass);
  printf("after ass_free main\n");


  return EXIT_SUCCESS;
}
