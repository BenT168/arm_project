#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"

TOKEN *tokenStruct = NULL;

////////////////////////////////////////////////////////////////////////////


void *tok_chk(void *ass)
{
  if (ass  == NULL)
  {
    perror(" INSUFFICIENT MEMORY");
    exit(EXIT_FAILURE);
  }
  return ass;
}

void tokens_free(TOKEN *lines)
{
	for (int i = 0 ; i < lines->tokenCount; i++)
	{
		free(lines->tokens[i]);
	}
	free(lines->tokens);
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
	TOKEN *tokenstruct  = tok_chk(malloc(sizeof(tokenStruct)));
	tokenstruct->tokens   = tok_chk(malloc(0));
	printf("\n");
	printf("after malloc (in tokenise)\n");
  int Tokncount = 0; // initial room for string
	char *token = NULL;
  size_t space = 0;
   // allocate space for tokenst
   // tokens are added to this array;


  tokenstruct->line = strdup(str);

  token = strtok(str, delim); // get the first token
	printf("first tok: %s\n", token);
  while(token != NULL) {
		if (*token == '\0') continue; // Discard empty tokens
		space              	 = sizeof(char *) * (Tokncount + 1);
		tokenstruct->tokens  = tok_chk(realloc(tokenstruct->tokens, space));
		tokenstruct->tokens[Tokncount++] = strdup(token);
		token = strtok(0, delim);
	}
	tokenstruct->tokenCount = Tokncount;

  //free(tokenStruct->line);
//	free(tokenStruct->tokens);
	//free(tokenStruct->line);
	//tokens_free(tokenStruct);
	return tokenstruct;

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
