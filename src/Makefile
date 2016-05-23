CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic
SOURCES = library/arm11.h library/bitwise.h library/bitwise.c library/instruction.h library/registerwise.h library/registerwise.c

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: emulate.o library/bitwise.o library/registerwise.o

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
	rm -f library/bitwise
	rm -f library/registerwise
