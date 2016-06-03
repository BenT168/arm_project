#ifndef _BIT_WISE
#define _BIT_WISE

#include <stdio.h>
#include <stdint.h>

void print_bits(int32_t i);
/*PrintBits Function*/
void print_bits_inBE(int32_t i);
/*Print bits in big endian */
int32_t get_bits(int32_t i, int start, int end);
/*GetBits Function*/
int32_t rotate_right(int32_t i, int n);
/*Rotate Right Function*/
int32_t rotate_reft(int32_t i, int n);
/*Rotate Left Function*/


/* Set/Clear/Get/Flip/Put the bit n pos of int i where 0 is rightmost bit */

#define BOOL(x) (!(!(x)))

#define BIT_SET(i, pos)       ((i) |=  (1 << (pos)))
#define BIT_CLR(i, pos)       ((i) &= ~(1 << (pos)))
#define BIT_GET(i, pos)   BOOL((i) &   (1 << (pos)))
#define BIT_FLP(i, pos)       ((i) ^   (1 << (pos)))
#define BIT_PUT(i, pos, b) { (IS_SET(b)) ? BIT_SET(i, pos) : BIT_CLR(i, pos); }

#define IS_SET(i)   (i == 1)
#define IS_CLEAR(i) (i == 0)


#endif
