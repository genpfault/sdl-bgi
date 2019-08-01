/* multiwin.c  -*- C -*-
 * 
 * To compile:
 * gcc -o multiwin multiwin.c -lSDL_bgi -lSDL2
 * 
 * By Guido Gonzato, July 2018.
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

#include <graphics.h>

int main (int argc, char *argv[])
{

  int gd, gm = SDL_320x200;
  int win1, win2, win3, x, y;

  setwinoptions ("First Window", 10, 10, -1);
  initgraph (&gd, &gm, "");
  cleardevice ();
  outtextxy (0, 0, "Click or press a key for a SECOND window");
  circle (50, 100, 50);
  win1 = getcurrentwindow ();
  printf ("Current window: %d\n", win1);
  x = getmaxx () - 110;
  y = getmaxy () - 60;
  readimagefile ("logo.bmp", x, y, x + 100, y + 50);
  getevent ();
  delay (500);
  
  setwinoptions ("Second Window", 10, 200 + 50, -1);
  initgraph (&gd, &gm, "");
  cleardevice ();
  setcolor (RED);
  outtextxy (0, 0, "Click or press a key for a THIRD window");
  circle (100, 100, 50);
  win2 = getcurrentwindow ();
  printf ("Current window: %d\n", win2);
  readimagefile ("logo.bmp", x, y, x + 100, y + 50);
  getevent ();
  delay (500);

  gm = SDL_640x480;
  setwinoptions ("Third Window", 320 + 50, 10, -1);
  initgraph (&gd, &gm, "");
  cleardevice ();
  setcolor (GREEN);
  outtextxy (0, 0, "Press a key to switch back to the FIRST window");
  circle (150, 100, 50);
  win3 = getcurrentwindow ();
  printf ("Current window: %d\n", win3);
  x = getmaxx () - 110;
  y = getmaxy () - 60;
  readimagefile ("logo.bmp", x, y, x + 100, y + 50);
  getevent ();
  delay (500);
  
  setcurrentwindow (win1);
  setcolor (WHITE);
  circle (150, 100, 50);
  win1 = getcurrentwindow ();
  printf ("Current window: %d\n", win1);
  outtextxy (0, 10, "Back to the FIRST window; press a key");
  getevent ();
  outtextxy (0, 20, "Shutting down...");
  
  delay (1000);
  printf ("Shutting down window %d\n", win3);
  closewindow (win3);
  delay (1000);
  printf ("Shutting down window %d\n", win2);
  closewindow (win2);
  delay (1000);
  printf ("Shutting down window %d\n", win1);
  closewindow (win1);
  
  return 0;
  
}

// ----- end of file multiwin.c
