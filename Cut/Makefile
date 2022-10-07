CC = gcc

CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDLIBS  = `pkg-config --libs sdl2 SDL2_image`


SRC = cut.c main.c

OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: main

main: ${OBJ}

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main
