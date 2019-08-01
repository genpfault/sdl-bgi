# Makefile for SDL_bgi

VERSION := $(shell cat ../VERSION)
NAME = SDL_bgi
SRC = $(NAME).c
OBJ = $(NAME).o
HEADERS = SDL_bgi.h 

# Detect the platform: GNU/Linux, Darwin (OS X), MSYS2
PLATFORM := $(shell uname)

ifeq ($(PLATFORM),Linux)
INC_DIR = /usr/include/
SDL_INC = $(INC_DIR)/SDL2
LIB_DIR = /usr/lib/
LIB     = lib$(NAME).so
LDFLAGS = -lSDL2
STRIP   = strip
endif

ifeq ($(PLATFORM),Darwin)
INC_DIR = /usr/include
SDL_INC = /library/Frameworks/SDL2.framework/Headers
LIB_DIR = /usr/lib
LIB     = lib$(NAME).so
LDFLAGS = -framework SDL2
STRIP   = echo # don't strip the library
endif

# MSYS2 includes version number in uname - dumb idea indeed
OS := $(shell uname -o)
ifeq ($(OS),Msys)
INC_DIR = /mingw64/include
SDL_INC = $(INC_DIR)/SDL2
LIB_DIR = /mingw64/bin
LIB     = $(NAME).dll
LDFLAGS = -L/mingw64/bin -lSDL2
STRIP   = strip
endif

# C compiler: tested with gcc and clang
CC = gcc
CFLAGS = -O2 -g -c -Wall -I $(INC_DIR) -fPIC

.PHONY : all
all: $(LIB)

OBJ:
	$(CC) $(CFLAGS) $(SRC)

$(LIB): $(OBJ)
	$(CC) -shared -o $(LIB) $(OBJ) $(LDFLAGS) ; \
	$(STRIP) $(LIB)

install: $(LIB) $(HEADERS)
	install -m 755 $(LIB) $(LIB_DIR) ; \
	install -m 644 $(HEADERS) $(SDL_INC) ; \
	install graphics.h $(INC_DIR)

uninstall:
	rm -f $(SDL_INC)/SDL_bgi.h ; \
	rm -f $(INC_DIR)/graphics.h
	rm -f $(LIB_DIR)/$(LIB)

test: all
	cd test; make

clean:
	rm -f $(OBJ) $(LIB) a.out
