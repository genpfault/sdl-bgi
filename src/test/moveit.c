/* moveit.c  -*- C -*-
 * 
 * To compile:
 * gcc -o moveit moveit.c SDL_bgi.c -lSDL -lSDL_gfx -lm
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
#include <string.h>
#include <math.h>
#include "SDL_bgi.h"

/* ----- */

int main ()
{

  int c, gd, gm, x, y;
  
  gd = X11;
  gm = X11_1024x768;
  initgraph (&gd, &gm, "");
  setbkcolor (BLACK);
  setcolor (YELLOW);
  cleardevice ();
  x = getmaxx () / 2;
  y = getmaxy () / 2;
  outtextxy (0, 0, "Press movement keys (ESC=exit)");

  do {
    
    setcolor (WHITE);
    circle (x, y, 10);
    c = getch ();
    setcolor (RED);
    circle (x, y, 10);
    // added!
    refresh ();
    
    switch (c) {
    case KEY_UP:
      y -= 20;
      break;
    case KEY_DOWN:
      y += 20;
      break;
    case KEY_LEFT:
      x -= 20;
      break;
    case KEY_RIGHT:
      x += 20;
      break;
    case KEY_HOME:  
      x = 10;
      y = 20;
      break;
    case KEY_PGUP:  
      y -= 80;
      break;
    case KEY_PGDN:  
      y += 80;
      break;      
    case KEY_END:  
      x = getmaxx () - 10;
      y = getmaxy () - 10;
      break;
    default:
      printf ("Key code: %d\n", c);
    }
    
    if (x < 0)
      x = getmaxx ();
    if (x > getmaxx ())
      x = 0;
    if (y < 20)
      y = getmaxy ();
    if (y > getmaxy ())
      y = 20;
    
  } while (c != KEY_ESC);
  
  closegraph ();
  return 0;
  
}
