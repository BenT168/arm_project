CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: emulate.o library/bitwise.o library/register.o

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
	rm -f library/bitwise
	rm -f library/register
	rm -f library/arm11
	rm -f library/instruction
	rm -f library/bitwise.o
	rm -f library/register.o
