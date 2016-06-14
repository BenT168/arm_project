

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
void funcArray(void);

TOKEN *read_Source(const char *);
void write_File(ASSEMBLER_STRUCT *, const char *);

int as_numeric_constant(int);
int as_shifted_reg_ass(TOKEN *, int);

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

int mnemonic_to_Opcode(char*);


/////// EXTENSION ///////

/* Block Data Transfer */
int32_t ass_block_data_transfer_ldm(TOKEN *, ASSEMBLER_STRUCT *);
int32_t ass_block_data_transfer_stm(TOKEN *, ASSEMBLER_STRUCT *);

int32_t push(TOKEN *) ;
int32_t pop(TOKEN *) ;

/* Branch and Exchange */
int32_t ass_branch_exchange_bx(TOKEN *, ASSEMBLER_STRUCT *);

/*  Branch With Link */
int32_t ass_branch_w_link_bl(TOKEN *, ASSEMBLER_STRUCT *);

/* Software Interrupt */
int32_t ass_software_interrupt(TOKEN *, ASSEMBLER_STRUCT *);


///////////////////////BINARY FILE READER //////////////////////////////////////

/* Add comma at scape to buffer */
char* writeBuffer(char* buffer) {
  for(int i = 0; i < strlen(buffer); i++) {
    if(buffer[i] == ' ') {
      buffer[i] = ',';
    } else if(buffer[i] == '{' || buffer[i] == '}') {
      buffer[i] = ' ';
      writeBuffer(buffer);
    }
  }
  return buffer;
}

/* Get token from input file */
TOKEN *read_Source(const char *sourceFile)
{
  FILE *file = fopen(sourceFile, "rt");
  puts("in read source");
  fseek(file, 0, SEEK_END);
  /* Size of file in bytes */
  long size = ftell(file);
  fseek(file, 0, SEEK_SET); //go back to start
  char buffer[size];
  puts("before fread");
  fread(buffer, 1, size, file);
  puts("after fread");
  /* Check the file */
  if(ferror(file)) {
    perror("Error reading from sourceFile.\n");
  }

  buffer[size - 1]= '\0';
  printf("buffer: %s\n",buffer);

  return tokenise(writeBuffer(buffer), "\n");
}

void write_File(ASSEMBLER_STRUCT *ass, const char *binaryFile)
{
  FILE *file = fopen(binaryFile, "wb");  //w = write, b = binary

  /* Check the file */
  if(!file) {
     perror("file did not open correctly");
     exit(EXIT_FAILURE);
   }

   /* get binary code from assembler program */
  int32_t *program = assemble_generate_bin(ass);
  puts("after program");
  /* size of each element that will be written */
  size_t size = ass->TOTAL_line * sizeof(int32_t);
  puts("after size");
  /* Check size */
  assert(fwrite(program, 1, size, file) == size);
  puts("after fwrite");
  free(program);
  puts("free");
  fclose(file);
  puts("after close");
}

/////////////////////////////// CORE ////////////////////////////////////////

/* Initialising Function Array */

void funcArray(void);

function_assPtr function_Array[14];

/* Call the proper function */
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

  function_Array[9]  = ass_block_data_transfer_ldm;
  function_Array[10] = ass_block_data_transfer_stm;

  function_Array[11] = ass_branch_exchange_bx;
  function_Array[12] = ass_branch_w_link_bl;

  function_Array[13] = ass_software_interrupt;

}

