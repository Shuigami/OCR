# Makefile

CC = gcc -I./utils -I./processing -I./grid_detection -I./Cut -I./neuralNetwork -I./solver
CPPFLAGS= `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl2` -D__NO_INLINE__
CFLAGS=  -g -Wextra  -std=c99 -O0
LDFLAGS= 
LDLIBS= `pkg-config --libs gtk+-3.0` `pkg-config --libs sdl2` -lSDL2_image -lm -lpthread

SRC = Cut/cut.c $(wildcard solver/*.c) $(wildcard utils/*.c) $(wildcard processing/*.c) $(wildcard grid_detection/*.c) $(wildcard Cut/*.c) $(wildcard neuralNetwork/*.c)
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: sudoku-ocr sudoku-ocr-gui

sudoku-ocr: sudoku-ocr.o ${OBJ}

sudoku-ocr-gui: sudoku-ocr-gui.o ${OBJ}


.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} Final*
	${RM} split.txt
	${RM} sudoku-ocr.o
	${RM} sudoku-ocr-gui.o

# END
