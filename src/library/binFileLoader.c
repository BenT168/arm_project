#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "binFileLoader.h"
#include "arm11.h"


void read_ARM(const char *binFile) {
    file = fopen(binFile, "rb"); //open the file r = read , b = binary

    if (file != NULL) {
      fseek(file, 0, SEEK_END);
      long size = ftell(file); //Size of file in bytes
      fseek(file, 0, SEEK_SET); //go back to start

      //Allocate memory
      arm_Ptr = malloc(size+1);
      if(!arm_Ptr) {
        char errorMsg[] = "Memory Error!";
        fprintf(stderr, "%s\n", errorMsg);
        fclose(file);
        return;
      }

     //Read instructions into ARM Memory
  //   for(int i = 0; i < MEMORY_CAPACITY; i++) {
      size_t newLen = fread(arm_Ptr->memory, size, 1, file);
      if (newLen == 0) {
        fputs("Error reading file", stderr);
      } else {
       fclose(file);
     }

      free(arm_Ptr->memory);
    } else {
      perror("Error opening file");
    }
}
