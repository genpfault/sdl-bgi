# Makefile for test SDL_bgi programs
# Please note that f90_test.f90 is not included

# C compiler: gcc or clang
CC = gcc

# Detect the platform: GNU/Linux, Darwin (OS X), Mingw-w64
PLATFORM := $(shell uname)

ifeq ($(PLATFORM),Linux)
CFLAGS = -O2 -g -I. -Wall
LIBS = -lSDL_bgi -lSDL2
endif

ifeq ($(PLATFORM),Darwin)
CFLAGS = -O2 -g -I. -I/library/Frameworks/SDL2.framework/Headers
LIBS = -lSDL_bgi -framework SDL2
endif

# MSYS2 includes version number in uname - dumb idea indeed
OS := $(shell uname -o)
ifeq ($(OS),Msys)
CFLAGS = -O2 -g -I. -I/mingw64/include/SDL2
# -mconsole = open a console alongside the program
# -mwindows = Windows-only program (no console)
LIBS = -lmingw32 -L/mingw64/bin -lSDL_bgi -lSDL2main -lSDL2 # -mwindows
endif

PROGRAMS = cellular dla fern floodfilltest hopalong life \
           kaleido mandelbrot mousetest moveit multiwin plasma \
	   psychedelia sdlbgidemo simple turtledemo

all: $(PROGRAMS)

cellular: cellular.c 
	$(CC) $(CFLAGS) -o cellular cellular.c $(LIBS)

dla: dla.c 
	$(CC) $(CFLAGS) -o dla dla.c $(LIBS)

fern: fern.c 
	$(CC) $(CFLAGS) -o fern fern.c $(LIBS)

floodfilltest: floodfilltest.c 
	$(CC) $(CFLAGS) -o floodfilltest floodfilltest.c $(LIBS)

hopalong: hopalong.c 
	$(CC) $(CFLAGS) -o hopalong hopalong.c $(LIBS) -lm

life: life.c 
	$(CC) $(CFLAGS) -o life life.c $(LIBS)

kaleido: kaleido.c 
	$(CC) $(CFLAGS) -o kaleido kaleido.c $(LIBS)

mandelbrot: mandelbrot.c 
	$(CC) $(CFLAGS) -o mandelbrot mandelbrot.c $(LIBS)

mousetest: mousetest.c 
	$(CC) $(CFLAGS) -o mousetest mousetest.c $(LIBS)

moveit: moveit.c 
	$(CC) $(CFLAGS) -o moveit moveit.c $(LIBS)

multiwin: multiwin.c 
	$(CC) $(CFLAGS) -o multiwin multiwin.c $(LIBS)

plasma: plasma.c 
	$(CC) $(CFLAGS) -o plasma plasma.c $(LIBS) -lm

psychedelia: psychedelia.c 
	$(CC) $(CFLAGS) -o psychedelia psychedelia.c $(LIBS) -lm

sdlbgidemo: sdlbgidemo.c 
	$(CC) $(CFLAGS) -I. -o sdlbgidemo sdlbgidemo.c $(LIBS) -lm

simple: simple.c 
	$(CC) $(CFLAGS) -o simple simple.c $(LIBS)

turtledemo: turtledemo.c turtle.c 
	$(CC) $(CFLAGS) -o turtledemo turtledemo.c turtle.c $(LIBS) -lm

clean:
	rm -f $(PROGRAMS) a.out *o f90_test
