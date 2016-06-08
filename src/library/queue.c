#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

Queue* createQueue() {
  Queue* Q = (Queue*)malloc(sizeof(Queue));
  if(!Q) {
    perror("Malloc failed: Queue");
    exit(EXIT_FAILURE);
  }
  Q->elements = (char **)malloc(sizeof(char*)*MAX_CAPACITY);
  if(!Q->elements) {
    perror("Malloc failed: Queue->elements");
    exit(EXIT_FAILURE);
  }

  for(int i = 0; i < MAX_CAPACITY; i++) {
    Q->elements[i] = (char*)malloc(sizeof(char));
  }

  Q->size = 0;
  Q->first = 0;
  Q->last = -1;

  return Q;
}


void Dequeue(Queue *Q) {
  /* If Queue size is zero then it is empty. So we cannot pop */
  if(Q->size == 0) {
    printf("Queue is Empty\n");
    return;
  }

  /* Removing an element is equivalent to incrementing index of first by one */
  else {
  Q->size--;
  Q->first++;
  if(Q->first == MAX_CAPACITY) {
    Q->first = 0;
  }
  }
  return;
}


char* first(Queue *Q) {
  if(Q->size == 0) {
    printf("Queue is Empty\n");
    exit(0);
  }

  /* Return the element which is at the first*/
  return Q->elements[Q->first];
}


void Enqueue(Queue *Q, char* element) {
  /* If the Queue is full, we cannot push an element into it as there is no space for it.*/
  if(Q->size == MAX_CAPACITY) {
    printf("Queue is Full\n");
  } else {
    Q->size++;
    Q->last = Q->last + 1;
    if(Q->last == MAX_CAPACITY) {
      Q->last = 0;
    }
    /* Insert the element in its last side */
    Q->elements[Q->last] = element;
  }
  return;
}
