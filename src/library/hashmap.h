#ifndef __HASHMAP_H
#define __HASHMAP_H

#define MAP_MISSING -3  /* No such element */
#define MAP_FULL -2 	/* Hashmap is full */
#define MAP_OMEM -1 	/* Out of Memory */
#define MAP_OK 0 	/* OK */

typedef void *any_t;

typedef int (*PFany)(any_t, any_t);

typedef any_t map_t;


extern map_t hashmap_new();

extern int hashmap_iterate(map_t in, PFany f, any_t item);

extern int hashmap_put(map_t in, int key, any_t value);

extern int hashmap_get(map_t in, int key, any_t *arg);

extern int hashmap_remove(map_t in, int key);

extern void hashmap_free(map_t in);


#endif
