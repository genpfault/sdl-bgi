/* dla.c  -*- C -*-
 * 
 * To compile:
 * gcc -o dla dla.c SDL_bgi.c -lSDL -lSDL_gfx -lm
 * 
 * Diffusion limited aggregation.
 * 
 * By Guido Gonzato, October 2014.
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

#include "SDL_bgi.h"

#define SIZE1 1024
#define SIZE2 768
#define ALIVE 1
#define DEAD  0

char screen[SIZE1][SIZE2];
void random_walk (void);
unsigned long iterations;

// -----

int main ()
{
  int x, y, stop = 0;
  unsigned int percent;
  int gd, gm;
  
  srand (time(NULL));
  
  for (x = 0; x < SIZE1; x++)
    for (y = 0; y < SIZE2; y++)
      screen[x][y] = 0;
  
  for (x = 0; x < 10; x++) 
    screen[random(SIZE1)][random(SIZE2)] = 1; // initial seeds
  
  gd = X11;
  gm = X11_1024x768;
  initgraph (&gd, &gm, "");
  
  setbkcolor (BLACK);
  cleardevice ();
  refresh ();
  iterations = 0;
  
  while (! stop) {
    
    // lay a particle
    random_walk ();
    
    if (iterations % 1000)
      if (kbhit ())
	stop = 1;
  }
  
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
  
  color = iterations % 50000;
  putpixel (x, y, COLOR (128 + color, 255 - color, 128 + color) );
  
}

// end of file
