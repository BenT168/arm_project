#include <stdint.h>
#include <stdio.h>
#include "bitwise.h"


/* PrintBits Function */
void print_bits(int32_t x)
{
    int i;
    uint32_t mask = 1 << 31;

    for (i = 0; i < 32; ++i)
    {
        if((x & mask) == 0)
            printf("0");
        else
            printf("1");

        x = x << 1;
    }
    printf("\n");
}


/*Prints Bits in Big Endian */
void print_bits_inBE(int32_t i)
{
  print_bits(((i>>24)&0xff) |
  //byte 3 to 0
  ((i<<8)&0xff0000) |
  // byte 1 to 2
  ((i>>8)&0xff00) |
  //  byte 2 to 1
  ((i<<24)&0xff000000));
  // byte 0 to 3
}

/* Get Bits Function */
int32_t get_bits(int32_t i, int start, int end)
{
    uint32_t mask;

    if (end - start == 31)
        return i;

    /* create a mask that matches the bits between start and end*/
    mask = ((1 << (end - start + 1)) - 1) << start;
    /* use the & operation to extract the required bits*/
    return (mask & i) >> start;
}


/* Rotate Right Function */
int32_t rotate_right(int32_t x, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
      int last = BIT_GET(x, 0);
      x >>= 1;
      BIT_PUT(x, 31, last);
    }
    return x;
}

/* Rotate Left Function */
int32_t rotate_left(int32_t x, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        int last = BIT_GET(x, 31);
        x <<= 1;
        BIT_PUT(x, 0, last);
    }
    return x;
}
