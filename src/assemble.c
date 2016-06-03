#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <inttypes.h>
//////////////////////////////////STRUCTURE//////////////////////////////////

#include "library/arm11.h"
#include "library/assembler.h"
#include "library/instruction.h"

////////////////////////////////////MACROS/////////////////////////////////////

#include "library/register.h"
#include "library/tokens.h"
#include "library/bitwise.h"
#include "library/symbolTableList.h"



// for numerica constant it's in the form "#x" where x is a naturAL number
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

/* SpeciAL */
int32_t andeq_func(TOKEN *, ASSEMBLER_STRUCT *);
int32_t lsl_func(TOKEN *, ASSEMBLER_STRUCT *);
int32_t lsr_func(TOKEN *, ASSEMBLER_STRUCT *);

int mnemonic_to_Opcode(char* mnemonic);


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
    fseek(file, 0, SEEK_SET); //go back to start
    char buffer[size];

    fread(buffer, 1, size, file);
      if(ferror(file)) {
        perror("Error reading from sourceFile.\n");
      }
      buffer[size - 1]= '\0';
      fclose(file);
      //printf("write buffer with comma:%s\n", writeBuffer(buffer));

   return tokenise(writeBuffer(buffer), "\n");
}



void write_File(ASSEMBLER_STRUCT *ass, const char *binaryFile)
{
  //printf("writefile\n");
  FILE *file = fopen(binaryFile, "wb"); //w = write b = binary

  //printf("after opening file (in write_file)\n");
  int32_t *program = assemble_generate_bin(ass);
  //get binary code from assembler program
  //printf("program (in write_file)\n");
  size_t size = ass->TOTAL_line * sizeof(int32_t);
  //size of each element that will be written
  //printf("size: %zu\n",size );
  assert(fwrite(program, 1, size, file) == size);

  //printf("after assertion (in write_file)\n");
  fclose(file);
  puts("print after fclose");

  free(program);
}

//////////////////////////   Core     //////////////////////////////////////////

int as_numeric_constant(int vALue){

  int to_num = 0;

  while (get_bits(vALue, 8 , range_bit - 1 ) != 0 && to_num < range_bit )
  {
     for (int i = 0; i < 2; i++)
      {
        int last = BIT_GET(vALue, 31);
        vALue <<= 1;
        BIT_PUT(vALue, 0, last);
      }
    	to_num += 2;
  }

  if(to_num == range_bit) {
    perror("numerical constant cannot be represented.");
    exit(EXIT_FAILURE);
  }
  return ((to_num / 2) << 8) | vALue;
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

	ShiftReg shiftReg;
	ShiftRegOptional regOp;
  int shiftType = str_to_shift(shift_name);

	//in the form <shiftname><#expression>
	if(Is_Expression(Operand2))
	{
  	//+1 to get rid of 'r' but just getting the reg number
 		shiftReg.Rm = PARSE_REG(Rm - 1);
  	shiftReg.Flag = 0;
  	shiftReg.Type = shiftType;
  	shiftReg.Amount = expr_to_num(Operand2);

  	result = *((int *) &shiftReg);

	} else { //in the form <shiftname><register>
  		regOp.Rm = PARSE_REG(Rm + 2);
  		regOp.Flag1 = 1;
  		regOp.Type = shiftType;
      regOp.Flag2 = 0;
  		regOp.Rs = PARSE_REG(Rm + 1) | (0 << 3);

  //regOp.Rs = atoi(line->tokens[Rm] + 1) << 3; //getting the last bit of Rs

  	result = *((int *) &regOp);
	}

	return result;
}

//to check if operand2 is an expression or a register
int check_op2(TOKEN *line, int op2){
char *operand2 = line->tokens[op2];

  if(Is_Expression(operand2)){
    //printf("!!!num constant:%i\n",as_numeric_constant(expr_to_num(operand2)));
    return as_numeric_constant(expr_to_num(operand2));
  }
  //printf("before going into as_shifted_reg\n");
  return as_shifted_reg_ass(line, op2);

if(Is_Expression(operand2)){
  return as_numeric_constant(expr_to_num(operand2));
}
return as_shifted_reg_ass(line, op2);
}


function_assPtr function_Array[9];

