CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_GNU_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all	: 	assemble emulate

#To create an executable file assemble, we need sources files
#assemble.c,library/token.c, library/bitwise.c and library/assembler.c
#library/symbolTableADT.c
#
assemble: 	assemble.c library/tokens.c library/bitwise.c library/assembler.c library/symbolTableList.c library/queue.c

#To create an executable file emulate, we need sources files
#emulate.c, library/bitwise.c, library/gpio.c
#
emulate : emulate.c library/bitwise.c library/gpio.c


clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
