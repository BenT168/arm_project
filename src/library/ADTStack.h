#ifndef STACK_H
#define STACK_H


#include <stdio.h>
#include <stdlib.h>


typedef struct stack_item {
  char element;
  struct stack_item *next;
} stack_item;

typedef stack_item *stack;

stack initialise(void);
int isEmpty(stack S);
char top_elem (stack S);
stack push_on_stack(stack S, char c);
stack pop_stack(stack S);
void print_stack(stack S);


#endif
