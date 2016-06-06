#ifndef _SYMBOLTABLELIST_H
#define _SYMBOLTABLELIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


int cmp_str(void *, void *);

int cmp_int(void *, void *);

////////////////////////////////////////////////////////////////////////////////


typedef struct symbolTableNode {
  char *stringVal;
  uint16_t address;
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

void list_insert(symbolTableList *, list_iter iter, char*, uint16_t);

void list_insert_ascending(symbolTableList *, char *, uint16_t);

void list_insert_front(symbolTableList *, char *, uint16_t);

void list_insert_back(symbolTableList *, char *, uint16_t);

uint16_t list_get_address(symbolTableList *, char*);

void list_destroy(symbolTableList *);


void displayList(symbolTableList *);

///////////////////////////////Iterate/////////////////////////////////////////

list_iter begin_list(symbolTableList *list);

list_iter end_list(symbolTableList *list);

list_iter list_iter_next(list_iter iter);

list_iter list_iter_prev(list_iter iter);

char* list_iter_value(list_iter iter);

uint16_t list_iter_addr(list_iter iter);

#define list_is_internal(iter) (iter->next != NULL && iter->next != NULL)



#endif