int32_t assembler_func(TOKEN *line, ASSEMBLER_STRUCT *ass) {
  char *mnemonic = line->tokens[0];
  int i = str_to_mnemonic(mnemonic);
  //printf("mnemonic: %i then going to funcArray\n", i);
  return function_Array[i](line, ass);
}

void funcArray(void) {
  printf("Let's start to call functions\n");
  function_Array[0] = ass_data_proc_result;
  function_Array[1] = ass_data_proc_mov;
  function_Array[2] = ass_data_proc_cpsr;

  function_Array[3] = ass_multiply_mul;
  function_Array[4] = ass_multiply_mla;

  function_Array[5] = ass_single_data_transfer;

  function_Array[6] = ass_branch;

  function_Array[7] = lsl_func;
  function_Array[8] = andeq_func;
  function_Array[9] = lsr_func;

}


int mnemonic_to_Opcode(char* mnemonic) {

 if(strcmp(mnemonic, "and") == 0) {
   return 0;
 } else if (strcmp(mnemonic, "eor") == 0) {
    return 1;
 } else if (strcmp(mnemonic, "sub") == 0) {
    return 2;
 } else if (strcmp(mnemonic, "rsb") == 0) {
   return 3;
 } else if (strcmp(mnemonic, "add") == 0) {
   return 4;
 } else if (strcmp(mnemonic, "tst") == 0) {
   return 8;
 } else if (strcmp(mnemonic, "teq") == 0) {
   return 9;
 } else if (strcmp(mnemonic, "cmp") == 0) {
   return 10;
 } else if (strcmp(mnemonic, "orr") == 0) {
   return 12;
 } else if (strcmp(mnemonic, "mov") == 0) {
   return 13;
 } else if (strcmp(mnemonic, "beq") == 0) {
   return 14;
 } else if (strcmp(mnemonic, "bne") == 0) {
   return 15;
 }
   return 0;

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


 int32_t ass_data_proc(TOKEN *line, int SetCond, int Rn, int Rd, int Operand_2)
{
  printf("start of data proccessing\n");
	char *Operand2 = line->tokens[Operand_2];
  //printf("\n");
  //printf("operand_2 position: %i\n", Operand_2);
  //printf("Operand2 is....%s\n",Operand2);
	char *mnemonic = line->tokens[0];
  //printf("mnemonic : %s\n", mnemonic);
  //printf("destination reg: %s\n", line->tokens[1]);
  //printf("\n");

  DataProcessingInstruct DPInst;


	DPInst.Cond	     = AL;
	DPInst._00	     = 0;
	DPInst.ImmOp	   = Is_Expression(Operand2);
  //printf("Immop: %i\n",DPInst.ImmOp);
	DPInst.Opcode	   = mnemonic_to_Opcode(mnemonic);
  //printf("Opcode: %i\n",DPInst.Opcode);
	DPInst.SetCond	 = SetCond;
  //printf("SetCond: %i\n",DPInst.SetCond);
	DPInst.Rn        = PARSE_REG(Rn);
  //printf("Rn (Data_proc): %i\n", PARSE_REG(Rn));
	DPInst.Rd	       = PARSE_REG(Rd);
  //printf("Rd (Data_proc): %i\n", PARSE_REG(Rd));
	DPInst.Operand2  = check_op2(line, Operand_2);
  //printf("Op2 (Data_proc): %i\n", DPInst.Operand2);
  //printf("%" PRId32 "\n", *((int32_t *) &DPInst));
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
  printf("going to do data proccessing result!!!\n");
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
  printf("going to do data proccessing move!!!\n");
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
  printf("going to do data proccessing CPSR!!!\n");
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
  //printf("%s\n", line->tokens[0] );
  //printf("%s\n", line->tokens[1] );
	MultiplyInstruct MulInst;
  //printf("ass_multiply\n");
	MulInst.Cond	   = AL;
	MulInst._000000  = 0;
	MulInst.Acc	     = Acc;
	MulInst.SetCond  = 0;
	MulInst.Rd	     = PARSE_REG(Rd);
  //printf("Rd: %i\n", PARSE_REG(Rd));
	MulInst.Rn	     = PARSE_REG(Rn);
  //printf("Rn: %i\n", PARSE_REG(Rn));
  //printf("Rs: %i\n",Rs );
	MulInst.Rs	     = PARSE_REG(Rs);
  //printf("Rs: %i\n", PARSE_REG(Rs));
  //printf("in mul reg rs\n");
	MulInst._1001	 = 9; //% (1 << sizeof(char));
  //printf("in mul 1001\n");
	MulInst.Rm	     = PARSE_REG(Rm);
  //printf("Rm: %i\n", PARSE_REG(Rm));
  //printf("before return in ass_multiply\n");
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
  //printf("inside multiply mla\n");
  int _ACC        = 1;
  int POS_OF_RD   = 1;
  int POS_OF_RM   = 2;
  int POS_OF_RS   = 3;
  int POS_OF_RN   = 4;
  //printf("before return multiply mla\n");
  return ass_multiply(line, _ACC, POS_OF_RD, POS_OF_RM, POS_OF_RS, POS_OF_RN);
}


int32_t SDT_num_const(TOKEN *line, ASSEMBLER_STRUCT *ass) {
  char *Regd = line->tokens[1];
  char *adr  = line->tokens[2];
  int newAddress = expr_to_num(adr);

  // Treat as mov Instruction
  if (newAddress <= endian) {
    adr[0] = '#';
    line->tokens[0] = strdup("mov");
    return ass_data_proc_mov(line, ass);

  }
    // use PC to calculate new address
    uint16_t last_address = assemble_constant_write(ass, newAddress);
    // off-by-8 bytes effect
    int offset = last_address - ass->current_address - 8;
    //generate ldr r0,[PC, offset]
    char *newline = NULL;

    // PC = 15
    asprintf(&newline, "ldr %s, [PC, #%d]", Regd, offset);
    TOKEN *newtoken = tokenise(newline, " ,");
    return ass_single_data_transfer(newtoken, ass);
  }


int32_t ass_single_data_transfer(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  char *adr  = line->tokens[2];
  char *mnem = line->tokens[0];
  int RnNum = 0;

  if (Is_Expression(adr)) {    // In <=expression> form
    return SDT_num_const(line, ass);
  }

  //It's Pre-indexed address if the expression ends with ']'
  int Pre_index = tokens_endc(line) == ']';
  //initialise I, U, Offset
  int Imm    = 0;
  int UpFlag = 1;
  int offset = 0;


  // get arguements from <address>
  TOKEN *newline = tokenise(strdup(line->line), " ,[]");
  char *expr = newline->tokens[3];
  char *rn = newline->tokens[2];

  //printf("Rn char: %s\n",rn + 1 );
    //printf("expr: %s\n",  expr);

  // Case [Rn]
  if (newline->tokenCount == 3) {
    RnNum = atoi(rn +1);
    offset = 0;

  // Case [Rn, <#expression>]
  } else if (Is_Expression(expr)) {
    RnNum = atoi(rn +1);
    offset = expr_to_num(expr);
    UpFlag = offset >= 0;

  // Case Optional
  } else {
    Imm = 1;                                 // As shifted register

    if (expr[0] == '+' || expr[0] == '-') {  // Check if there is sign
      UpFlag = (expr[0] == '+') ;            // If U is set then + else -
      expr++;                                // Remove the sign
    }

    offset = as_shifted_reg_ass(newline, 3); // As shifted register
    UpFlag = (expr[0] == '+' || expr[0] == '-' ) ? UpFlag : ( offset >= 0 );
  }

  SDTInstruct SDTinstr;

  SDTinstr.Cond   = AL;
  SDTinstr._01	   = 1;
  SDTinstr.ImmOff	  = Imm;
  //printf("Immediate offset is : %i\n", Imm);
  SDTinstr.P	     = Pre_index;
  //char* indexing = (Pre_index == 1) ? "Pre-Indexing" : "Post-indexing";
  //printf("We are doing %s\n !", indexing);
  SDTinstr.Up	     = UpFlag;
  SDTinstr._00	   = 0;
  SDTinstr.L	     = (strcmp(mnem, "ldr") == 0);  //ldr --> L is set
  //printf("mnem :%s\n",mnem);
  SDTinstr.Rn     = RnNum;
  //printf("Rn: %i\n", atoi(rn + 1));
  SDTinstr.Rd	   = PARSE_REG(1);
  //printf("Rd: %i\n", PARSE_REG(1));
  SDTinstr.Offset = offset;
  //printf("Offset: %i\n", offset);

  //tokens_free(line);

  return *((int32_t *) &SDTinstr);

}



int32_t ass_branch(TOKEN *line, ASSEMBLER_STRUCT *ass)
{

  //printf("line->tokens[0]...%s\n", line->tokens[0]);
  //printf("line->tokens[0]...%s\n", line->tokens[0] + 1);

  char first_letter_token = line->tokens[0][0];
  //printf("%c\n", first_letter_token );

  char *suffix = (first_letter_token != 'b') ? "AL" : (line->tokens[0] + 1);
	char *lbl    = line->tokens[1];

  uint16_t lbl_address = list_get_address(ass->symbolTable,lbl);


  int sign   = (lbl_address > ass->current_address) ? -1 : 1;
  // compute offet
	int offset = ((ass->current_address - lbl_address + 8) * sign ) >> 2;

	BranchInstruct Branchinstr;
	Branchinstr.Cond   = str_to_cond(suffix);
	Branchinstr._101   = 5 % (1 << sizeof(char));
  Branchinstr._0     = 0;
	Branchinstr.Offset = offset;

  //printf("PLEASE do %s for %s !!\n", suffix, lbl);

	return *((int32_t *) &Branchinstr);
}


//////////////////SpeciAL Instruction //////////////////////////////////////////

/*andeq func */
//for instr that compute results, the syntax is <opcode> Rd, Rn, <Operand 2>
//andeq is similar to and with cond set to 0000 (eq condition)
//andeq r0, r0, r0
//TOKEN *line is to get 'andeq from the line read'
int32_t andeq_func(TOKEN *line, ASSEMBLER_STRUCT *ass){
  return 0x00000000;
}

/*lsl func */
//note: asprintf() cAL the length of the string, ALlocate that amount of mem and
//write the string into it. it is an implicit mALloc need to free afterward
//Compile lsl Rn,<#expression> as mov Rn, Rn, lsl <#expression>
int32_t lsl_func(TOKEN *line, ASSEMBLER_STRUCT *ass){
char *new_line = NULL;
asprintf(&new_line, "mov %s, %s, lsl %s", line->tokens[1],
                                          line->tokens[1],
                                          line->tokens[2]);
TOKEN *new_token = (TOKEN*) malloc(sizeof(TOKEN));
new_token = tokenise(new_line, " ,");
return ass_data_proc_mov(new_token, ass);

}

/*lsr func */
//note: asprintf() cAL the length of the string, ALlocate that amount of mem and
//write the string into it. it is an implicit mALloc need to free afterward
//Compile lsr Rn,<#expression> as mov Rn, Rn, lsr <#expression>
int32_t lsr_func(TOKEN *line, ASSEMBLER_STRUCT *ass){
char *new_line = NULL;
asprintf(&new_line, "mov %s, %s, lsr %s", line->tokens[1],
                                          line->tokens[1],
                                          line->tokens[2]);
TOKEN *new_token = (TOKEN*) malloc(sizeof(TOKEN));
new_token = tokenise(new_line, " ,");
return ass_data_proc_mov(new_token, ass);

}

////////////////////A factoriAL program ////////////////////////////////////////
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

  //printf("before function_Array\n");
  funcArray();
  //printf("after funcArray\n");

  TOKEN *lines = read_Source(argv[1]);
  //printf("after read_source main\n");
  //printf("lines->tokenCount :%i\n",lines->tokenCount);

  //get lines of assembly codes
  ass = malloc(sizeof(ASSEMBLER_STRUCT));
  //printf("after malloc ass\n");
  ass = assemble(lines, &assembler_func, ",");
  //printf("after ass main\n");

  //assemble lines using assembler and get output to write to file

  write_File(ass, argv[2]);
  //printf("after write main\n");

  tokens_free(lines);
  //printf("after token_free main\n");

  assemble_free(ass);
  //printf("after ass_free main\n");

  return EXIT_SUCCESS;
}
