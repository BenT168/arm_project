#include "tower_struct.h"
#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"
//#include "show.h"


stack_item_t stack;

extern int num_of_elem;

void initialise(int max_num){

  for (int i = 0; i < tower_num; i++){
    for (int j = 0; j < max_num; j++){
      towers[i][j] = 0;
    }
  }

  for (int k = 0; k < max_num; k++) {
    towers[0][k] = max_num - k;
  }
}

void reset(int max_num){
  initialise(max_num);
}


int isEmpty(int towerNum){
  return (towers[towerNum][0] == 0);
}

/* Return the position of the top ring in the tower */
int getTopRing (int towerNum) {
  for (int i = num_of_elem - 1; i >= 0; i-- ) {
    if ((towers[towerNum][i]) != 0) {
      return i;
    }
  }
  return -1;
}

void push(int towerNum, int item){
  int top_elem = getTopRing(towerNum);

  towers[towerNum][top_elem + 1] = item;
}

/* Find the top ring value*/
int peek(int towerNum){
  int top_elem = getTopRing(towerNum);
  if (top_elem == -1) {
    return 0;
  }
  //printf("the top to ring is %i\n", top_elem);
  return towers[towerNum][top_elem];
}

/* Take the top ring from the tower */
int pop(int towerNum){

  if(isEmpty(towerNum)){
//perror("The tower you selected is empty.");
    return -1;
  }

  int top_elem = getTopRing(towerNum);
  //printf("the top from ring is %i\n", top_elem);
  int result = towers[towerNum][top_elem];
  towers[towerNum][top_elem] = 0;

  return result;
}

/* Move the top ring from tower to tower */
void move(int from, int to){
  int from_elem = pop(from);
  int to_top_elem = peek(to);
  //printf("from ring is %i in %i\n", from_elem, from);
  //printf("to ring is %i in %i\n", to_top_elem, to);

  /* Check moving condition */
   if (from_elem == -1) {
     perror("The tower you selected is empty.");
  } else if(from_elem < to_top_elem || isEmpty(to)){
    printf("going to move rings\n");
    push(to, from_elem);
    if(check_win()){
      printf("Congradulation. You have won!!\n");
      //drawScreen();
    }
  } else {
    printf("This is an invalid move, please try again...\n");
    push(from, from_elem);  // push the top ring back
  }
  //return 0;
}

void print_stack(void){

  for(int i = 0; i < 3; i++){
    printf("tower %i:  \n", i + 1);
    for(int j = 0; j < Max_elem; j++){
      printf("%i\t", towers[i][j]);
      printf("\n");
    }
  }

}


int check_win(){
//return 1 if have won
  if(isEmpty(0) && isEmpty(1)){
    for(int i = 0; i < num_of_elem; i++){
      if(towers[2][i] != num_of_elem - i) {
        return 0;
      }
    }
    return 1;
  } else {
    return 0;
  }
}
