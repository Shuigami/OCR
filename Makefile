# Makefile

CC = gcc -I./utils -I./processing -I./grid_detection -I./Cut -I./neuralNetwork 
CPPFLAGS =
CFLAGS = -g -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

SRC = $(wildcard utils/*.c) $(wildcard processing/*.c) $(wildcard grid_detection/*.c) $(wildcard Cut/*.c) $(wildcard neuralNetwork/*.c) 
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: main

main: ${OBJ}

test: CFLAGS = -O3 `pkg-config --cflags sdl2 SDL2_image`
test: main

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} Final*

# END
