# Makefile for SDL_bgi

SRC = SDL_bgi.c
OBJ = SDL_bgi.o
LIB = libSDL_bgi.so
INC_DIR = /usr/include/SDL/
LIB_DIR = /usr/lib/

# C compiler: tested with gcc and clang
CC = gcc
CFLAGS = -O2 -g -c -Wall -fpic

.PHONY : all
all: $(LIB)

OBJ:
	$(CC) $(CFLAGS) $(SRC)

libSDL_bgi.so: $(OBJ)
	$(CC) -shared -o $(LIB) $(OBJ)

install: $(LIB) SDL_bgi.h
	cp $(LIB) $(LIB_DIR); \
	cp SDL_bgi.h $(INC_DIR)

uninstall:
	rm -f $(INC_DIR)\SDL_bgi.h
	rm -f $(LIB_DIR)\$(LIB)

test: all
	cd test; make

clean:
	rm -f $(OBJ) $(LIB)