/* Transfer mnemonics into corresponding opcode */
int mnemonic_to_opcode(char* mnemonic) {
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

int32_t assembler_func(TOKEN *line, ASSEMBLER_STRUCT *ass) {
  char *mnemonic = line->tokens[0];
  int i = str_to_mnemonic(mnemonic);
  printf("func int: %i\n",i);
  return function_Array[i](line, ass);
}

//////////////////////////////// MAIN ////////////////////////////////////////
int main(int argc, char **argv)
{
  /* Need two files (+ executer) */
  if(argc < 3) {
    printf("Incomplete number of arguments in input!\n");
    printf("Please type in as first argument : ARM source file\n");
    printf("And as aecond argument : an output ARM binary code file\n");
    exit(EXIT_FAILURE);
  }

  funcArray();
    puts("after funcarray\n");
  TOKEN *lines = read_Source(argv[1]);
  puts("after read\n");
  /* get lines of assembly codes */
  ass = malloc(sizeof(ASSEMBLER_STRUCT));
  ass = assemble(lines, &assembler_func, ",");
  puts("after assemble");
  /* assemble lines using assembler and get output to write to file */
  write_File(ass, argv[2]);
  puts("after write");

  tokens_free(lines);

  assemble_free(ass);

  return EXIT_SUCCESS;
}


//////////////////////////   SHIFTING    //////////////////////////////////////

int as_numeric_constant(int value){
  /* initialise the constant integer */
  int to_num = 0;

  /* check if the non-zero integer is in the range */
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

  /* process shifting */
  return ((to_num / 2) << 8) | value;
}

/* can be either <shiftname><register> or <shiftname><#expression>
   <shiftname> can be either asr, lsl, lsr or ror
   If operand2 is a register the 12-bit is shift(11-4)+Rm(3-0)
   first case integer(11-7)+shift type(6-5)+0(4)
   second case shiftReg RS(11-8)+0(7)+shift type(6-5)+1(4)
   TOKEN *elem is a pointer to elems in tokenized line */

int as_shifted_reg_ass(TOKEN *line, int Rm)
{
  if(line->tokenCount == Rm + 1)
  {
    return PARSE_REG(Rm);
  }

  ShiftReg shiftReg;
	ShiftRegOptional regOp;

	char *shift_name = line->tokens[Rm + 1];
	char *Operand2   = line->tokens[Rm + 2];
  int shiftType    = str_to_shift(shift_name);
  int result       = 0;

	/* in the form <shiftname><#expression> */
	if(Is_Expression(Operand2))
	{
 		shiftReg.Rm = PARSE_REG(Rm);
		shiftReg.Flag = 0;
		shiftReg.Type = shiftType;
		shiftReg.Amount = expr_to_num(Operand2);

  	result = *((int *) &shiftReg);

	} else { /* in the form <shiftname><register> */

    regOp.Rm = PARSE_REG(Rm);
    regOp.Flag1 = 1;
    regOp.Type = shiftType;
    regOp.Flag2 = 0;
    /* the amount is specify 2 spaces after Rn */
    regOp.Rs = PARSE_REG(Rm + 2) | (1 << 4);

  	result = *((int *) &regOp);
	}

	return result;
}

/* Check if operand2 is an expression or a register */
int check_op2(TOKEN *line, int op2){
  char *operand2 = line->tokens[op2];

  if(Is_Expression(operand2)){
    /* case for shift by a constant */
    return as_numeric_constant(expr_to_num(operand2));
  }
  /* case for shift by a register */
  return as_shifted_reg_ass(line, op2);

}


////////////////////// ASEMBLE INSTRUCTIONS //////////////////////////////////

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
	char *Operand2 = line->tokens[Operand_2];
	char *mnemonic = line->tokens[0];

  DataProcessingInstruct DPInst;

	DPInst.Cond	     = AL;
	DPInst._00	     = 0;
	DPInst.ImmOp	   = Is_Expression(Operand2);
	DPInst.Opcode	   = mnemonic_to_opcode(mnemonic);
	DPInst.SetCond	 = SetCond;
	DPInst.Rn        = PARSE_REG(Rn);
	DPInst.Rd	       = PARSE_REG(Rd);
	DPInst.Operand2  = check_op2(line, Operand_2);

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
	MultiplyInstruct MulInst;

	MulInst.Cond	   = AL;
	MulInst._000000  = 0;
	MulInst.Acc	     = Acc;
	MulInst.SetCond  = 0;
	MulInst.Rd	     = PARSE_REG(Rd);
	MulInst.Rn	     = PARSE_REG(Rn);
	MulInst.Rs	     = PARSE_REG(Rs);
	MulInst._1001	   = 9;
	MulInst.Rm	     = PARSE_REG(Rm);

	return *((int32_t *) &MulInst);
}

