#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdint.h>
//#include <lcthw/darray.h>

//hashmap
/*
typedef int (*symboltableADT_compare)(void *a, void *b);
typedef uint32_t (*symboltableADT_hash)(void *key);

typedef struct symboltableADT {
    symboltableADT_compare compare;
    //symboltableNode *root;
     symboltableADT_hash hash;
     DArray *buckets;
} symboltableADT;

typedef struct symboltableNode {
    //struct symboltableNode *left;
    //struct symboltableNode *right;
    void *key;
    void *value;
    uint32_t hash;
} symboltableNode;


typedef enum {
  symbolKey = 1;
  symbolVal = 2;
} symboltableFlag;
*/

/*typedef void (*symboltableFunc)(symboltableNode *node);

symboltableADT* symboltableADT_create(symboltableADT_compare compare);

int symboltableADT_set(symboltableADT *symboltable, void *key, void *value);

void* symboltableADT_get(symboltableADT *symboltable, void *key);

int symboltableADT_traverse(symboltableADT *symboltable, symboltableFunc);

void* symboltableADT_delete(symboltableADT *symboltable, void *key);
*/

typedef int (*map_cmp)(void *, void *);

typedef struct map_entry
{
	void *key;
	void *value;
	struct map_entry *left, *right;
} map_entry;

typedef struct map
{
	map_entry *root;
	map_cmp    cmp;
} map;

typedef void (*map_func)(map_entry *);

typedef enum
{
	 MAP_FREE_KEY  = 1,
	 MAP_FREE_VAL  = 2
} map_free_flag;

////////////////////////////////////////////////////////////////////////////////

map  *map_new(map_cmp);

void *map_del(map *, void *);

void *map_get(map *, void *);

void  map_put(map *, void *, void *);

void  map_free(map *, map_free_flag);

void  map_iter(map *, map_func);

////////////////////////////////////////////////////////////////////////////////

int map_cmp_str(void *, void *);

int map_cmp_int(void *, void *);


#endif
