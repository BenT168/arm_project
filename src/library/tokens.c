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

	/*tokenStruct = malloc(sizeof(TOKEN));
	printf("c1\n");
  char *token;
  int countToken = 0; // initial room for string
  size_t space = 0;

  printf("c2\n");
   tokenStruct->tokens = (char**) malloc(sizeof(char *) * space);
	printf("c3\n");
   // allocate space for tokens
   // tokens are added to this array;
  if(!(tokenStruct->tokens)) {
     perror("Malloc failed for tokens."); //check if allocated properly
  }

   tokenStruct->tokens[0] = (char*) malloc(sizeof(char) * space * countToken);
	 printf("c4\n");
   //allocate first part of tokens
   if(!(tokenStruct->tokens[0])) {
    perror("Malloc failed for token[0]."); // check if allocated properly
  }

  //tokenStruct->line = strdup(str);
	tokenStruct->line = (char*)malloc(sizeof(char) * MAX_CHAR_inLINE);
	printf("c4a\n");

	if(!(tokenStruct->line)) {
  perror("Malloc failed for line."); //check if allocated properly
  }
  tokenStruct->line = str;
	printf("c5\n");
  //pointer to new string which is dupliate of string
	printf("c14\n");

  token = strtok(str, delim); // get the first token
	printf("c6\n");

  while(token != NULL) {
		printf("c7\n");
    if(*token != '\0') { // stop when end of string is reached
    //  space = sizeof(char *) * (countToken + 1);
		printf("c8\n");
      tokenStruct->tokens[countToken++] = token;
			printf("c9\n");
      token = strtok(0, delim);
			printf("c10\n");

    }
  }


  tokenStruct->tokenCount = countToken;
	printf("c11\n");


  free(tokenStruct->tokens);
  tokens_free(tokenStruct);

	printf("c12\n");

	//free(tokenStruct->line);
	//free(tokenStruct);
	printf("c13\n");

  return tokenStruct;*/


	tokenStruct  = malloc(sizeof(tokenStruct));
	tokenStruct->tokens   = malloc(0);
	printf("\n");
	printf("after malloc (in tokenise)\n");
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

  printf("str lenght: %zu\n",strlen(str));
//	size_t len = strlen(str) + 1;
//  tokenStruct->line = malloc(len);
//  memmove(tokenStruct->line, str, len);

//	(tokenStruct->line) = malloc(strlen(str) + 1)
//  strcpy(tokenStruct->line, str);
  tok = strtok(str, delim); // get the first token
	printf("first tok: %s\n", tok);
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
