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

/* Parsing Macro */

// for numerica constant it's in the form "#x" where x is a natural number
// or in the form "=x" for ldr instr (the expr can be 32 bits after =)

#define Is_Hexadecimal(token) (Is_Expression(token) & token[1] == '0' & token[2] == 'x')
#define max_8bit_represented  256 // 2^8 = 256
#define expr_to_num(expr)    (strtol(expr, NULL, 0))

///////////////////////////// FUNCTION PROTOTYPE //////////////////////////////


char *buffer;
<<<<<<< HEAD

=======
>>>>>>> 060be51292c285b7561429446774c8980d409fd8
ASSEMBLER_STRUCT *ass = NULL;

TOKEN read_Source(const char *);
void write_File(const char *);

int as_numeric_constant(int);
int as_shifted_reg_ass(TOKEN *, int);

int32_t ass_data_proc(TOKEN *, int, int, int, int);
int32_t ass_data_proc_result(TOKEN *);
int32_t ass_data_proc_mov(TOKEN *);
int32_t ass_data_proc_cpsr(TOKEN *);

int32_t ass_multiply(TOKEN *, int, int, int, int, int);
int32_t ass_multiply_mul(TOKEN *);
int32_t ass_multiply_mla(TOKEN *);

int32_t ass_single_data_transfer(TOKEN *);
int32_t SDT_num_const(int, int, char *);

int32_t andeq_func(TOKEN *);
int32_t lsl_func(TOKEN *);



///////////////////////Binary file reader //////////////////////////////////////

TOKEN read_Source(const char *sourceFile) {
  FILE *file = fopen(sourceFile, "rs");

  if (file != NULL) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file); //Size of file in bytes
    fseek(file, 0, SEEK_SET); //go back to start

    buffer = (char *) malloc(size * sizeof(char));
    if(buffer == NULL) {
      perror("Malloc failed.");
    } else {
    //const char s = ",";
    //char *token;
    //token = strtok(buffer, s);
    fread(buffer, size, 1, file);
    if(ferror(file)) {
      perror("Error reading from sourceFile.\n");
    }

    int sizeBuffer = sizeof(buffer);

    buffer[sizeBuffer - 1]= '\0';

    return tokenise(buffer, ",");

  }

  free(buffer);

} else {
  perror("Error opening the file.");
}

}

void write_File(const char *binaryFile) {
  FILE *file = fopen(binaryFile, "wb"); //w = write b = binary

   //TODO
  //int32_t *program = assembler(ass); //get code from assembler program

  //int size = ass->TOTAL_line * sizeof(int32_t);
  //size of each element that will be written

  //fwrite(program, size, 1, file);

  fclose(file);
}

//////////////////////////   Core     //////////////////////////////////////////

//TODO: CHECKKKKKK!!!!!!
int as_numeric_constant(int value){
  int num_bit = 0;

  while(num_bit < 32){
    rotate_right(value, 2);
    num_bit += 2;
  }
  if(value > max_8bit_represented) {
    perror("numerical constant cannot be represented.");
    exit(EXIT_FAILURE);
  }
  return num_bit;
}

// can be either <shiftname><register> or <shiftname><#expression>
//<shiftname> can be either asr, lsl, lsr or ror
//If operand2 is a register the 12-bit is shift(11-4)+Rm(3-0)
  //first case integer(11-7)+shift type(6-5)+0(4)
  //second case shiftReg RS(11-8)+0(7)+shift type(6-5)+1(4)
//TOKEN *elem is a pointer to elems in tokenized line
int as_shifted_reg_ass(TOKEN *line, int pos_of_Rm)
{
  	char *shift_name = line->tokens[pos_of_Rm + 1];
  	char *Operand2 = line->tokens[pos_of_Rm + 2];
  	int  result = 0;

	ShiftReg 	 shiftReg;
  	ShiftRegOptional regOp;
  	int shiftType = str_to_shift(shift_name);

	//in the form <shiftname><#expression>
	if(Is_Expression(Operand2))
	{
  	//+1 to git rid of 'r' but just getting the reg number
 		shiftReg.Rm = PARSE_REG(pos_of_Rm - 1); 
  		shiftReg.Flag = 0;
  		shiftReg.Type = shiftType;
  		shiftReg.Amount = atoi(Operand2);

  		result = *((int *) &shiftReg);

	} else { //in the form <shiftname><register>
	  //CHECK THE STRUC?!??!
  		regOp.Rm = PARSE_REG(pos_of_Rm + 2);
  		regOp.Flag1 = 0;
  		regOp.Type = shiftType;
		regOp.Flag2 = 0;
  		regOp.Rs = PARSE_REG(pos_of_Rm) | (1 << 4);

  //regOp.Rs = atoi(token_line->tokens[pos_of_Rm] + 1) << 3; //getting the last bit of Rs

  		result = *((int *) &regOp);
	}	

	return result;
}

