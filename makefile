CC = gcc
OBJS = main.o helpingfunc.o gameCommands.o parser.o gurobifunc.o initstruct.o
EXEC = sudoku-console
COMP_FLAGS = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c parser.h initstruct.h
	$(CC) $(COMP_FLAGS) -c $*.c
helpingfunc.o: helpingfunc.c parser.h helpingfunc.h initstruct.h gameCommands.h
	$(CC) $(COMP_FLAGS) -c $*.c
gameCommands.o: gameCommands.c helpingfunc.h parser.h gurobifunc.h gameCommands.h initstruct.h
	$(CC) $(COMP_FLAGS) -c $*.c
parser.o: parser.c helpingfunc.h parser.h gameCommands.h 
	$(CC) $(COMP_FLAGS) -c $*.c
initstruct.o: gameCommands.h parser.h helpingfunc.h initstruct.h initstruct.c
	$(CC) $(COMP_FLAGS) -c $*.c
gurobifunc.o: gurobifunc.c gurobifunc.h helpingfunc.h initstruct.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
clean:
	rm -f *.o $(EXEC)
