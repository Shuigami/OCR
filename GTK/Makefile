# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -g -Wextra -O3 `pkg-config --cflags gtk+-3.0`
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0`

SRC = gtk.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: gtk

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
