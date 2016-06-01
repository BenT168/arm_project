#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTableList.h"
#include <assert.h>


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

/////////////////////////////////main functions////////////////////////////////

symbolTableNode *list_alloc_node(void) {
  symbolTableNode *node = malloc(sizeof(symbolTableNode));
  if (node == NULL) {
    perror("list_alloc_node");
    exit(EXIT_FAILURE);
  }
return node;
}

void list_free_node(symbolTableNode *node) {
  free(node);
}

void list_initialise(symbolTableList *list) {
list->first = list_alloc_node();
list->last = list_alloc_node();
list->first->prev = NULL;
list->last->next = NULL;
list->first->next = list->last;
list->last->prev = list->first;
}

void list_insert(symbolTableList *list, list_iter iter, char* val, int32_t *key) {
  symbolTableNode *newNode = list_alloc_node();
  newNode->stringVal = val;
  newNode->address = key;

  newNode->prev = iter->prev;
  newNode->next = iter;
  iter->prev->next = newNode;
  iter->prev = newNode;

}

void list_insert_front(symbolTableList *list, char *val, int32_t *key) {
  list_insert(list, begin_list(list), val, key);

}

void list_insert_back(symbolTableList *list, char *val, int32_t *key) {
   list_insert(list, end_list(list), val, key);
 }


void list_destroy(symbolTableList *list) {
  symbolTableNode *node = list->first;
  while (node != NULL) {
    symbolTableNode *nextNode = node->next;
    list_free_node(node);
    node = nextNode;
  }
}
