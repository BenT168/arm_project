#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>


typedef struct TOKENISE_STRUCT {
  char **tokens;
  unsigned int tokenCount;
  char *line;
} TOKENISE_STRUCT;

TOKENISE_STRUCT tokenise(char* , const char*);

#endif
