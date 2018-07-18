/* moveit.c  -*- C -*-
 * 
 * To compile:
 * gcc -o moveit moveit.c -lSDL_bgi -lSDL2
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
#include <string.h>
#include <math.h>
#include <graphics.h>

// -----

int main (int argc, char *argv[])
{

  int c, gd, gm, x, y, stop = NOPE;
  
  gd = SDL;
  gm = SDL_800x600;
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
    // Mingw64 bug - use getevent() instead
    // c = getch ();
    c = getevent ();
    printf ("Key code: %d\n", c);
    
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
      ;
    }
    
    if (x < 0)
      x = getmaxx ();
    if (x > getmaxx ())
      x = 0;
    if (y < 20)
      y = getmaxy ();
    if (y > getmaxy ())
      y = 20;
    
    if (KEY_ESC == c || QUIT == c)
      stop = YEAH;
    
  } while (!stop);
  
  closegraph ();
  return 0;
  
} // main ()

// ----- end of file moveit.c

