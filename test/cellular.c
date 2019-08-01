/* cellular.c  -*- C -*-
 * 
 * To compile:
 * gcc -o cellular cellular.c -lSDL_bgi -lSDL2
 * 
 * Simple cellular automata, as described at
 * http://mathworld.wolfram.com/ElementaryCellularAutomaton.html
 * 
 * Run this program as:
 * 
 * ./cellular [rule]
 * 
 * where [rule] is a cellular automaton rule (1..255).
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
#include <math.h>
#include <time.h>
#include <graphics.h>

// -----

void run_rule (int rule)
{
  int maxx, pixel;
  
  maxx = getmaxx ();

  // for an explanation about rules, please see
  // http://mathworld.wolfram.com/CellularAutomaton.html
  
  // loop: for every pixel, run the rule
  for (int y = 20; y < 20 + maxx / 2; y++)
    for (int x = 1; x < maxx - 1; x++) {
      
      pixel = 0;
      
      // check the neighbour pixels on the line above
      // and take note of the ones that are RED
      if (RED == getpixel (x - 1, y - 1))
	pixel |= 4; // same as pixel += 4;
      if (RED == getpixel (x, y - 1))
	pixel |= 2;
      if (RED == getpixel (x + 1, y - 1))
	pixel |= 1;
      
      // for every possible state (3 bits: 0-7), if the
      // corresponding bit in the rule is 1, then
      // plot the pixel at (x, y)
      
      for (int i = 0; i < 8; i++)
	if ((rule << i) & 0x80) // the bit in the rule is set to 1
	  if (pixel == 7 - i)   // 'pixel' matches the rule
	    putpixel (x, y, RED);
	  
    } // for x
}

// -----

int main (int argc, char *argv[])
{
  
  int 
    rule, 
    ev,
    stop = NOPE;
  char s[50];

  // set window title and position
  setwinoptions ("Cellular Automata", 100, 100, -1);
  initwindow (1024, 1024 / 2 + 40);
  setbkcolor (WHITE);
  setcolor (RED);
  cleardevice ();
  
  if (2 == argc)
    rule = atoi (argv [1]);
  else {
    srand (time(NULL));
    rule = random (255);
  }

  while (! stop) {
    sprintf (s, "Click to continue - Rule: %d", rule);
    outtextxy (0, 0, s);
    putpixel (getmaxx () / 2, 20, RED); // set initial pixel
    run_rule (rule);
    refresh ();
    // srand (time(NULL));
    rule = random (255);
    
    ev = getevent();
    if (KEY_ESC == ev || WM_RBUTTONDOWN == ev || QUIT == ev)
      stop = YEAH;
    cleardevice ();
  }

  closegraph ();
  return 0;
}

// ----- end of file cellular.c
