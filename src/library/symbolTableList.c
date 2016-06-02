#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "symbolTableList.h"

///////////////////// ADT Double-Linked List //////////////////////////////////

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

//printf("alloc first\n");
list->first = list_alloc_node();
//printf("alloc last\n");
list->last = list_alloc_node();
//printf(" first prev\n");
list->first->prev = NULL;
//printf(" last next\n");
list->last->next = NULL;
//printf("first next\n");
list->first->next = list->last;
//printf("last prev\n");
list->last->prev = list->first;
}

void list_insert(symbolTableList *list, list_iter iter, char* val, uint16_t key) {
  struct symbolTableNode *newNode = list_alloc_node();
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

void displayList(symbolTableList *list) {

   //start from the beginning
  symbolTableNode* node = list->first;

   //navigate till the end of the list
   printf("\n[ ");

   while(node != NULL) {
      printf("(%s,%i)", node->stringVal ,node->address);
      node = node->next;
   }

   printf(" ]");
}