/* 1. multiply with syntax: mul Rd, Rm, Rs
*
*     Instruction doesnt have an accumulate, hence Acc = 0.
*     Positions of tokens Rd, Rm and Rs are 1, 2, and 3 respectively.
*     Rn is ignored therefore -1 is passed as the 'position' of token Rn.
*/

int32_t ass_multiply_mul(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  int N_ACC       =  0;
  int POS_OF_RD   =  1;
  int POS_OF_RM   =  2;
  int POS_OF_RS   =  3;
  int RN_IGNORED  = -1;

  return ass_multiply(line, N_ACC, POS_OF_RD, POS_OF_RM, POS_OF_RS, RN_IGNORED);
}

/* 2. multiply with accumulate and with syntax: mla Rd, Rm, Rs, Rn
*
*     Instruction have an accumulate, hence Acc = 1.
*     Positions of tokens Rd, Rm ,Rs and Rn are 1, 2, 3 and 4 respectively.
*/


int32_t ass_multiply_mla(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  int _ACC        = 1;
  int POS_OF_RD   = 1;
  int POS_OF_RM   = 2;
  int POS_OF_RS   = 3;
  int POS_OF_RN   = 4;
  return ass_multiply(line, _ACC, POS_OF_RD, POS_OF_RM, POS_OF_RS, POS_OF_RN);
}


////////* Single Data Transfer *////////

int32_t ass_single_data_transfer(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  /* input is in form <ldr/str> Rd, <address> */
  char *adr  = line->tokens[2];
  char *mnem = line->tokens[0];
  /* initialise content of base register */
  int RnNum  = 0;


  /* In the case <=expression> */
  if (Is_Expression(adr)) {
    return SDT_num_const(line, ass);
  }

  /* It's Pre-indexed address if the expression ends with ']' */
  int Pre_index = tokens_endc(line) == ']';

  /* initialise I, U, Offset */
  int Imm    = 0;
  int UpFlag = 1;
  int offset = 0;

  /* get arguements from <address> */
  TOKEN *newline = tokenise(strdup(line->line), " ,[]");
  char *expr = newline->tokens[3];
  char *rn = newline->tokens[2];

  if (newline->tokenCount == 3) {   // Case [Rn]
    RnNum = atoi(rn +1);
    offset = 0;

  } else if (Is_Expression(expr)) {   // Case [Rn, <#expr>] or [Rn] <#expr>
    RnNum = atoi(rn +1);
    if(strcmp(adr, "[PC") != 0) {
      offset = abs(expr_to_num(expr));
      UpFlag = offset >= 0;
      if(expr_to_num(expr) < 0) {
        UpFlag = 0;
      }
    } else {   // Case Rn, [PC, offset]
      RnNum = PC;
      offset = abs(expr_to_num(expr));
    }


  } else {   // Case Optional

    Imm = 1;
    /* Check if there is sign */
    if (expr[0] == '+' || expr[0] == '-') {
      /* If U is set then '+' else '-' */
      UpFlag = (expr[0] == '+') ;
      /* Remove the sign */
      expr++;
    }
    RnNum = atoi(rn+1);
    /* As shifted register */
    offset = as_shifted_reg_ass(newline, 3);

    tokens_free(newline);
  }



  SDTInstruct SDTinstr;

  SDTinstr.Cond      = AL;
  SDTinstr._01	     = 1;
  SDTinstr.ImmOff    = Imm;
  SDTinstr.P	       = Pre_index;
  SDTinstr.Up	       = UpFlag;
  SDTinstr._00	     = 0;
  SDTinstr.L	       = (strcmp(mnem, "ldr") == 0);  //ldr --> L is se
  SDTinstr.Rn        = RnNum;
  SDTinstr.Rd	       = PARSE_REG(1);
  SDTinstr.Offset    = offset;

  return *((int32_t *) &SDTinstr);
}

