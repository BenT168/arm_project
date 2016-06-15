CFLAGS  = -Wall -g -std=c99 -Werror -pedantic
LFLAGS  =	`sdl-config --libs` -lSDL
OBJS    =	control.o show.o tower_hanoi.o
PROG    = tower
CXX			= gcc

#.SUFFIXES: .c .o
all: $(PROG)

# compiling other source files.
%.o: tower_hanoi/%.c tower_hanoi/%.h tower_hanoi/tower_struct.h
			$(CXX) $(CFLAGS) -c $<

# linking the program
$(PROG): $(OBJS)
	$(CXX) $(OBJS) -o $(PROG) $(LFLAGS)

# cleaning everything that can be automatically recreated with "make"
clean:
	rm $(PROG) *.o
