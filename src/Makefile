CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o


emulate: emulate.o

#To create an object file emulate.o, we need source files emulate.c, 
#bitwise.o and register.o
emulate.o: emulate.c  library/bitwise.o library/register.o

#To create an object file bitwise.o, we need sources files bitwise.h 
#and bitwise.c
library/bitwise.o: library/bitwise.h library/bitwise.c

#To create an object file bitwise.o we need sources file register.h and
# register.c
library/bitwise.o: library/register.h library/register.c

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
	rm -f library/bitwise.o
	rm -f library/register.o
