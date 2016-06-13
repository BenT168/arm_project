CC      = gcc
CFLAGS  = -Wall -g -std=c99 -Werror -pedantic
LFLAGS  ='sdl-config --libs' -lSDL
OBJS    = tower.o
PROG    = sdlcode

.SUFFIXES: .c .o

.PHONY: all clean

all:	$(PROG)

$(PROG): $(OBJS)
	      $(CC) $(OBJS) -o &(PROG) $(LFLAGS)


tower:	tower_hanoi.c tower_main.c show.c

clean:
  rm $(PROG) *.o
