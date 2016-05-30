#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H

#include <stdint.h>
#include "tokens.h"
#include "symbolTableADT.h"


typedef struct binary_instruct
{
  int32_t binary_word;
  uint16_t word_address;
} binary_instruct;


typedef struct ASSEMBLER_STRUCT
{
  binary_instruct **instr; // Binary Instruction Array
  map *symbolTable; //array of symbol table
  int TOTAL_line;   // total of the line
  uint16_t current_address; //address of the current instruction
} ASSEMBLER_STRUCT;


//Function that takes token and assembler and returns an int
typedef int32_t (*function_assPtr)(TOKEN *, ASSEMBLER_STRUCT *);

ASSEMBLER_STRUCT* assemble(TOKEN *, function_assPtr, const char *);

void assemble_free(ASSEMBLER_STRUCT *);

void assemble_write(ASSEMBLER_STRUCT *, int32_t);

void assemble_print(ASSEMBLER_STRUCT *);

int32_t* assemble_generate_bin(ASSEMBLER_STRUCT *);

char* delstr(char *str);

#endif
