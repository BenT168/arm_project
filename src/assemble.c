#include <stdlib.h>
#include <stdio.h>

#include "library/instruction.h"

////////////////////////////////ARM STRUCTURE//////////////////////////////////

#include "library/arm11.h"

///////////////////////// STRUCTURE OF INSTRUCTION ////////////////////////////
/////////////////////////////two-pass assembly/////////////////////////////////
#include "library/instruction.h"

////////////////////////////////////MACROS/////////////////////////////////////

#include "library/register.h"
#include "library/tokens.h"
// for numerica constant it's in the form "#x" where x is a natural number
// or in the form "=x" for ldr instr (the expr can be 32 bits after =)
#define Is_Expression(token)  (token[0] == '#' | token[0] == '=')
#define Is_Hexadecimal(token) (Is_Expression(token) & token[1] == '0' & token[2] == 'x')
#define max_8bit_represented = 256; // 2^8 = 256


///////////////////////////two-pass assembly////////////////////////////////////

/////// first pass//////////////////////////////////////////////////////////////

char *buffer;
ASSEMBLER_STRUCT *ass;

TOKENISE_STRUCT read_Source();
void write_File();

TOKENISE_STRUCT read_Source(const char *sourceFile) {
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
    fread(buffer, size, 1, sourceFile);
    if(ferror(sourceFile)) {
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

  int32_t *program = assembler(ass); //get code from assembler program

  int size = ass.TOTAL_line * sizeof(int32_t);
  //size of each element that will be written

  fwrite(program, size, 1, binaryFile);

  fclose(binaryFile);
}

//////////////////////////   Core     //////////////////////////////////////////

void data_processing(int32_t word)
{
	DataProcessingInstruct *DPInst = (DataProcessingInstruct *) &word;

	int ImmOp    = DPInst->ImmOp;        // 25
	int OpCode   = DPInst->Opcode;   // 24-21
	int SetCond  = DPInst->SetCond;
	int Rn       = DPInst->Rn;
	int Rd       = DPInst->Rd;
	int Operand2 = DPInst->Operand2; // 11-0

	int Operand1 = arm_Ptr.registers[Rn];

	Operand2     = IS_CLEAR(ImmOp) ? as_shifted_reg(Operand2, SetCond)
	           		           : as_immediate_reg(Operand2);
	int result   = 0;

	// calculate result by opcode
	switch (Mnemonic)
	{
		case and :
		case eor :
		case sub :
		case rsb :
		case add :
		case orr :

                    break;
		case mov :

                    break;
		case tst :
		case teq :
		case cmp :

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
	if(OpCode != TST || OpCode != TEQ || OpCode != CMP) {
    REG_WRITE(Rd, result);
  }


	if (IS_SET(SetCond)) {
	// set flags
  	    CPSR_PUT(Z, (result == 0));
            CPSR_PUT(N, BIT_GET(result, 31));

            switch (OpCode)  {
  		case 2  :
  		case 3  :
  		case 10 :
        	    CPSR_PUT(C, (result >= 0));
                    break;
  		case 4  :
                    CPSR_PUT(C, CPSR_GET(V));
                    break;
    }
	}
}



int32_t single_data_transfer(int Rd, char *adr)
{
  /*

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
    adr[0] = '#';
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
*/
}

//////////////////////////Instruction //////////////////////////////////////////

/////////////////////// Data Processing ////////////////////////////////////////
char *decimal_to_binary(int number){
  int count = 1, quotient, binary;

  while(number != 0){
    quotient = number % 2;
    number /= 2;
    binary = quotient * count;
    i *= 10;
  }
  return binary;
}

char *hex_to_binary(int number){
  int count = 1, quotient, binary;

  while(number != 0){
    quotient= number
  }
}


int32_t as_shifted_reg(){

}

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
}

//////////////////Special Instruction //////////////////////////////////////////
/*andeq func */

//for instr that compute results, the syntax is <opcode> Rd, Rn, <Operand 2>
//andeq is similar to and with cond set to 0000 (eq condition)
//andeq r0, r0, r0
//TOKENISE_STRUCT *line is to get 'andeq from the line read'
int32_t andeq_func(TOKENISE_STRUCT *token_line){
  return 0x00000000;
}

/*lsl func */
int32_t lsl_func(TOKENISE_STRUCT *token_line, ){

}




///////////////////////// Main /////////////////////////////////////////////////
int main(int argc, char **argv) {

  if(argc < 2) { // Need two files
    printf("Incomplete number of arguments in input!\n");
    printf("Please type in as first argument : ARM source file\n");
    printf("And as aecond argument : an output ARM binary code file\n");
    exit(EXIT_FAILURE);
  }


  TOKENISE_STRUCT *lines = read_Source(argv[1]); // get each line of source code as tokens

  ass = assembler(lines); //assemble lines and get output to write to file

  write_File(argv[2]); //

  free(tokenStruct.lines);

  return EXIT_SUCCESS;
}
