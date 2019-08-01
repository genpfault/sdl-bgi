/* floodfilltest.c  -*- C -*-
 * 
 * To compile:
 * gcc -o floodfilltest floodfilltest.c -lSDL_bgi -lSDL2
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
#include <graphics.h>

// -----

int main (int argc, char *argv[])
{

  int
    stop = 0,
    p;
  
  initwindow (800, 600);
  setbkcolor (BLACK);
  setcolor (RED);
  cleardevice ();
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  
  outtextxy (getmaxx() / 2, getmaxy () / 2 - 15, 
	     "Left click to draw a random circle;");
  outtextxy (getmaxx() / 2, getmaxy () / 2, 
	     "right click to fill;");
  outtextxy (getmaxx() / 2, getmaxy () / 2 + 15, 
	     "press a key to exit.");
  refresh ();
  getevent ();
  cleardevice ();
  setcolor (YELLOW);

  while (! stop) {
  
    if ((p = mouseclick ())) {
      if (WM_LBUTTONDOWN == p) {
	setcolor (YELLOW);
	circle (mousex (), mousey (), random (100));
	// added!
	refresh ();
      }
      else 
	if (WM_RBUTTONDOWN == p) {
	  setcolor (random (MAXCOLORS));
	  setfillstyle (1 + random(USER_FILL),
			COLOR (random (255), random (255), random (255)));
	  floodfill (mousex (), mousey (), YELLOW);
	  refresh ();
	}
    }
    if (xkbhit ())
      stop = 1;
    
  } // while

  closegraph ();
  return 0;
  
}

// ----- end of file floodfilltest.c
