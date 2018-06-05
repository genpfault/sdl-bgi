/* mousetest.c  -*- C -*-
 * 
 * To compile:
 * gcc -o mousetest mousetest.c -lSDL_bgi -lSDL2
 * 
 * By Guido Gonzato, May 2015
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

  int gd, gm, p, x, y;
  char s[] = "Click around (or here to exit)";
  int len;
  
  gd = SDL;
  gm = SDL_1024x768;
  initgraph (&gd, &gm, "");
  setbkcolor (BLACK);
  setcolor (RED);
  setviewport (10, 10, getmaxx() - 10, getmaxy() - 10, 1);
  
  clearviewport ();
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  outtextxy (getmaxx() / 2, getmaxy () / 2, 
	     "Click the RIGHT button to begin");
  
  while (!ismouseclick (WM_RBUTTONDOWN))
    ;
  getmouseclick(WM_RBUTTONDOWN, &x, &y);
  clearviewport ();
  rectangle (0, 0, getmaxx () - 20, getmaxy () - 20);
  
  len = strlen(s) * 8; // assumes an 8x8 font
  rectangle (0, 0, len, 16);
  settextjustify (LEFT_TEXT, TOP_TEXT);
  outtextxy (0, 4, s);
  setlinestyle (SOLID_LINE, 0, THICK_WIDTH);
  
  // while (!kbhit ()) {
  while (1) {
    
    p = mouseclick ();
    
    if (p > 0) {
      
      if ( (mousex () < len) && (mousey () < 16))
	break;
    
      if (WM_MOUSEMOVE == p)
	putpixel (mousex (), mousey (), getcolor ());
      else { // mouse clicks
	setfillstyle (1, // random(WHITE));
		      COLOR (random(255), random(255), random(255)));
	setcolor (COLOR (random(255), random(255), random(255)));
	if (0 == random (2))
	  fillellipse (mousex (), mousey (),
		       5 + p*random(15), 5 + p*random(15));
	else
	  bar (mousex () - p*10, mousey () - p*10,
	       mousex () + p*10, mousey () + p*10);
      } // else
    }
    
  } // while

  closegraph ();
  return 0;
  
} // main ()

// ----- end of file mousetest.c

