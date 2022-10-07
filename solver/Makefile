# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3
LDFLAGS =
LDLIBS =

all: main

SRC = solverprog.c tools.c solver.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

main: 	${OBJ}
	gcc -Wall -Wextra -O3 tools.o solverprog.o solver.o -o solver

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
