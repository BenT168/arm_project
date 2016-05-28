#ifndef _TOKEN
#define _TOKEN

#include <stdio.h>
#include <stdlib.h>


typedef struct TOKEN {
  char **tokens;
  unsigned int tokenCount;
  char *line;
} TOKEN;

TOKEN tokenise(char* , const char*);

#endif
