CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_GNU_SOURCE -std=c99 -Werror -pedantic
LDLIBS	= -lm

.SUFFIXES: .c .o

.PHONY: all clean

all :	calendargame

calendargame:	calendargame.o hangman.o historyeasylevel.o historymediumlevel.o historyhardlevel.o

calendargame.o:	calendargame.c calendargame.h

historyeasylevel.o:	historyeasylevel.c historyeasylevel.h

historymediumlevel.o:	historymediumlevel.c historymediumlevel.h

historyhardlevel.o:	historyhardlevel.c historyhardlevel.h

hangman.o:	hangman.c hangman.h


clean:
	rm -f $(wildcard *.o)
	rm -f calendargame
