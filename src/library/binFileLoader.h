#ifndef _BIN_FILE_LOADER
#define _BIN_FILE_LOADER

#include <stdio.h>

FILE *file;

void read_ARM(FILE *file);
struct ARM_State *arm_Ptr;

#endif
