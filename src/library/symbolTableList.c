#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "symbolTableList.h"

///////////////////// ADT Douvle-Linked List //////////////////////////////////

///////////////////////////////Iterate/////////////////////////////////////////

list_iter begin_list(symbolTableList *list) {
  return list->first->next;
}

list_iter end_list(symbolTableList *list) {
  return list->last;
}

list_iter list_iter_next(list_iter iter) {
  return iter->next;
}

char* list_iter_value(list_iter iter) {
  assert(list_is_internal(iter));
  return iter->stringVal;
}

uint16_t list_iter_addr(list_iter iter) {
  assert(list_is_internal(iter));
  return iter->address;
}

/////////////////////////////////main functions////////////////////////////////

symbolTableNode *list_alloc_node(void) {
  printf("start to do list_alloc_node\n");
  symbolTableNode *node = malloc(sizeof(symbolTableNode));
  printf("after malloc in list_alloc_node\n");
  if (node == NULL) {
    perror("list_alloc_node");
    exit(EXIT_FAILURE);
  }
  printf("We will return a node\n");
  return node;
}

void list_free_node(symbolTableNode *node) {
  free(node);
}

void list_initialise(symbolTableList *list) {
  printf("Start to initialise list\n");
  list->first = list_alloc_node();
  printf("after list_alloc_node\n");
  list->last = list_alloc_node();
  list->first->prev = NULL;
  list->last->next = NULL;
  list->first->next = list->last;
  list->last->prev = list->first;
  printf("finishing initialising list\n");
}

void list_insert(symbolTableList *list, list_iter iter, char* val, uint16_t key) {
  symbolTableNode *newNode = list_alloc_node();
  newNode->stringVal = val;
  newNode->address = key;

  newNode->prev = iter->prev;
  newNode->next = iter;
  iter->prev->next = newNode;
  iter->prev = newNode;

}

void list_insert_front(symbolTableList *list, char *val, uint16_t key) {
  list_insert(list, begin_list(list), val, key);

}

void list_insert_back(symbolTableList *list, char *val, uint16_t key) {
   list_insert(list, end_list(list), val, key);
 }

 uint16_t list_get_address(symbolTableList *list, char* val) {
   for(list_iter i = begin_list(list); i != end_list(list); i = list_iter_next(i)) {
     if(strcmp(list_iter_value(i), val) == 0) {
       return list_iter_addr(i);
     }
   }
   perror("string value not found in symbolTableList");
   exit(EXIT_FAILURE);
 }


void list_destroy(symbolTableList *list) {
  symbolTableNode *node = list->first;
  while (node != NULL) {
    symbolTableNode *nextNode = node->next;
    list_free_node(node);
    node = nextNode;
  }
}
