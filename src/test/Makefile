# Makefile for test Xbgi programs

SDL_bgi = ../libSDL_bgi.so

# C compiler: gcc or clang
CC = gcc
CFLAGS = -std=c99 -O2 -g -I.. -L..
LIBS = -lSDL -lSDL_gfx

PROGRAMS = bgidemo cellular dla fern floodfilltest hopalong life \
mandelbrot mousetest moveit simple simple-fast turtledemo

all: $(PROGRAMS)

bgidemo: bgidemo.c $(SDL_bgi)
	$(CC) $(CFLAGS) -I. -o bgidemo bgidemo.c $(SDL_bgi) $(LIBS) -lm

cellular: cellular.c $(SDL_bgi)
	$(CC) $(CFLAGS) -o cellular cellular.c $(SDL_bgi) $(LIBS)

dla: dla.c $(SDL_bgi)
	$(CC) $(CFLAGS) -o dla dla.c $(SDL_bgi) $(LIBS)

fern: fern.c $(SDL_bgi)
	$(CC) $(CFLAGS) -o fern fern.c $(SDL_bgi) $(LIBS)

floodfilltest: floodfilltest.c $(SDL_bgi)
	$(CC) $(CFLAGS) -o floodfilltest floodfilltest.c $(SDL_bgi) $(LIBS)

hopalong: hopalong.c $(SDL_bgi)
	$(CC) $(CFLAGS) -o hopalong hopalong.c $(SDL_bgi) $(LIBS) -lm

life: life.c $(SDL_bgi)
	$(CC) $(CFLAGS) -o life life.c $(SDL_bgi) $(LIBS)

mandelbrot: mandelbrot.c $(SDL_bgi)
	$(CC) $(CFLAGS) -o mandelbrot mandelbrot.c $(SDL_bgi) $(LIBS)

mousetest: mousetest.c $(SDL_bgi)
	$(CC) $(CFLAGS) -o mousetest mousetest.c $(SDL_bgi) $(LIBS)

moveit: moveit.c $(SDL_bgi)
	$(CC) $(CFLAGS) -o moveit moveit.c $(SDL_bgi) $(LIBS)

simple: simple.c $(SDL_bgi)
	$(CC) $(CFLAGS) -o simple simple.c $(SDL_bgi) $(LIBS)

simple-fast: simple-fast.c $(SDL_bgi)
	$(CC) $(CFLAGS) -o simple-fast simple-fast.c $(SDL_bgi) $(LIBS)

turtledemo: turtledemo.c turtle.c $(SDL_bgi)
	$(CC) $(CFLAGS) -o turtledemo turtledemo.c turtle.c $(SDL_bgi) $(LIBS) -lm

clean:
	rm -f $(PROGRAMS)
