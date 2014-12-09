/* simple-fast.c  -*- C -*-
 * 
 * To compile:
 * gcc -o simple-fast simple-fast.c SDL_bgi.c -lSDL -lSDL_gfx -lm
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
#include "SDL_bgi.h"

/* ----- */

#define SDL_BGI_AA 1

int main ()
{

  int i, col, gd, gm, x, y, stop = 0;
  
  // we use initwindow() instead of initgraph()
  initwindow (1024, 768);
  setbkcolor (BLACK);
  cleardevice ();
  x = getmaxx ();
  y = getmaxy ();

  /* circles */
  while (! stop) {
    setcolor (YELLOW);
    outtextxy (0, 0, "Press a key to continue");
    for (i = 0; i < 500; i++) {
      setcolor (1 + random (15));
      circle  (random(x), random(y), random (100));
    }
    // let's refresh the screen
    refresh ();
    sleep (1);
    cleardevice ();
    if (kbhit ())
      stop = 1;
  }
  stop = 0;
  
  /* lines */
  while (! stop) {
    setcolor (YELLOW);
    outtextxy (0, 0, "Press a key to continue");
    for (i = 0; i < 500; i++) {
      setlinestyle (SOLID_LINE, 0, 1 + random(THICK_WIDTH));
      setcolor (1 + random (15));
      line (random(x), random(y), random(x), random(y));
    }
    // let's refresh the screen
    refresh ();
    sleep (1);
    cleardevice ();
    if (kbhit ())
      stop = 1;
  }
  stop = 0;
  
  /* pixels */
  while (! stop) {
    setcolor (YELLOW);
    outtextxy (0, 0, "Press a key to exit");
    srand (2014);
    for (i = 0; i < 1000; i++) {
      col = 1 + random (MAXCOLORS);
      putpixel (random(x), random(y), col);
    }
    // let's refresh the screen
    refresh ();
    sleep (1);
    srand (2014);
    for (i = 0; i < 1000; i++) {
      col = 1 + random (MAXCOLORS); /* keep random () in sync */
      putpixel (random(x), random(y), BLACK);
      if (kbhit ())
	stop = 1;
    }
    // let's refresh the screen
    refresh ();
    sleep (1);
    cleardevice ();
  }
  
  closegraph ();
  return 0;
  
}
