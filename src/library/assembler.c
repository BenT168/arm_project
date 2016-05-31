#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "tokens.h"
#include "bitwise.h"
#include "symbolTableADT.h"
//#include "hashmap.h"


extern ASSEMBLER_STRUCT *ass;

//ASSEMBLER_STRUCT* assemble(TOKEN *lines, function_assPtr func, const char *delim)
//{
  //char* line_after_delim = NULL;
  //=(char*)malloc(sizeof(char) * MAX_CHAR_inLINE);
//  int label_count = 0;
  //int size_of_string = 0;
  //char **symbol = (char**)malloc(sizeof(char*)*sizeof(line_after_delim));
  //for(int i = 0; i < 10; i++) {
    //symbol[i] = malloc(sizeof(char)*sizeof(line_after_delim));
  //}
//  TOKEN *token = (TOKEN*)malloc(sizeof(TOKEN));
//  char* current_Line = (char*) malloc(sizeof(char) * lines->tokenCount);

//  printf("before for loop\n");
//  for(int i = 0; i < lines->tokenCount; i++) {
//    printf("d1 \n");

//    char* current_Line = (char*) malloc(sizeof(char) * lines->tokenCount);
//    printf("d2 \n");

  //  if(!current_Line) {
    //  perror("Malloc failed.");
    //} else {
  /*    printf("d3 \n");
      current_Line = lines->tokens[i];
      printf("d4 \n");
      token = tokenise(current_Line, delim);
      tokens_print(token);
      printf("d5 \n");
      char *label = (char *) malloc(sizeof(char) * MAX_CHAR_inLINE);
      label = lines->tokens[0];
      printf("%s\n", label);
      const char dLm = ':';
      printf("d6 \n");
      printf("%ld\n", sizeof(label));
    //  if(sizeof(label))


      char *test = ":from";
      char *res = delstr(test);
      printf("%s\n", res);
      char *l = strchr(label, dLm);
      printf("after l\n");
      //line_after_delim = strdup(l);
      label_count++;

      char **symbol = (char**)malloc(sizeof(char*) * sizeof(l));
      printf("after initialising symbol\n");
      for(int i = 0; i < 10; i++) {
        symbol[i] = malloc(sizeof(char)*sizeof(l));
      }

    //  for(int i=0; i < )
      symbol[i] = delstr(l);

      printf("after symbol\n");
      //(char*)malloc(sizeof(char) * MAX_CHAR_inLINE);
      printf("d6a \n"); */
    /*  if(!line_after_delim) {
        perror("Malloc failed");
      }
      printf("d6b \n");

      char* str;
      char* l = "hello:from";

      str = strchr(l, dLm);

      printf("%s\n", str);

      line_after_delim = strchr(label, dLm);

      printf("d7 \n");
      if(line_after_delim != NULL) {
        printf("d8 \n");
        //have a label
        label_count++;
        symbol[i] = delstr(line_after_delim);
         // removes ':' from line_after_delim
      } */
      //free(line_after_delim);
      //free(label);
  //  }
  //free(current_Line);
  //free(line_after_delim);
/*  for(int i = 0; i < 10; i++) {
    free(symbol[i]);
  }
  free(symbol);
  }

  //free(line_after_delim);

  //free(current_Line);
  printf("after for loop\n"); */

  //tokens_free(token);
/*  for(int i = 0; i < 10; i++) {
    free(symbol[i]);
  }
  free(symbol);*/

  //initialise assemble
  /*int line_total = lines->tokenCount - label_count;
  ass = malloc(sizeof(ASSEMBLER_STRUCT));
  ass->TOTAL_line = line_total;
  ass->symbolTable = symbol;
  if(line_after_delim != NULL) {
    ass->instr = malloc(sizeof(binary_instruct) * (ass->TOTAL_line));
    ass->current_address = 0;
    //realloc(ass->instr, sizeof(binary_instruct) * (ass->TOTAL_line));
  }

  for(int i = 0; i < lines->tokenCount; i++) {
    char* current_Line = malloc(strlen(lines->tokens[i]) + 1);
    current_Line = lines->tokens[i];
    if(!current_Line) {
      perror("Malloc failed.");
    } else {
    token = tokenise(current_Line, delim);
    char *label = token->tokens[0];
    line_after_delim = strchr(label, ':');
    int32_t word = func(token);
    assemble_write(ass, word);
   }
   free(current_Line);
   tokens_free(token);
 }*/
//  return ass;
//}
/*
char *delstr(char *str)
{
  char *result = strdup(str);
  str++;
  strcpy(result, str);
  return result;
}
*/

void assemble_free(ASSEMBLER_STRUCT *ass)
{
  for(int i = 0; i < ass->TOTAL_line; i++)
  {
    free(ass->instr[i]);
  }

  free(ass->instr);
//  for(int i = 0; i < sizeof(sizeof(ass->symbolTable)); i++)
  //{
    //map_free(ass->symbolTable[i]);
  //}
  //map_free(ass->symbolTable, MAP_FREE_VAL | MAP_FREE_KEY);
  free(ass);
}