/* In the case <=expression> */
int32_t SDT_num_const(TOKEN *line, ASSEMBLER_STRUCT *ass) {
  char *Regd = line->tokens[1];
  char *adr  = line->tokens[2];
  int newAddress = expr_to_num(adr);

  /* Treat as mov Instruction if less than 0xFF */
  if (newAddress <= endian) {
    adr[0] = '#';
    line->tokens[0] = strdup("mov");
    return ass_data_proc_mov(line, ass);
  }

  /* Use PC to calculate new address */
  uint16_t last_address = assemble_constant_write(ass, newAddress);
  int offset = last_address - ass->current_address - 8; // off-by-8 bytes effect

  /* initialise ldr r0,[PC, offset] */
  char *newline = NULL;

  /* generate new token */
  asprintf(&newline, "ldr %s, [PC, #%d]", Regd, offset);
  TOKEN *newtoken = tokenise(newline, " ,");
  return ass_single_data_transfer(newtoken, ass);
}


////////* Branch *////////

int32_t ass_branch(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  /* get the suffix or treat b as al: ignore it */
  char *suffix = (strcmp(line->tokens[0], "b") == 0) ? "al"
                                                     : (line->tokens[0] + 1);
  /* get the label */
	char *lbl    = line->tokens[1];

  /* get the address of label */
  uint16_t lbl_address = list_get_address(ass->symbolTable, lbl);

  /* compute offset */
  int sign   = (lbl_address < ass->current_address) ? -1 : 1;
  /* oﬀ-by-8 bytes eﬀect and shift right by 2 bits */
	int offset = ( sign * (ass->current_address - lbl_address + 8)) >> 2;

	BranchInstruct BranchInstr;

	BranchInstr.Cond   = str_to_cond(suffix);
  BranchInstr.Link   = 0;
	BranchInstr._101   = 5;
  BranchInstr.Offset = offset;

	return *((int32_t *) &BranchInstr);
}

//////* Special Instruction *///////

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


///////////////////// EXTENSION : INSTRUCTIONS ////////////////////////////////


////////* Block Data Transfer *////////

int32_t ass_block_data_transfer(TOKEN *line, int L, int P, int Up)
{
  char** regList = (char**)malloc(sizeof(char*) * line->tokenCount - 2);
  int count = line->tokenCount - 2; //number of registers in reglist
  int j = 2;
  for(int i = 0; i < line->tokenCount - 2; i++) {
    if(line->tokens[j][0] == '/') { //remove comment from regList
      //regList[i] = (char*)malloc(sizeof(char) * (strlen(line->tokens[j]) + 1));
      //regList = (char**)realloc(regList, sizeof(char*) * line->tokenCount -3);
      puts("is comment");
      count = line->tokenCount - 3;
      break;
    }
    regList[i] = line->tokens[j];
    j++;
  }
  char *rn         = strtok(line->tokens[1], "!"); // remove "!"
  uint16_t RegList = 0;
  uint16_t mask;
  char* setS = "n";\

    for(int i = 0; i < count; i++) {
      printf("reglist[i]: %s\n",regList[i]);
    if (strchr(regList[i], '-') == NULL) {
      if(strcmp(regList[i],"^") == 0) {
        setS = strdup(regList[i]);
        goto endloop;
      } else if(regList[i][0] == '/') { //comment encountered
      puts("in here");
      break;
    }
      mask = (uint16_t) 1 << PARSE_REG(expr_to_num(regList[i]));
    } else {
      char* savePtr;
      char* first = strtok_r(regList[i], "-", &savePtr);
      char* last  = strtok_r(NULL, "-", &savePtr);
                                                          //...,Ra-Rb,...
      uint8_t first_reg = PARSE_REG(expr_to_num(first));
      printf("first: %i\n", first_reg);
      uint8_t last_reg   = PARSE_REG(expr_to_num(last));
      printf("last: %i\n", last_reg);
      if (last_reg > 15) {
        perror("Register number greater than 15");
        exit(EXIT_FAILURE);
      }
      mask = (uint16_t) ((uint16_t) ~0 >> (15 - last_reg)) << first_reg;
   }

    RegList |= mask;
  }
  endloop:;

  BDTInstruct BDTInst;

  BDTInst.Cond    = AL;
  BDTInst._100    = 4;
  BDTInst.P       = P;
  BDTInst.Up      = Up;
  BDTInst.S       = strcmp(setS, "^") == 0;
  puts("after S");
  BDTInst.W       = (rn[strlen(rn) - 1] == '!') == 0;
    puts("after W");
  BDTInst.L       = L;
  BDTInst.Rn      = expr_to_num(rn);
    puts("after Rn");

  BDTInst.RegList = RegList;




  return *((int32_t *) &BDTInst);
}

