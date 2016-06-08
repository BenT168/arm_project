#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_CAPACITY 50

typedef struct Queue
{
        int size;
        int first;
        int last;
        char** elements;
} Queue;

//////////////////////////////Queue functions//////////////////////////////////
Queue* createQueue();
void Dequeue(Queue *Q);
char* first(Queue *Q);
void Enqueue(Queue *Q, char* element);


#endif
