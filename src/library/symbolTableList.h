#ifndef symbolTableList_H
#define symbolTableList_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct symbolTableNode {
  char *stringVal;
  int32_t *address;
  struct symbolTableNode *next;
  struct symbolTableNode *prev;
} symbolTableNode;

typedef struct symbolTableList {
  symbolTableNode *first;
  symbolTableNode *last;
} symbolTableList;


typedef struct symbolTableNode* list_iter;

symbolTableNode *list_alloc_node(void);

void list_free_node(symbolTableNode *);

void list_initialise(symbolTableList *);

void list_insert(symbolTableList *, list_iter iter, char*, int32_t *);

void list_insert_front(symbolTableList *, char *, int32_t *);

void list_insert_back(symbolTableList *, char *, int32_t *);

void list_destroy(symbolTableList *);

///////////////////////////////Iterate/////////////////////////////////////////

list_iter begin_list(symbolTableList *);

list_iter end_list(symbolTableList *);

list_iter list_iter_next(list_iter iter);

list_iter list_iter_prev(list_iter iter);

char* list_iter_value(list_iter iter);

#define list_is_internal(iter) (iter->next != NULL && iter->next != NULL)

#endif
