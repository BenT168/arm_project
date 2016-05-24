#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void getLine();
/*Getline function */
void processLine();
/*Process line function*/
void define();
/*Define Function*/
void expand();
/*Expand the opcode function*/
FILE *output;
/*Output file */
FILE *input;
/*Input file containing program */

char cond[4],null[2],immOperad[1],opcode[4],setcond[1],rn[4],rd[4],operand[12];
char line[32];
int namcount=0, defcount=0;
int EXPANDING;
int curr;

struct codechamp {
  char name[10];
  int start,end;
}

mycodechamp[15];

struct definetab {
 char macroline[25];
}

mydefinetab[25];

struct argumenttab {
 char arg[3][9];
}

myargumenttab;

/* GETLINE FUNCTION */
void getLine() {
  char word1[10],word2[10],word3[10],buff[10];
  int count=0,i,j=0;

  if (EXPANDING) {
    strcpy(line,mydefinetab[curr++].macroline);
  }else{
    fgets(line,20,input);
  }
  opcode[0]='\0';
  label[0]='\0';
  operand[0]='\0';
  word1[0]='\0';
  word2[0]='\0';
  word3[0]='\0';

  for (i=0; line[i]!= '\0'; i++){
    if(line[i] != ' ') {
      buff[j++] = line[i];
    } else {
      buff[j]='\0';
      strcpy(word3, word2);
      strcpy(word2, word1);
      strcpy(word1, buff);

      j=0;

     count++;

    }
  }

  buff[j-1] = '\0';
  strcpy(word3, word2);
  strcpy(word2, word1);
  strcpy(word1, buff);

  switch (count) {
    case 0:
      strcpy(opcode, word1);
    break;
    case 1: {
      strcpy(opcode, word2);
      strcpy(operand, word1);
    } break;
    case 2: {
      strcpy(label, word3);
      strcpy(opcode, word2);
      strcpy(operand, word1);}
    break;
   }
}

/* PROCESSLINE Function */
void processLine() {

  int i;

  for (i=0;i<namcount;i++) {
    if (!strcmp(opcode, mycodechamp[i].name)){
      expand();
      return;
    }
    if (!strcmp(opcode, "MACRO"))
      define();
    } else {
      fprintf(output, "%s", line);
    }
}

/* Define Fucntion */

void define() {

  int LEVEL, i=0, j=0, k=0;
  char param[5][9];
  char s[3];

  strcpy(s, "123");
  strcpy( mycodechamp[namcount].name, label );
  mycodechamp[namcount].start = defcount;
  strcpy(mydefinetab[defcount].macroline, line);

  while (operand[i] != '\0') {
    if (operand[i] != ',') {
      param[j][k++] = operand[i];
    } else {
      param[j++][k] = '\0';
      k=0;
    }
  i++;
  }

  param[j][k] = '\0';
  LEVEL=1;

  while ( LEVEL>0 ) {
    getLine();
    if ( operand[0] != '\0' ) {
       for( i = 0 ; i < 3 ; ++i ) {
         if ( !strcmp(operand, param[i])) {
            operand[0] = '?';
            operand[1] = s[i];
            operand[2] = '\0';

         }
      }
    }
    if (!strcmp ( opcode, "MACRO" )) {
       LEVEL++;
    } else if ( !strcmp (opcode, "MEND")) {
       LEVEL--;
       strcpy ( mydefinetab[defcount].macroline, opcode );
       if ( operand[0] != '\0') {
         strcat( mydefinetab[defcount].macroline, " " );
         strcat( mydefinetab[defcount].macroline, operand );
         strcat (mydefinetab[defcount].macroline, "\n" );
       }
       strcat(mydefinetab[defcount++].macroline, "\n" );
    }
  }
  mycodechamp[namcount++].end=defcount;
}

/*Expand Function */
void expand(){

  int i, end=0, j=0, k=0;
  EXPANDING=1;

  int arg=0;

  fprintf( output, "//%s", line );

  for ( i = 0 ; i < namcount; i++ ) {
    if ( !strcmp(opcode, mycodechamp[i].name) ){
      curr = mycodechamp[i].start;
      end  = mycodechamp[i].end;
      while ( operand[i] != '\0'){
        if ( operand[i] != ',' ) {
          myargumenttab.arg[j][k++]=operand[i];
        } else {
          myargumenttab.arg[j++][k] = '\n';
          k=0;
        }
      }
      i++;
    }
    myargumenttab.arg[j][k] = '\n';
  }

  while ( curr < (end-1) ) {
    getLine();
    if ( operand[0] == '?') {
      strcpy (operand, myargumenttab.arg[operand[1]-'0'-1]);
      fprintf ( output, "%s %s %s", label, opcode, operand);
    }
  }
  EXPANDING=0;
}


/*Main function*/
int main(int argc, char **argv) {
  EXPANDING=0;
  input = fopen ( "input.txt", "r" );
  output= fopen ( "utput.txt", "w" );

  getLine();
  while ( strcmp ( opcode, "END" ) != 0 ) {

  processLine();
  getLine();
  }
  fprintf( output, "%s" , line );

  printf ( "\nExpaned output file has been successfully created.\n");
  system ( "pause" );

  getch();

  return 1;
}
