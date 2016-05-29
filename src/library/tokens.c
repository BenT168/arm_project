#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"

TOKEN *tokenStruct = NULL;

////////////////////////////////////////////////////////////////////////////

void tokens_free(TOKEN *tokens)
{
	for (int i = 0 ; i < tokens->tokenCount; i++)
	{
		free(tokens->tokens[i]);
	}
	free(tokens->tokens);
	free(tokens->line);
	free(tokens);
}

void tokens_print(TOKEN *tokens)
{
	printf("Printing %d tokens:\n", tokens->tokenCount);
	for (int i = 0; i < tokens->tokenCount; i++)
	{
		printf("%s\n", tokens->tokens[i]);
	}
}

TOKEN tokenise(char *str, const char *delim)
{
  char *token;
  int countToken = 0; // initial room for string
  size_t space = 0;

  (*tokenStruct).tokens = (char**) malloc(sizeof(char *) * space);
   // allocate space for tokens
   // tokens are added to this array;
   if(!((*tokenStruct).tokens)) {
     perror("Malloc failed for tokens."); //check if allocated properly
   }

   (*tokenStruct).tokens[0] = (char*) malloc(sizeof(char) * space * countToken);
   //alloca first part of tokens
   if((*tokenStruct).tokens[0]) {
     perror("Malloc failed for token[0]."); // check if allocated properly
   }

  (*tokenStruct).line = (char*) malloc(sizeof(char) * strlen((*tokenStruct).line));
  if(!((*tokenStruct).line)) {
  perror("Malloc failed for line."); //check if allocated properly
}

  //(*tokenStruct).line = strdup(str);
  (*tokenStruct).line = str;
  //pointer to new string which is dupliate of string

  token = strtok(str, delim); // get the first token

  while(token != NULL) {
    if(*token != '\0') { // stop when end of string is reached
      space = sizeof(char *) * (countToken + 1);
      (*tokenStruct).tokens[countToken++] = token;
      token = strtok(0, delim);
    }
  }
  (*tokenStruct).tokenCount = countToken;

  free((*tokenStruct).tokens);
  return *tokenStruct;
}

void tokens_iter(TOKEN *tokens, tokens_func func)
{
	for (int i = 0; i < tokens->tokenCount; i++)
	{
		func(tokens->tokens[i]);
	}
}

char tokens_endc(TOKEN *lines)
{
	return lines->tokens[lines->tokenCount-1]
		[strlen(lines->tokens[lines->tokenCount-1])-1];
}

char string_endc(char* str) {
	return str[srlen - 1]; 
}
