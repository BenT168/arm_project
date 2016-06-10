#include <stdio.h>
#include <stdlib.h>

#include "ADTstack.h"


// implementing a dynamic stack
// There is a topNode at the beginning of the list
// It's underlying data structure is a linked list

stack initialise(void){
  stack S = (stack_item *)malloc(sizeof(stack_item));
  S-> next = NULL;
  return S;
}

int isEmpty(stack S){
  return (S->next == NULL);
}

int32_t top_elem(stack S){
  if(isEmpty(S)){
    perror("The stack is empty, cannot find top elem");
    exit(EXIT_FAILURE);
  }
  return S->next->element;
}

stack push_on_stack(stack S, int32_t c){
  stack_item *top_node;

  //copy the new elem into the top node
  S->element = c;

  //create a new topNode
  top_node = (stack_item *)malloc (sizeof(stack_item));
  top_node -> next = S;
  return top_node;

}


stack pop_stack(stack S){
  if(isEmpty(S)){
    perror("the stack is empty, cannot pop");
    return S;
  }
  S-> next -> element = '\0';
  return S-> next;
}

void print_stack(stack S){
  stack_item *top_node;

  top_node = S -> next;
  while(top_node != NULL){
    printf("stacks: %c\n", top_node->element);
    top_node = top_node->next;
  }
}
