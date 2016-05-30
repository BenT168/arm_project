#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "symbolTableADT.h"

/*

static uint32_t default_hash(void *a)
{
    size_t len = blength((bstring)a);
    char *key = bdata((bstring)a);
    uint32_t hash = 0;
    uint32_t i = 0;

    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

//////////////////////symboltableADT Main functions ///////////////////////////
symboltableADT* symboltableADT_create(symboltableADT_compare compare) {
  symboltableADT *symbolTable = malloc(sizeof(symboltableADT));
  symbolTable->root = NULL;
  symbolTable->compare = compare;
  return symbolTable;
}

void symboltableADT_set(symboltableADT *symboltable, void *key, void *value){
  symboltable->root
    = put_symboltable(symboltable->root, key, value, symboltable->compare);
}

void* symboltableADT_get(symboltableADT *symboltable, void *key){
  return get_symboltable(symboltable->root, key, symboltable->compare);
}

void symboltableADT_traverse(symboltableADT *symboltable, symboltableFunc) {
  traverse_symboltable(symboltable->root, symboltableFunc);
}

void* symboltableADT_delete(symboltableADT *symboltable, void *key) {
  return NULL;
}

void symboltableADT_free(symboltableADT *symboltable, symboltableFlag flag) {
  free_symboltable(symboltable->root, flag);
}

//////////////////////symboltableADT Helper functions //////////////////////////

symboltableADT* put_symboltable(symboltableADT *symbolTable,
                        void *key,void *value, symboltableADT_compare compare) {
  if(symbolTable != NULL){
    int cmp = compare(key, symbolTable->key);
    if()
  }
} */


int map_cmp_str(void *mic, void *mik)
{
	return strcmp((char *) mic, (char *) mik);
}


int map_cmp_int(void *mic, void *mik)
{
	return *((int *) mic) - *((int *) mik);
}

///////////////////////////////////////////////////////////////////////////  NEW

map *map_new(map_cmp cmp)
{
	map *m  = malloc(sizeof(map));
	m->cmp  = cmp;
	m->root = NULL;
	return m;
}


static map_entry *entry_new(void *key, void *value)
{
	map_entry *e = malloc(sizeof(map_entry));
	e->key       = key;
	e->value     = value;
	e->left      = NULL;
	e->right     = NULL;
	return e;
}

//////////////////////////////////////////////////////////////////////////  FREE

static void entry_free(map_entry *e, map_free_flag flag)
{
	if (e == NULL) return;
	entry_free(e->left,  flag);
	entry_free(e->right, flag);

	if ((flag & MAP_FREE_VAL) == MAP_FREE_VAL) free(e->value);
	if ((flag & MAP_FREE_KEY) == MAP_FREE_KEY) free(e->key);

	free(e);
}


void map_free(map *m, map_free_flag flag)
{
	entry_free(m->root, flag);
	free(m);
}

///////////////////////////////////////////////////////////////////////////  GET

static void *entry_get(map_entry *e, void *key, map_cmp cmp)
{
	if (e == NULL) return NULL;

	int c = cmp(key, e->key);

	if      (c == 0) return e->value;
	else if (c < 0)  return entry_get(e->left,  key, cmp);
	else             return entry_get(e->right, key, cmp);
}


void *map_get(map *m, void *key)
{
	return entry_get(m->root, key, m->cmp);
}

///////////////////////////////////////////////////////////////////////////  PUT

static map_entry *entry_put(map_entry *e, void *key, void *value, map_cmp cmp)
{
	if (e == NULL) return entry_new(key, value);

	int c = cmp(key, e->key);

	if		  (c == 0) e->value = value;
	else if	(c < 0)  e->left  = entry_put(e->left,  key, value, cmp);
	else					   e->right = entry_put(e->right, key, value, cmp);

	return e;
}


void map_put(map *m, void *key, void *value)
{
	m->root = entry_put(m->root, key, value, m->cmp);
}

//////////////////////////////////////////////////////////////////////////  ITER

static void entry_iter(map_entry *e, map_func func)
{
	if (e == NULL) return;
	entry_iter(e->left,  func);
	func(e);
	entry_iter(e->right, func);
}


void map_iter(map *m, map_func func)
{
	entry_iter(m->root, func);
}
