/* simple.c  -*- C -*-
 * 
 * To compile:
 * gcc -o simple simple.c -lSDL_bgi -lSDL2
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

int i, stop, maxx, maxy;

// -----

void message (char *str)
{
  cleardevice ();
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 2);
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  setcolor (RED);
  outtextxy (maxx / 2, maxy / 2, str);
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 1);
  setcolor (YELLOW);
  outtextxy (maxx / 2, maxy - 20, "Press a key to continue");
  getevent ();
  cleardevice ();
  settextjustify (LEFT_TEXT, TOP_TEXT);

} // message ()

// -----

void circles (void)
{
  stop = 0;
  
  while (! stop) {
    setcolor (YELLOW);
    outtextxy (0, 0, "Press a key to continue");
    for (i = 0; i < 300; i++) {
      setcolor (BLACK + random (MAXCOLORS));
      circle  (random (maxx), random(maxy), random (100));
    }
    refresh ();
    delay (1000);
    cleardevice ();
    if (kbhit ())
      stop = 1;
  } // while

} // circles ()

// ---

void lines (void)
{
  stop = 0;
  
  while (! stop) {
    setcolor (YELLOW);
    outtextxy (0, 0, "Press a key to continue");
    for (i = 0; i < 500; i++) {
      setlinestyle (SOLID_LINE, 0, NORM_WIDTH);
      setcolor (BLACK + random (MAXCOLORS));
      line (random(maxx), random(maxy), random(maxx), random(maxy));
    }
    refresh ();
    delay (1000);
    cleardevice ();
    if (kbhit ())
      stop = 1;
  } // while

} // lines ()

// -----

void pixels (void)
{
  int col;
  stop = 0;
  
  while (! stop) {
    setcolor (YELLOW);
    outtextxy (0, 0, "Press a key to continue");
    srand (2015);
    for (i = 0; i < 300; i++) {
      col = BLACK + random (MAXCOLORS);
      putpixel (random(maxx), random(maxy), col);
    }
    refresh ();
    delay (1000);
    srand (2015);
    for (i = 0; i < 300; i++) {
      col = 1 + random (MAXCOLORS); /* keep random () in sync */
      putpixel (random(maxx), random(maxy), BLACK);
      if (kbhit ())
	stop = 1;
    }
    refresh ();
    delay (1000);
    cleardevice ();
  } // while
 
} // pixels ()

// -----

int main (int argc, char *argv[])
{

  int gd, gm;
  
  gd = DETECT;
  initgraph (&gd, &gm, "");
  maxx = getmaxx ();
  maxy = getmaxy ();
  setbkcolor (BLACK);
  cleardevice ();
  
  message ("Drawing in SLOW mode:");
  sdlbgislow ();

  circles ();
  lines ();
  pixels ();
  
  message ("Drawing in FAST mode:");
  sdlbgifast ();
  
  circles ();
  lines ();
  pixels ();
  
  // getevent ();
  closegraph ();
  
}

// ----- end of file simple.c
