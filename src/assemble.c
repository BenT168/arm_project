
#include <stdlib.h>
#include <stdio.h>

///////////////////////////two-pass assembly////////////////////////////////////

/////// first pass//////////////////////////////////////////////////////////////

char *buffer;
ASSEMBLER_STRUCT *ass;

TOKENISE_STRUCT read_Source();
void write_File();

TOKENISE_STRUCT read_Source(const char *sourceFile) {
  FILE *file = fopen(sourceFile, "rs");

  if (file != NULL) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file); //Size of file in bytes
    fseek(file, 0, SEEK_SET); //go back to start

    buffer = (char *) malloc(size * sizeof(char));
    if(buffer == NULL) {
      perror("Malloc failed.");
    } else {
    //const char s = ",";
    //char *token;
    //token = strtok(buffer, s);
    fread(buffer, size, 1, sourceFile);

    int sizeBuffer = sizeof(buffer);

    buffer[sizeBuffer - 1]= '\0';

    return tokenise(buffer, ",");


    //while(token != NULL) {
      //}
  //  tokenise(buffer, )
  //  assembler();
  }

  free(buffer);

} else {
  perror("Error opening the file.");
}

}

void write_File(const char *binaryFile) {
  FILE *file = fopen(binaryFile, "wb"); //w = write b = binary

  int32_t *program = assembler(ass); //get code from assembler program

  int size = ass.TOTAL_line * sizeof(int32_t);
  //size of each element that will be written

  fwrite(program, size, 1, binaryFile);

  fclose(binaryFile);
}


int main(int argc, char **argv) {

  if(argc < 2) { // Need two files
    printf("Incomplete number of arguments in input!\n");
    printf("Please type in as first argument : ARM source file\n");
    printf("And as aecond argument : an output ARM binary code file\n");
    exit(EXIT_FAILURE);
  }


  TOKENISE_STRUCT *lines = read_Source(argv[1]); // get each line of source code as tokens

  ass = assembler(lines); //assemble lines and get output to write to file

  write_File(argv[2]); //

  free(tokenStruct.lines);

  return EXIT_SUCCESS;
}
