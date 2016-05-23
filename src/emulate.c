#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

////////////////////////////////ARM STRUCTURE//////////////////////////////////

#include "library/arm11.h"

///////////////////////// STRUCTURE OF INSTRUCTION ////////////////////////////

#include "library/instruction.h"

////////////////////////////////////MACROS/////////////////////////////////////

#include "library/bitwise.h"
#include "library/registerwise.h"

/* Memory Read/Write */
#define MEMORY_READ(m)     (arm_Ptr->memory[m])
#define MEMORY_WRITE(m, b) (arm_Ptr->memory[m] = b)


///////////////////////////// FUNCTION PROTOTYPE //////////////////////////////

void read_ARM(const char *); 
 
void emulator(void); 
int  check_cond(int32_t); 
void decode_instr(int32_t);
void decode_checker(int32_t);
void print_register_state(void); 

void data_processing(int32_t); 
int32_t convert();
int32_t convert2complement();
void multiply(int32_t);
void single_data_transfer(int32_t); 
void branch(int32_t); 

////////////////////////// BINARY FILE LOADER ////////////////////////////////


ARM_State *arm_Ptr = NULL;

void read_ARM(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    //file = fopen(binFile, "rb"); //open the file r = read , b = binary

    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        long size = ftell(file); //Size of file in bytes
        fseek(file, 0, SEEK_SET); //go back to start

        //Allocate memory
        arm_Ptr =  alloc(1, size+1);
        if(!arm_Ptr)
        {
            char errorMsg[] = "Memory Error!";
            fprintf(stderr, "%s\n", errorMsg);
            fclose(file);
            return;
        }

        //Read instructions into ARM Memory
        // for(int i = 0; i < MEMORY_CAPACITY; i++) {
        size_t newLen = fread(arm_Ptr->memory, size, file);
        if (newLen == 0)
        {
            fputs("Error reading file", stderr);
        } else {
           fclose(file);
        }

    } else {
        perror("Error opening file");
    }
}

//////////////////////////// EMULATE CORE /////////////////////////////////////

/* emulator */
void emulator()
{   
    REG_WRITE(PC, 0);
    arm_Ptr->pipeline->fetched = MEMORY_READ(REG_READ(PC));
    INC_PC(4);

    int32_t fetched_code = arm_Ptr->pipeline->fetched;

    while(fetched_code != 0)
    {
     //TODO : NEED A LOOP HERE WHAT I THE CONDITION????
     //for a cycle of pipeline, previously fetched instr is decoded and ancestor ints is executed.
        arm_Ptr->pipeline->decoded = arm_Ptr->pipeline->fetched;
        arm_Ptr->pipeline->fetched = MEMORY_READ(REG_READ(PC));
        INC_PC(4);

        int cond_check = check_cond(fetched_code);

  //If the condition matched, we can execute the instr
  //TODO !!!!!!
        if(cond_check == 1)
            decode_instr(arm_Ptr->pipeline->decoded);

  

  //the emulator should terminate when it executes an all-0 instr
  //Upon termination, output the state of all the registers
        print_register_state();
     }
}

/*decode instruction */
void decode_instr(int32_t word)
{
    int code = bits_get(word, 26, 27);
    switch (code)
    {
	case 1 : 
	    single_data_transfer(word); 
            break;
	case 2 : 
            branch(word);               
            break;
	case 0 :
	    decode_checker(word);
	    break;
	default :
            break;
	}
}

/* helper function for decode_instr */
void decode_checker(int32_t word)
{
	if (IS_SET(BIT_GET(word, 25) || IS_CLEAR(BIT_GET(word, 4))))    
            data_processing(word);
	else    
           (IS_SET(BIT_GET(word, 7))) ? multiply(word) : data_processing(word);        
}     

/* Check condiitons */
int check_cond(int32_t word)
{
    int cond = get_bits(word, 28, 31);

    switch(cond){
 	 case(EQ): 
		return CPSR_GET(Z); 
		break;
	 case(NE): 
		return !CPSR_GET(Z); 
		break;
 	 case(GE): 
		return CPSR_GET(N) == CPSR_GET(V); 
		break;
 	 case(LT): 
		return CPSR_GET(N) != CPSR_GET(V); 
		break;
 	 case(GT): 
		return CPSR_GET(Z) & (CPSR_GET(N) == CPSR_GET(V)); 
		break;
  	 case(LE): 
		return CPSR_GET(Z) | (CPSR_GET(N) != CPSR_GET(V)); 
		break;
  	 case(AL): 
		return 1; 
		break;
	 default;
		return 0;

    }
}

/* Print Register State (upon termination) */
void print_register_state()
{
    printf("Registers state: \n");
    printf("General registers: \n");
    //Register 0 - 12 are the general registers
    for(int i = 0; i <= REGISTER_COUNT - 4; i++){
        int32_t reg = REG_READ(i);
        printf("%-3i:(0x%08x)\n", i, reg);
    }

    printf("PC: (0x%08x)\n", REG_READ(PC));
    printf("CPSR: (0x%08x)\n", REG_READ(CPSR));
}
  

