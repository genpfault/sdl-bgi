/* fern.c  -*- C -*-
 * 
 * To compile:
 * gcc -o fern fern.c -lSDL_bgi -lSDL2
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
#include <unistd.h>
#include <time.h>
#include <math.h>

/* graphics.h could be a link to SDL_bgi.h
 * or another implementation. Let's write this program
 * in a portable way.
 */

#include <graphics.h>

/* ----- */

int main (int argc, char *argv[])
{
#ifndef _SDL_BGI_H
  int gd, gm;
#endif
  int stop = 0;
  long int counter = 0;
  int midx, midy, scale;
  int k = 0, prob;
  float x, y, xx, yy;
  float
    a[4] = {0.0, 0.85, 0.2, -0.15},
    b[4] = {0.0, 0.04, -0.26, 0.28},
    c[4] = {0.0, 0.04, 0.23, 0.26},
    d[4] = {0.16, 0.85, 0.22, 0.24},
    /* e[4] = {0.0, 0.0, 0.0, 0.0}, */
    f[4] = {0.0, 1.6, 1.6, 0.44};
    /* p[4] = {0.01, 0.85, 0.07, 0.07}; */

#ifdef _SDL_BGI_H
  setwinoptions ("This is a fractal fern",
		 SDL_WINDOWPOS_CENTERED,
		 SDL_WINDOWPOS_CENTERED,
		 -1);
  initgraph (NULL, NULL, "");
#else
  initgraph (&gd, &gm, "");
#endif
  srand (time(NULL));
  
#ifdef _SDL_BGI_H
  setbkcolor (COLOR (0, 0, 40));
#else
  setbkcolor (BLACK);
#endif
  cleardevice ();
  setcolor (YELLOW);
  outtextxy (0, 0, "Press a key to exit: ");

  midx = getmaxx () / 2;
  midy = getmaxy () / 2;
  scale = getmaxx () / 16;
  x = y = 0.0;
#ifdef _SDL_BGI_H
  setcolor (COLOR (random (256), random (256), random (256)));
#else
  setcolor (random (WHITE));
#endif
  
  /* kbhit (), mouseclick () and event () are slow. Use them sparingly. */
  
  while (! stop) {
    prob = 1 + random (100);
    if (prob == 1)
      k = 0;
    if ( (prob > 1) && (prob < 87))
      k = 1;
    if ( (prob > 86) && (prob < 94))
      k = 2;
    if (prob > 93)
      k = 3;
    
    /* to use equal probability, just use:
     * k = random (5);
     */
    xx = a[k] * x + b[k] * y; /*  + e[k]; */
    yy = c[k] * x + d[k] * y + f[k];
    x = xx;
    y = yy;
#ifdef _SDL_BGI_H
    _putpixel ((int)(midx + scale * x), (2 * midy - scale * y));
#else
    putpixel ((int)(midx + scale * x), (2 * midy - scale * y), getcolor ());
#endif
    counter++;
    if (0 == counter % 100000) {
#ifdef _SDL_BGI_H
      setcolor (COLOR (random (256), random (256), random (256)));
      refresh ();
#else
      setcolor (random (WHITE));
#endif
      /* let's check for an event only once in a while. */
      if (kbhit ())
	stop = 1;
    }
    if (10000000 == counter) /* 10 million pixels */
      break;
  }
  setcolor (GREEN);
  outtextxy (0, 10, "Ok, leaving in 2 seconds");
  delay (2000);
  closegraph ();
  return 0;
}

/* --  end of file fern.c --- */
