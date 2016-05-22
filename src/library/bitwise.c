#include <stdint.h>
<<<<<<< HEAD
#include <stdint.h>
=======
#include <stdio.h>
>>>>>>> cyw115
#include "bitwise.h"

/*PrintBits Function */
void print_bits(int32_t x){
        int i;
        uint32_t mask = 1 << 31;

        for(i=0; i<32; ++i) {
                if((x & mask) == 0){
                        printf("0");
                } else {
                        printf("1");
                }
                x = x << 1;
        }
        printf("/n");
}

/* Get Bits Function */
int32_t get_bits(int32_t i, int start, int end){
  if(end - start == 31){
    return i;
  }
    //create a mask that matches the bits between start and end
    int32_t mask = (((1 << (end - start + 1)) - 1)  >> (31 - end)) << start;
    //use the & operation to extract the required bits
    int32_t result = mask & i;
    return result >> start;

}

void set_bits(int32_t i, int start, int end) {
  
}

/* Rotate_Right Function */
int32_t rotate_right(int32_t x, int n){
  for (int i = 0; i < n; i++) {
    int last_bit = BIT_GET(x, 0);
    x >>= 1;
    BIT_PUT(x, 31, last_bit);
  }
  return x;
}

/* Rotate_Left Function */
int32_t rotate_left(int32_t x, int n){
  for (int i = 0; i < n; i++) {
    int last_bit = BIT_GET(x, 31);
    x <<= 1;
    BIT_PUT(x, 0, last_bit);
  }
  return x;
}
<<<<<<< HEAD

=======
>>>>>>> cyw115
