#ifndef STACK_H
#define STACK_H


#include <stdio.h>
#include <stdlib.h>

typedef struct single_reg{
  char *registers;
  int32_t address;
} single_reg;

typedef struct stack_item {
  single_reg *single_reg;
  struct stack_item *next;
} stack_item;

typedef stack_item *stack;

stack initialise(void);
int isEmpty(stack S);
int32_t top_elem (stack S);
stack push_on_stack(stack S, int32_t c);
stack pop_stack(stack S);
void print_stack(stack S);


#endif
