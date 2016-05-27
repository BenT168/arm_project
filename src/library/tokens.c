#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include <string.h>


TOKENISE_STRUCT *tokenStruct = NULL;

TOKENISE_STRUCT tokenise(char *str, const char *delim) {
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
