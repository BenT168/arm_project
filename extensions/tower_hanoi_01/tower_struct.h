#ifndef tower_stuct_h
#define tower_struct_h


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "SDL/SDL.h"

//There are three stacks and maximum 8 elements in each stack
int towers[3][6];

int currentTower;
int currentElem;

#define Max_elem 6
#define tower_num 3


typedef struct stack_item {
  SDL_Rect num[Max_elem];
  SDL_Rect position;
  uint8_t colour;
  int top[tower_num]; //there are three stacks
  //struct stack_item *next;
} stack_item_t;

//typedef stack_item_t *Rod;



void initialise(int);
void reset(int);
int isEmpty(int);
void push(int, int);
int peek(int);
int pop(int);
void move(int, int);
void print_stack(void);
int check_win();



#endif
