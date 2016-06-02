#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"

TOKEN *tokenStruct = NULL;

////////////////////////////////////////////////////////////////////////////


void tokens_free(TOKEN *lines)
{
	for (int i = 0 ; i < lines->tokenCount; i++)
	{
		free(lines->tokens[i]);
	}
	//free(lines->tokens);
  free(lines->line);
	free(lines);
}

void tokens_print(TOKEN *lines)
{
	printf("Printing %d tokens:\n", lines->tokenCount);
	for (int i = 0; i < lines->tokenCount; i++)
	{
		printf("%s\n", lines->tokens[i]);
	}
}

TOKEN* tokenise(char *str, const char *delim)
{
	tokenStruct  = malloc(sizeof(tokenStruct));
	tokenStruct->tokens   = malloc(0);
	//printf("\n");
	//printf("after malloc (in tokenise)\n");
  int Tokncount = 0; // initial room for string
	char *tok = NULL;
  size_t space = 0;
   // allocate space for tokenst
   // tokens are added to this array;
	 if ( tokenStruct == NULL || tokenStruct->tokens == NULL)
 	{
 		perror("Malloc failed for tokens! INSUFFICIENT MEMORY");
 		exit(EXIT_FAILURE);
 	}

  tokenStruct->line = strdup(str);

  tok = strtok(str, delim); // get the first token
	//printf("first tok: %s\n", tok);
  while(tok != NULL) {
		if (*tok == '\0') continue; // Discard empty tokens
		space              	 = sizeof(char *) * (Tokncount + 1);
		tokenStruct->tokens  = realloc(tokenStruct->tokens, space);
		if ((tokenStruct->tokens)  == NULL)
  	{
  		perror("Malloc failed for tokens! INSUFFICIENT MEMORY");
  		exit(EXIT_FAILURE);
  	}
		tokenStruct->tokens[Tokncount++] = strdup(tok);
		tok = strtok(0, delim);
	}
	tokenStruct->tokenCount = Tokncount;

  //free(tokenStruct->line);
//	free(tokenStruct->tokens);
	//free(tokenStruct->line);
	//tokens_free(tokenStruct);
	return tokenStruct;

}

void tokens_iter(TOKEN *lines, tokens_func func)
{
	for (int i = 0; i < lines->tokenCount; i++)
	{
		func(lines->tokens[i]);
	}
}

char tokens_endc(TOKEN *lines)
{
	return lines->tokens[lines->tokenCount-1]
		[strlen(lines->tokens[lines->tokenCount-1])-1];
}
