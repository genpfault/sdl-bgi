# Makefile for SDL_bgi

VERSION = 2.0.8
NAME = SDL_bgi
SRC = $(NAME).c
OBJ = $(NAME).o
LIB = lib$(NAME).so.$(VERSION)
INC_DIR = /usr/include/SDL2/
LIB_DIR = /usr/lib/

# C compiler: tested with gcc and clang
CC = gcc
CFLAGS = -O2 -g -c -Wall -fpic

.PHONY : all
all: $(LIB)

OBJ:
	$(CC) $(CFLAGS) $(SRC)

$(LIB): $(OBJ)
	$(CC) -shared -o $(LIB) $(OBJ) ; \
	strip $(LIB); \
	ln -s ./$(LIB) lib$(NAME).so

install: $(LIB) SDL_bgi.h
	cp $(LIB) $(LIB_DIR) ; \
	cp SDL_bgi.h $(INC_DIR) ; \
	ln -sf $(INC_DIR)/SDL_bgi.h /usr/include/graphics.h

uninstall:
	/bin/rm -f $(INC_DIR)/SDL_bgi.h ; \
	/bin/rm -f $(LIB_DIR)/$(LIB) ; \
	/bin/rm -f /usr/include/graphics.h

test: all
	cd test; make

clean:
	rm -f $(OBJ) $(LIB) lib$(NAME).so
