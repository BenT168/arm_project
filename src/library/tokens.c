#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"
#include <inttypes.h>

TOKEN *token_struct = NULL;

////////////////////////////////////////////////////////////////////////////

void *tokens_chk(void *ass)
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
	for (int i = 0; i < lines->tokenCount; i++)
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
	token_struct  = tokens_chk(malloc(sizeof(token_struct)));
	token_struct ->tokens   = tokens_chk(malloc(0));

  int Tokncount = 0; // initial room for string
	char *token = NULL;
  size_t space = 0;
   // allocate space for tokenst
   // tokens are added to this array;

  token_struct->line = strdup(str);

  token = strtok(str, delim); // get the first token

  while(token != NULL) {
		if (*token == '\0') continue; // Discard empty tokens
		space              	 = sizeof(char *) * (Tokncount + 1);
		token_struct->tokens  = tokens_chk(realloc(token_struct->tokens, space));
		token_struct->tokens[Tokncount++] = strdup(token);
		token = strtok(0, delim);
  }

	token_struct->tokenCount = Tokncount;

	//free(tokenStruct->tokens);
	//free(tokenStruct->line);
	//tokens_free(tokenStruct);
	return token_struct;

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
