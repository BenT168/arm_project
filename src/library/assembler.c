#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "tokens.h"
#include "bitwise.h"

ASSEMBLER_STRUCT* assemble(TOKEN *lines, function_assPtr func, const char *delim) {

  char *line_after_delim = NULL;
  int label_count = 0;
  char **symbol = malloc(sizeof(line_after_delim));

  for(int i = 0; i < lines->tokenCount; i++) {
    char current_Line = malloc(strlen(lines->tokens[i]) + 1);
    current_Line = lines->tokens[i];
    if(!current_Line) {
      perror("Malloc failed.");
    } else {
      TOKEN *token = tokenise(current_Line, delim);
      char *label = line->tokens[0];

      line_after_delim = strchr(label, ':');

      if(line_after_delim != NULL) {
        //have a label
        label_count++;
        symbol[i] = delstr(line_after_delim);
         // removes ':' from line_after_delim
      }
    }
    free(current_Line);
    tokens_free(token);
  }

  free(symbol);

  //initialise assemble
  int line_total = lines->tokenCount - label_count;
  *ass = malloc(sizeof(ass));
  ass->TOTAL_line = line_total;
  ass->symbolTable = symbol;
  if(line_after_delim != NULL) {
    ass->instr = NULL;
    ass->current_address = 0;
    realloc(ass->instr, sizeof(binary_instruct) * (ass->TOTAL_line));
  }

  for(int i = 0; i < lines->tokenCount; i++) {
    char current_Line = malloc(strlen(lines->tokens[i]) + 1);
    current_Line = lines->tokens[i];
    if(!current_Line) {
      perror("Malloc failed.");
    } else {
    TOKEN *token = tokenise(current_Line, delim);
    char *label = token->tokens[0];
    line_after_delim = strchr(label, ':');
    int32_t word = func(token);
    assemble_write(ass, word);
   }
   free(current_Line);
   tokens_free(token);
  }
  return ass;
}



void assemble_free(ASSEMBLER_STRUCT *ass) {
  for(int i = 0; i < ass->TOTAL_line; i++) {
    free(ass->instr[i]);
  }
  free(ass->instr);

  for(int i = 0; i < sizeof(sizeof(ass->symbolTable)); i++) {
    free(ass->symbolTable[i]);
  }
  free(ass->symbolTable);
  free(ass);
}


void assemble_write(ASSEMBLER_STRUCT *ass, int32_t word) {
  printf("%080x", p->current_address);
  print_bits(word);

  binary_instruct *instr = malloc(sizeof(binary_instruct));
  instr->binary_word = word;
  instr->word_address = ass->current_address;
  int current_instruct = ass->current_address / sizeof(int32_t);
  p->instr[current_instruct] = instr;
  ass->current_address += sizeof(int32_t);
}


void assemble_print(ASSEMBLER_STRUCT *ass) {
  for(int i = 0; i < ass->TOTAL_line; i++) {
    uint8_t first = ass->instr[i]->word_address;
    int32_t word = ass->instr[i]->binary_word;
    printf("%07x", first);
    print_bits_inBE(word);
  }
}


int32_t* assemble_generate_bin(ASSEMBLER_STRUCT *ass) {
  int32_t* words = (int32_t*) malloc(sizeof(int32_t) * ass->TOTAL_line);
  for(int i = 0; i < ass->TOTAL_line; i++) {
    words[i] = ass->instr[i]->binary_word;
  }
  return words;
}

char* delstr(char *str) {
  char* result[strlen(str)];
  strcpy(result, str);
 	str++;
  return result;
}
