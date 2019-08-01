/* dla.c  -*- C -*-
 * 
 * To compile:
 * gcc -o dla dla.c -lSDL_bgi -lSDL2
 * 
 * Diffusion limited aggregation.
 * Try to run this program with and without 'auto mode',
 * that is setting and unsetting the environment variable
 * SDL_BGI_RATE=auto
 * 
 * By Guido Gonzato, May 2015.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <graphics.h>

#define SIZE1 800
#define SIZE2 600
#define ALIVE 1
#define DEAD  0

char screen[SIZE1][SIZE2];
void random_walk (void);
unsigned long iterations;

// -----

int main (int argc, char *argv[])
{
  int x, y, stop = 0;
  int gd, gm;
  
  srand (time(NULL));
  
  for (x = 0; x < SIZE1; x++)
    for (y = 0; y < SIZE2; y++)
      screen[x][y] = 0;
  
  for (x = 0; x < 10; x++) 
    screen[random(SIZE1)][random(SIZE2)] = 1; // initial seeds
  
  gd = SDL;
  gm = SDL_800x600;
  initgraph (&gd, &gm, "");
  
  setbkcolor (BLACK);
  cleardevice ();
  iterations = 0;
  
  // set up a palette

  // initialise it to black
  for (x = 0; x < PALETTE_SIZE; x++)
    setrgbpalette (x, 0, 0, 0);
  
  // blue fading to white
  for (x = 0; x < 128; x++)
    setrgbpalette (x, 2*x, 2*x, 255);

  // white fading to yellow
  for (x = 128; x < 256; x++)
    setrgbpalette (x, 255, 255, 255 - x);
  
  // yellow fading to red
  for (x = 256; x < 512; x++)
    setrgbpalette (x, 255, 512 - x, 0);
  
  // red fading to black
  for (x = 512; x < 1024; x += 2)
    setrgbpalette (x, 1024 - 2*x, 0, 0);
  
  // the window is full after ~ 500k iterations.
  
  while (! stop) {
    
    // lay a particle
    random_walk ();
    
    if (iterations % 5000) {
      if (xkbhit ())
	stop = 1;
    }
  }

  printf ("Iterations computed: %lu\n", iterations);
  closegraph ();
  
}

// -----

#define NORTH(n) (n == 0) ? (SIZE2 - 1) : (n - 1)
#define SOUTH(n) (n == SIZE2 - 1) ? (0) : (n + 1)
#define WEST(n)  (n == 0) ? (SIZE1 - 1) : (n - 1)
#define EAST(n)  (n == SIZE1 - 1) ? (0) : (n + 1)

// -----

void random_walk (void)
{
  int x, y, n, done = 0;
  int color;
  
  x = random (SIZE1);
  y = random (SIZE2);
  
  // start random walk until another pixel is hit
  
  do {
    n = random (8);
    
    switch (n) {
     
    case 0:
      x = EAST(x);
      break;
      
    case 1:
      x = EAST(x);
      y = NORTH(y);
      break;
      
    case 2:
      y = NORTH(y);
      break;
            
    case 3:
      x = WEST(x);
      y = NORTH(y);
      break;
      
    case 4:
      x = WEST(x);
      break;
      
    case 5:
      x = WEST(x);
      y = SOUTH(y);
      break;
      
    case 6:
      y = SOUTH(y);
      break;
      
    case 7:
      x = EAST(x);
      y = SOUTH(y);
      break;
    
    } // switch
    
    // any neighbours?
    
    if ( (screen[EAST(x)][y]) ||
	 (screen[EAST(x)][NORTH(y)]) ||
	 (screen[x][NORTH(y)]) ||
	 (screen[WEST(x)][NORTH(y)]) ||
	 (screen[WEST(x)][y]) ||
	 (screen[WEST(x)][SOUTH(y)]) ||
	 (screen[x][SOUTH(y)]) ||
	 (screen[EAST(x)][SOUTH(y)]) ) {
      screen[x][y] = 1;
      iterations++;
      done = 1;
    }
    
  } while (! done);
  
  color = iterations / 150;
  if (color > 1024)
    color = 1024;
  setrgbcolor (color);
  _putpixel (x, y);
  
}

// ----- end of file dla.c
