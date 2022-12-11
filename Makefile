# Makefile

CC = gcc -I./utils -I./processing -I./grid_detection -I./Cut -I./neuralNetwork
CPPFLAGS =
CFLAGS = -g -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0` `pkg-config --libs sdl2 SDL2_image` -lm

SRC = main_test.c $(wildcard utils/*.c) $(wildcard processing/*.c) $(wildcard grid_detection/*.c) $(wildcard Cut/*.c) $(wildcard neuralNetwork/*.c)	
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: gui nogui

gui: gui.o ${OBJ}

nogui: nogui.o ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} Final*
	${RM} gui
	${RM} gui.o
	${RM} nogui

# END
