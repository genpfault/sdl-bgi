/* hopalong.c  -*- C -*-
 * 
 * To compile:
 * gcc -o hopalong hopalong.c SDL_bgi.c -lSDL -lSDL_gfx -lm
 * 
 * By Guido Gonzato, March 2013.
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
#include <math.h>

#include "SDL_bgi.h"

/* ----- */

int sgn (float n)
{
  return (n < 0) ? -1: 1;
}

/* ----- */

int main (int argc, char **argv)
{
  int gd, gm, stop;
  unsigned int seed;
  unsigned long int counter;
  float j, k, x, y, xx, xp, yp, r, xoffs, yoffs;
  
  if (argc == 2)
    seed = atoi (argv [1]); /* no checks! */
  else {  
    printf ("Seed: ");
    j = scanf ("%d", &seed);
  }
  srand (seed);
  
  // gd = X11;
  // gm = getmaxmode ();
  detectgraph (&gd, &gm);
  initgraph(&gd, &gm, "");
  
  setbkcolor (BLACK);
  cleardevice ();
  setcolor (YELLOW);
  outtextxy (0, 0, "Press a key or right click to exit: ");
  
  xoffs = getmaxx () / 2;
  yoffs = getmaxy () / 3;
  j = random (100);
  k = random (100);
  x = y = xx = xp = yp = r = 0.0;
  stop = counter = 0;
  setcolor (COLOR (random (256), random (256), random (256)));

  while (! stop) {
    xx = y - sgn (x) * sqrt (abs (k * x - 1));
    y = j - x;
    x = xx;
    xp = x * 2 + xoffs;
    yp = y * 2 + yoffs;
    _putpixel (xp, yp);
    if (++counter == 50000) {
      counter = 0;
      setcolor (COLOR (random (256), random (256), random (256)));
      refresh ();
      if (event ())
	stop = 1;
    }
  }
   
  closegraph ();
  return 0;
}