//to check if operand2 is an expression or a register
int check_op2(TOKEN *token_line, int pos_of_op2){
  char *op2 = token_line->tokens[pos_of_op2 + 2];

  if(Is_Expression(op2)){
    return as_numeric_constant(atoi(op2));
  }
  return as_shifted_reg_ass(token_line, pos_of_op2);

}

///////////////////////Instructions ////////////////////////////////////////////


int32_t ass_data_proc(TOKEN *line, int SetCond, int Rn, int Rd, int Operand_2)
{
	char *Operand2 = line->tokens[Operand_2];
	char *mnemonic = line->tokens[0];

	static DataProcessingInstruct *DPInst;

	DPInst->Cond	= AL;
	DPInst->_00	= 0;
	DPInst->ImmOp	= Is_Expression(Operand2);
	DPInst->Opcode	= str_to_mnemonic(mnemonic);
	DPInst->SetCond	= SetCond;
	DPInst->Rn	= PARSE_REG(Rn);
	DPInst->Rd	= PARSE_REG(Rd);
	DPInst->Operand2= check_op2(line, Operand_2);

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
  return ass_data_proc(line, 1,  1, -1, 2);
}

int32_t ass_multiply(TOKEN *line, int Acc, int Rd, int Rm, int Rs, int Rn)
{
	static MultiplyInstruct *MulInst;

	MulInst->Cond	   = AL; 
	MulInst->_000000   = 0;
	MulInst->Acc	   = Acc;
	MulInst->SetCond   = 0;
	MulInst->Rd	   = PARSE_REG(Rd);
	MulInst->Rn	   = PARSE_REG(Rn);
	MulInst->Rs	   = PARSE_REG(Rs);
	MulInst->_1001	   = 0;
	MulInst->Rm	   = PARSE_REG(Rm);

	return *((int32_t *) &MulInst);
}

int32_t ass_multiply_mul(TOKEN *line)
{
  return ass_multiply(line, 0, 1, 2, 3, -1);
}

int32_t ass_multiply_mla(TOKEN *line)
{
  return ass_multiply(line, 1, 1, 2, 3,  4);
}

int32_t ass_single_data_transfer(TOKEN *line)
{
  int *Rn    = PARSE_REG(line->tokens[1] + 1);
  char *adr = line->tokens[2];
  SDTInstruct *SDTInst = (SDTInstruct *) &line;

  int dataRn     = SDTInst->Rn;         // base register
  int dataOffset = SDTInst->Offset;
  int dataI      = SDTInst->I;
  int dataP      = SDTInst->P;
  int dataU      = SDTInst->U;
  int dataL      = SDTInst->L;

  if (IS_SET(dataL)) {                    // ldr: Load from memory into register
    if (Is_Expression(adr)) {            // In numeric form
      adr++;
      int address = expr_to_num(adr);
      return SDT_num_const(Rn, address, adr);
    }

    if (IS_SET(dataP)) {                  // Pre-indexing
      int offset = 0;
      if (Is_Expression(adr[1])) {       //Case offset = <#expression>
        offset = expr_to_num(adr[1]);
      }
      dataRn += (IS_SET(dataU)? dataOffset : -dataOffset);
      IS_SET(dataL) ? (word = MEM_R_32bits(dataRn)) : MEM_W_32bits(dataRn, word);

    } else {                              // Post-indexing
      int offset = expr_to_num(*adr[1]);
      IS_SET(dataL) ? (word = MEM_R_32bits(dataRn)) : MEM_W_32bits(dataRn, word);
      dataRn += (IS_SET(dataU)? dataOffset : -dataOffset);
    }
  }
}

int32_t SDT_num_const(int r0, int address, char *adr) {
  if (address <= 0xFF) {                  // Treat as mov Instruction
    adr[0] = '#';
    return data_processing(mov r0, adr);

  } else {
    // use PC to calculate new address
    register[15] += (IS_SET(dataU)? dataOffset : -dataOffset);
  }
}


//////////////////////////Instruction //////////////////////////////////////////

