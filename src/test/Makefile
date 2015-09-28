# Makefile for test SDL_bgi programs
# Please note that f90_test.f90 is not included

# C compiler: gcc or clang
CC = gcc
CFLAGS = -std=c99 -O2 -g -I.. -L..
LIBS = -lSDL2
SDL2_bgi = ../libSDL_bgi.so

PROGRAMS = cellular dla fern floodfilltest hopalong life \
mandelbrot mousetest moveit sdlbgidemo simple turtledemo pixels

all: $(PROGRAMS)

../libSDL_bgi.so:
	cd ..; make

sdlbgidemo: sdlbgidemo.c $(SDL2_bgi)
	$(CC) $(CFLAGS) -I. -o sdlbgidemo sdlbgidemo.c $(SDL2_bgi) $(LIBS) -lm

cellular: cellular.c $(SDL2_bgi)
	$(CC) $(CFLAGS) -o cellular cellular.c $(SDL2_bgi) $(LIBS)

dla: dla.c $(SDL2_bgi)
	$(CC) $(CFLAGS) -o dla dla.c $(SDL2_bgi) $(LIBS)

fern: fern.c $(SDL2_bgi)
	$(CC) $(CFLAGS) -o fern fern.c $(SDL2_bgi) $(LIBS)

floodfilltest: floodfilltest.c $(SDL2_bgi)
	$(CC) $(CFLAGS) -o floodfilltest floodfilltest.c $(SDL2_bgi) $(LIBS)

hopalong: hopalong.c $(SDL2_bgi)
	$(CC) $(CFLAGS) -o hopalong hopalong.c $(SDL2_bgi) $(LIBS) -lm

life: life.c $(SDL2_bgi)
	$(CC) $(CFLAGS) -o life life.c $(SDL2_bgi) $(LIBS)

mandelbrot: mandelbrot.c $(SDL2_bgi)
	$(CC) $(CFLAGS) -o mandelbrot mandelbrot.c $(SDL2_bgi) $(LIBS)

mousetest: mousetest.c $(SDL2_bgi)
	$(CC) $(CFLAGS) -o mousetest mousetest.c $(SDL2_bgi) $(LIBS)

moveit: moveit.c $(SDL2_bgi)
	$(CC) $(CFLAGS) -o moveit moveit.c $(SDL2_bgi) $(LIBS)

simple: simple.c $(SDL2_bgi)
	$(CC) $(CFLAGS) -o simple simple.c $(SDL2_bgi) $(LIBS)

pixels: pixels.c $(SDL2_bgi)
	$(CC) $(CFLAGS) -o pixels pixels.c $(SDL2_bgi) $(LIBS)

turtledemo: turtledemo.c turtle.c $(SDL2_bgi)
	$(CC) $(CFLAGS) -o turtledemo turtledemo.c turtle.c $(SDL2_bgi) $(LIBS) -lm

clean:
	rm -f $(PROGRAMS) a.out