//////////////////////////EMULATE INSTRUCTION///////////////////////////////////


/* data processing */

void data_processing(int32_t word)
{
	DataProcessingInstr *DPInst = (DataProcessingInstr *) &word;

	int ImmOp    = DPInst->ImmOp;        // 25
	int OpCode   = DPInst->OpCode;   // 24-21
	int SetCond  = DPInst->SetCond;
	int Rn       = DPInst->Rn;
	int Rd       = DPInst->Rd;
	int Operand2 = DPInst->Operand2; // 11-0

	int Operand1 = arm_Ptr->registers[Rn];

	Operand2     = IS_CLEAR(ImmOp) ? as_shifted_reg(Operand2, SetCond)
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
	if(Opcode != TST || Opcode != TEQ || Opcode != CMP) {
   	     REG_WRITE(Rd, result);
        }


	if (IS_SET(SetCond)) {
	// set flags
  	    CPSR_PUT(Z, (result == 0));
            CPSR_PUT(N, BIT_GET(result, 31));
  	
            switch (OpCode)
            {
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


/* multiply */

//Converts number in register
//if 2's complement then negates and add 1, else return;
int32_t convert(int32_t reg) 
{
    int mask = 1 >> 3;
    int32_t result = reg;
    if((mask & (reg >> 3)) == 1) {
        result = convert2complement(reg);
    }
    return result;
}


int32_t convert2complement(int32_t reg) 
{
    int32_t negatedReg = ~reg;
    return negatedReg + 1;
}


void multiply(int32_t word) 
{
    int CPSR = arm_Ptr->registers[CPSR];
    MultiplyInstruct *MultiInst = (MultiplyInstruct *) &word;
    struct CPSR_STRUCT *cpsrStruct;

    int Rd      = MultiInst->Rd;
    int Rn      = MultiInst->Rn;
    int Rs      = MultiInst->Rs;
    int Rm      = MultiInst->Rm;
    int Acc     = MultiInst->Acc;
    int SetCond = MultiInst->SetCond;

    int32_t dataRm = convert(REG_READ(Rm));
    int32_t dataRs = convert(REG_READ(Rs));
    int32_t mulResult = dataRm * dataRs;

    if(IS_SET(A)) {
        int Rn = MultiInst->Rn;
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


/*Single data transfer */

void single_data_transfer(int32_t word) 
{
    SDTInstruct *SDTInst = (SDTInstruct *) &word;

    int dataRn     = SDTInst->Rn;         // base register
    int dataRd     = SDTInst->Rd;         // destination register
    int dataOffset = SDTInst->Offset;
    int dataI      = SDTInst->_I;
    int dataP      = SDTInst->P;
    int dataU      = SDTInst->U;
    int dataL      = SDTInst->L;

	//Check if I is setbranchOffset
  //Pre-indexing
    if (IS_SET(dataP)) {
      dataRn += (IS_SET(dataU)? dataOffset : -dataOffset);

      IS_SET(dataL) ? (word = MEMORY_READ(dataRn)) : MEMORY_WRITE(dataRn, word);
	
  //Post-indexing
    } else {
        if (IS_SET(dataL)) {
          word = read_memory(dataRn);
	} else {
          write_memory(dataRn, word);
	}
        dataRn += (IS_SET(dataU)? dataOffset : -dataOffset);

        
    }
}


/*branch */

void branch(int32_t word)
{    
     int branchOffset = 24;
     int branchShift  =  2;

     BranchInstruct *BranchInst = (BranchInstruct *) &word;

     //offset is between in bits 0-23
     //branchStrc -> Offset = get_bits(word, 0, 23);

     int32_t offsetb = BranchInst-> Offset;

     //offset is shifted left 2 bits
     int32_t offsetNewb = offsetb << branchShift;

     //signed bits extended to 32 bits
     int32_t signed_bits = (offsetNewb >> (branchOffset - 1)) << (SIZE_OF_WORD - 1);

     //add the signed bits to PC
     INC_PC(signed_bits);

     //CHECK ABOUT THE PC AND PIPELINE!!

     //PC = PC + 4;
     INC_PC(4);
}



/////////////////////////MAIN  FUNCTION//////////////////////////////////////


int main(int argc, char **argv)
{
  //int argc mumber of chars
    char errorMsg[] = "No arguments in input!\n";

    if(argc < 6) {
      // not in form of .bin '.','b','i','n','/0';
        printf( "%s", errorMsg);
        printf("Please type in a bin file\n");
        return -1;

    //Read input file and emulate
    read_ARM(argv[1]);
    emulator();

    //Free memory and exit program
    free(arm_Ptr);
    free(arm_Ptr->pipeline);

    return EXIT_SUCCESS;
    }

}
