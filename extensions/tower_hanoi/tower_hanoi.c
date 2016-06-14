#include "tower_struct.h"
#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"

/* The number of rings set by player: 3-6 */
extern int num_of_elem;

/* Initialise the towers */
void initialise(int max_num) {
  /* Initialse towers to be empty */
  for (int i = 0; i < tower_num; i++) {
    for (int j = 0; j < max_num; j++) {
      towers[i][j] = 0;
    }
  }
  /*Only the first tower has ring on it */
  for (int k = 0; k < max_num; k++) {
    towers[0][k] = max_num - k;
  }
}

/* Reset the game: Key R */
void reset(int max_num) {
  initialise(max_num);
}

/* Check if the tower is empty */
int isEmpty(int towerNum) {
  return (towers[towerNum][0] == 0);
}

/* Return the position of the top ring in the tower */
int getTopRingPos (int towerNum) {
  /* Check the tower from top to bottom */
  for (int i = num_of_elem - 1; i >= 0; i-- ) {
    if ((towers[towerNum][i]) != 0) {
      return i;
    }
  }
  return -1;
}

/* Push the ring to specific tower */
void push(int towerNum, int ring) {
  int top_elem_pos = getTopRingPos(towerNum);
  towers[towerNum][top_elem_pos + 1] = ring;
}

/* Find the value of top ring */
int peek(int towerNum) {
  int top_elem_pos = getTopRingPos(towerNum);
  if (top_elem_pos == -1) {  // case of empty tower
    return 0;
  }
  return towers[towerNum][top_elem_pos];
}

/* Find the valus and take the top ring from the tower */
int pop(int towerNum) {
  if(isEmpty(towerNum)) {  // case of empty tower
    return -1;
  }

  int top_elem_pos = getTopRingPos(towerNum);
  int result = towers[towerNum][top_elem_pos];
  /* Clear the top ring */
  towers[towerNum][top_elem_pos] = 0;

  return result;
}

/* Move the top ring from tower to tower */
void move(int from, int to) {

  int from_elem   = pop(from); // The position of ring picked up
  int to_top_elem = peek(to);  // The top ring of the taget tower

  /* Check moving condition */
  if (from_elem == -1) {  // case of picking from empty tower
    perror("The tower you selected is empty.");
  } else if(from_elem < to_top_elem || isEmpty(to)) {  // Ok to move
    push(to, from_elem);
    if(check_win()) {
      printf("Congradulation. You have won!!\n");
    }
  } else {  // not satisfying the condition
    printf("This is an invalid move, please try again...\n");
    /* Push the top ring back to original tower */
    push(from, from_elem);
  }
}

/* Check if the third tower is completed */
int check_win() {
  if(isEmpty(0) && isEmpty(1)) {
    for(int i = 0; i < num_of_elem; i++) {
      if(towers[2][i] != num_of_elem - i) {
        return 0;
      }
    }
    return 1;
  } else {
    return 0;
  }
}
