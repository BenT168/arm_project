#ifndef token
#define token

#include <stdio.h>
#include <stdlib.h>

typedef struct TOKENISE_STRUCT {
  char **tokens;
  unsigned int tokenCount;
  char *line;
} TOKENISE_STRUCT;

#endif
