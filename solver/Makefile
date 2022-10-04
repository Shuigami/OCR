# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3
LDFLAGS =
LDLIBS =

all: solver

SRC = solver.c solverprog.c tools.c
OBJ = ${SRC:.c=.o}
EXE = solver 

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