static void realloc_instrs(ASSEMBLER_STRUCT *ass)
{
  ass->instr = realloc(ass->instr, sizeof(binary_instruct *) * (ass->TOTAL_line));
}

uint16_t assemble_append(ASSEMBLER_STRUCT *ass, int32_t word)
{
        ass->TOTAL_line++;
	realloc_instrs(ass);

	uint16_t address = (ass->TOTAL_line-1) * sizeof(int32_t);
	binary_instruct *instr = malloc(sizeof(binary_instruct));
	instr->binary_word  = word;
	instr->word_address = address;

	int current_instruct = address / sizeof(int32_t);
	ass->instr[current_instruct] = instr;

	return address;
}


void assemble_write(ASSEMBLER_STRUCT *ass, int32_t word)
{
  printf("%080x\n", ass->current_address);
  print_bits_inBE(word);

  binary_instruct *instr = malloc(sizeof(binary_instruct));
  instr->binary_word = word;
  printf("%i\n", instr->binary_word);
  instr->word_address = ass->current_address;
  int current_instruct = ass->current_address / sizeof(int32_t);
  ass->instr[current_instruct] = instr;
  ass->current_address += sizeof(int32_t);
}


void assemble_print(ASSEMBLER_STRUCT *ass)
{
  for(int i = 0; i < ass->TOTAL_line; i++)
  {
    uint8_t first = ass->instr[i]->word_address;
    int32_t word = ass->instr[i]->binary_word;
    printf("%07x", first);
    print_bits_inBE(word);
  }
}


int32_t *assemble_generate_bin(ASSEMBLER_STRUCT *ass)
{
  printf("pls generate bin\n");
  printf("printing the assemble.....\n");
  //assemble_print(ass);//TODO:delete
  int32_t *words = malloc(sizeof(int32_t) * (ass->TOTAL_line));
  printf("before for loop in generate_bin\n");
  for(int i = 0; i < (ass->TOTAL_line); i++)
  {
    printf("%i\n", words[ass->TOTAL_line] );
    printf("ass->instr[i]->binary_word %c\n", (ass->instr[i])->binary_word);
    words[i] = (ass->instr[i])->binary_word;
    printf("words[i] after writing....%i\n",words[i]);

  }
  return words;
}

char *strdelchr(char *buffer, char chr)
{
    char *str = strdup(buffer);
    char* c;

	if ((c = index(str, chr)) != NULL)
	{
	    //size_t left = sizeof(str) - (c + 1 - str);
	    strcpy(c, c+1);//memmove(c, c + 1, left);
	}

	return str;
}

uint16_t *heap_uint16_t(uint16_t i)
{
	uint16_t *pt = malloc(sizeof(uint16_t));
	*pt = i;
	return pt;
}


ASSEMBLER_STRUCT *assemble(TOKEN *lines,  function_assPtr func, const char *delim)
{
  printf("Hello assemble\n");
  // Pass #1
	map   *symtbl  = map_new(&map_cmp_str);
	uint16_t  address = 0;
	int       labelc  = 0;
  printf("before inner for loop in assemble\n");
	for (int i = 0; i < lines->tokenCount; i++)
	{
		char   *currl = strdup(lines->tokens[i]);
		TOKEN *line  = tokenise(currl, delim);
		char   *label = line->tokens[0];
    printf("before if in assemble\n");
		if (strchr(label, ':')) // Label encountered
		{
			labelc++;
			map_put(symtbl, strdelchr(label, ':'), heap_uint16_t(address));
            tokens_free(line);
			continue;
		}
    printf("right after if in assemble\n");
		address += sizeof(int32_t);
		tokens_free(line);
	}
printf("after for loop in assemble\n");
  // Initialize Assembly Program
	int line_tot      = lines->tokenCount - labelc;
	ASSEMBLER_STRUCT *p  = malloc(sizeof(ASSEMBLER_STRUCT));
	p->instr         = malloc(sizeof(0));
	p->TOTAL_line       = line_tot;
	p->symbolTable        = symtbl;
	p->current_address      = 0;
	//realloc_instrs(p);

	// Pass #2
	for (int i = 0; i < lines->tokenCount; i++)
	{
		char  *currl = strdup(lines->tokens[i]);
		TOKEN *line = tokenise(currl, delim);
		char   *mnem = line->tokens[0];
		if (strchr(mnem, ':')) continue; // Label encountered

		int32_t word = func(line, p);
    printf("word: %i\n", word );

		assemble_write(p, word);
    printf("can i write in assemle????\n");

  //  if(strcmp(line->tokens[0], "mla") != 0) {
      tokens_free(line);
//    }

  //    if (i != (lines->tokenCount - 1))
  //    {
//      }

	}// Assembling done
  printf("assemble donw\n");
	return p;
}