/*
Load/Store bit [L]
0 = Store to memory
1 = Load from memory

Up/Down bit [Up]
0 = down : subtract offset from base
1 = up   : add offset to base

Pre/Post indexing bit [P]
0 = post : add offset after transfer
1 = pre  : add offset before transfer
*/

int32_t ass_block_data_transfer_ldm(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  char *suffix    = line->tokens[0] + 3;
  int L; int P; int Up;

  if (strcmp(suffix, "ed") == 0) {
    L = 1; P = 1; Up = 1;
  } else if (strcmp(suffix, "fd") == 0) {
    L = 1; P = 0; Up = 1;
  } else if (strcmp(suffix, "ea") == 0) {
    L = 1; P = 1; Up = 0;
  }  else if (strcmp(suffix, "fa") == 0) {
    L = 1; P = 0; Up = 0;
  }

  return ass_block_data_transfer(line, L, P, Up);
}

int32_t ass_block_data_transfer_stm(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  puts("in stm");
  char *suffix    = line->tokens[0] + 3;
  int L; int P; int Up;

  if (strcmp(suffix, "fa") == 0) {
    L = 0; P = 1; Up = 1;
  } else if (strcmp(suffix, "ea") == 0) {
    L = 0; P = 0; Up = 1;
  } else if (strcmp(suffix, "fd") == 0) {
    L = 0; P = 1; Up = 0;
  }  else if (strcmp(suffix, "ed") == 0) {
    L = 0; P = 0; Up = 0;
  }

  puts("just stm");

  return ass_block_data_transfer(line, L, P, Up);
}


TOKEN* concat(TOKEN *line, char *s2)
{
    char *new_line;
    new_line = (char *) malloc(sizeof(line) + 4 );

    strcpy(new_line, line->tokens[0]);
    strcat(new_line, s2);
    strcat(new_line, line->tokens[1]);
    return (TOKEN *) new_line;
}


int32_t push(TOKEN *line)
{
  line = concat(line, "sp!");
  return ass_block_data_transfer(line, 0, 1, 0); //stmfd
}

int32_t pop(TOKEN *line)
{
  line = concat(line, "sp!");
  return ass_block_data_transfer(line, 1, 0, 1); //ldmfd
}

////////* Branch And Exchange *///////

int32_t ass_branch_exchange_bx(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  char *suffix = (strcmp(line->tokens[0], "bx") == 0) ? "al"
                                                     : (line->tokens[0] + 2);
  char *rn    = line->tokens[1];

  BXInstruct BXInstr;

  BXInstr.Cond    = str_to_cond(suffix);
  BXInstr._24bits = 0x12fff1;
  BXInstr.Rn      = PARSE_REG((expr_to_num(rn)));

  return  *((int32_t *) &BXInstr);
}

///////*  Branch With Link */////////

int32_t ass_branch_w_link_bl(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  int32_t normal_branch = ass_branch(line, ass);

  BranchInstruct *BranchwLinkInstr = (BranchInstruct *) &normal_branch;
  BranchwLinkInstr->Link = 1; /* link bit is set */

  return *((int32_t *) BranchwLinkInstr);
}


////////* Software Interrupt *////////

int32_t ass_software_interrupt(TOKEN *line, ASSEMBLER_STRUCT *ass)
{
  char *suffix = (strcmp(line->tokens[0], "swi") == 0) ? "al"
                                      : (line->tokens[0] + 3);

  SoftwareInterruptInstruct SWInstr;

	SWInstr.Cond   = str_to_cond(suffix);
	SWInstr._1111  = 15;

	return *((int32_t *) &SWInstr);
}
