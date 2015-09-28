/* simple.c  -*- C -*-
 * 
 * To compile:
 * gcc -o simple simple.c SDL_bgi.c -lSDL -lSDL_gfx -lm
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
#include <graphics.h>

/* ----- */

int main ()
{

  int i, x;
  
  initwindow (800, 600);
  sdlbgislow ();
  
  setbkcolor (BLUE);
  cleardevice ();
  for (i = 0; i < 1000; i++)
    putpixel (random (800), random (600), YELLOW);
  
  refresh ();
  delay (2000);
  closegraph ();
  return 0;
  
}