/////////////////////// Data Processing ////////////////////////////////////////
//a lookup table for binary numbers
/*
static const char *const binaries[4][15] = {{"0000","0001","0010","0011","0100"\
                                    ,"0101", "0110","0111","1000","1001"\
                                    ,"1010","1011","1100","1101","1110","1111"}};

const int *decimal_to_binary(int number){
  int count = 1, quotient, binary;

  while(number != 0) {
    quotient = number % 2;
    number /= 2;
    binary = quotient * count;
    count *= 10;
  }
  return binary;
}
*/
/*const char *hex_to_binary(char number){
  if(number >= '0' & number <= '9') return binaries[number];
  if(number >= 'A' & number <= 'F') return binaries[10 + number - 'A'];
  if(number >= 'a' & number <= 'f') return binaries[10 + number - 'a'];
}
*/


/*int32_t single_data_transfer(int Rd, char *adr)
{


  int dataRn     = SDTInst->Rn;         // base register
  int dataOffset = SDTInst->Offset;
  int dataI      = SDTInst->I;
  int dataP      = SDTInst->P;
  int dataU      = SDTInst->U;
  int dataL      = SDTInst->L;


  int address = get_value(adr);
  if (IS_SET(dataL)) {                    // ldr: Load from memory into register
    if (adr[0] == '=') {                  // In numeric form
      return SDT_num_const(Rd, address, *adr);
    }

    if (IS_SET(dataP)) {                  // Pre-indexing
      return SDT_PreIndexing(int Rd, char *adr);
    } else {                              // Post-indexing
      return SDT_PsotIndexing(int Rd, char *adr);
    }
  }

}

int32_t SDT_num_const(int r0, int address, char *adr) {

  if (address <= 0xFF) {                  // Treat as mov Instruction
    adr[0] = '#'; //TODO: can use Is_Expression here :)
    return data_processing(mov r0, adr);

  } else {
    //TODO place address in four bytes
  }
}

int32_t SDT_PreIndexing(int Rd, char *adr) {
  int offset = 0;
  if ((*adr[1])[0] == '#') {              //Case offset = <#expression>
    offset = get_value(*adr[1]);
  }
  dataRn += (IS_SET(dataU)? dataOffset : -dataOffset);
  IS_SET(dataL) ? (word = MEM_R_32bits(dataRn)) : MEM_W_32bits(dataRn, word);
}

int32_t SDT_PostIndexing(int Rd, char *adr) {
  int offset = get_value(*adr[1]);
  IS_SET(dataL) ? (word = MEM_R_32bits(dataRn)) : MEM_W_32bits(dataRn, word);
  dataRn += (IS_SET(dataU)? dataOffset : -dataOffset);
  return as_shifted_reg(line, pos_of_op2);

}
*/

//////////////////Special Instruction //////////////////////////////////////////

/*andeq func */
//for instr that compute results, the syntax is <opcode> Rd, Rn, <Operand 2>
//andeq is similar to and with cond set to 0000 (eq condition)
//andeq r0, r0, r0
//TOKEN *line is to get 'andeq from the line read'
int32_t andeq_func(TOKEN *token_line){
  return 0x00000000;
}

/*lsl func */
//note: asprintf() cal the length of the string, allocate that amount of mem and
//write the string into it. it is an implicit malloc need to free afterward
//Compile lsl Rn,<#expression> as mov Rn, Rn, lsl <#expression>
int32_t lsl_func(TOKEN *token_line){ 
 char *new_token_line = NULL;
 asprint(&new_token_line, "mov %s, %s, lsl %s", token_line->tokens[1],
                                                token_line->tokens[1],
                                                token_line->tokens[2]);
TOKEN *new_token = tokenise(new_token_line, " ,");
ass_data_proc_mov(new_token);

free(new_token_line);

}

////////////////////A factorial program ////////////////////////////////////////
//ARM stores instructions using Little-endian


///////////////////////// Main /////////////////////////////////////////////////
int main(int argc, char **argv) {

  if(argc < 2) { // Need two files
    printf("Incomplete number of arguments in input!\n");
    printf("Please type in as first argument : ARM source file\n");
    printf("And as aecond argument : an output ARM binary code file\n");
    exit(EXIT_FAILURE);
  }

  //TODO allocate ASSEMBLER_STRUCT
   //  *ass = malloc()


  //TOKEN *lines = read_Source(argv[1]); // get each line of source code as tokens

  //ass = assembler(lines); //assemble lines and get output to write to file

  write_File(argv[2]); //

  //free(lines);

  return EXIT_SUCCESS;
}
