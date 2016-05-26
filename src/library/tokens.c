#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include <string.h>


TOKENISE_STRUCT *tokenStruct;

TOKENISE_STRUCT tokenise(char *string, const char *delim) {
  char *token;
  int countToken = 0; // initial room for string
  size_t space = 0;
  tokenStruct.tokens = malloc(sizeof(char *) * space);
   // allocate space for tokens
   // tokens are added to this array;

  tokenStruct.line = strdup(string);
  //pointer to new string which is dupliate of string

  token = strtok(string, delim); // get the first token

  while(token != NULL) {
    if(*token != '\0') { // stop when end of string is reached
      space = sizeof(char *) * (countToken + 1);
      tokenStruct.tokens[countToken++] = strdup(token);
      token = strtok(0, delim);
    }
  }
  tokenStruct.tokenCount = countToken;

  free(tokenStruct.tokens);
  return tokenStruct;
}
