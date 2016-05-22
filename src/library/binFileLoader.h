#ifndef _BIN_FILE_LOADER
#define _BIN_FILE_LOADER

#include <stdio.h>

FILE *file;

void read_ARM(const char *binFile);
struct ARM_State *arm_Ptr;

#endif
