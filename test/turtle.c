/*
 * turtle.c	-*- C -*-
 * 
 * turtle graphics for BGI-compatible libraries
 * Tested with SDL_bgi, xbgi, and grx
 * 
 * By Guido Gonzato <guido.gonzato at gmail.com>
 * Latest update: October 2018
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

#include "turtle.h"

// -----

static int t_pos[2];                    // turtle coordinates
static int t_heading = 0;               // turtle heading

static int t_tmp_pos[2] = {-1, -1};     // saved turtle coordinates
static int t_tmp_heading = 0;           // saved turtle heading

static int t_show_turtle = T_FALSE;     // draw the turtle?
static int t_pen_down = T_TRUE;         // draw?
static int t_turtle_drawn = T_TRUE;     // has the turtle been drawn?
static int t_turtle_wrap = T_FALSE;     // wrap around the window?
static int t_turtle_size = 21;          // turtle size in pixels
static int t_turtle_shape = T_TRIANGLE; // turtle size in pixels

static void draw_turtle (void);

// sine and cosine tables for polar graphics. The angle
// starts at pi/2 and increases clockwise: -(angle-90)

static double t_sin[360] =
{ 1.000000,  0.999848,  0.999391,  0.998630,  0.997564,  
  0.996195,  0.994522,  0.992546,  0.990268,  0.987688,  
  0.984808,  0.981627,  0.978148,  0.974370,  0.970296,  
  0.965926,  0.961262,  0.956305,  0.951057,  0.945519,  
  0.939693,  0.933580,  0.927184,  0.920505,  0.913545,  
  0.906308,  0.898794,  0.891007,  0.882948,  0.874620,  
  0.866025,  0.857167,  0.848048,  0.838671,  0.829038,  
  0.819152,  0.809017,  0.798636,  0.788011,  0.777146,  
  0.766044,  0.754710,  0.743145,  0.731354,  0.719340,  
  0.707107,  0.694658,  0.681998,  0.669131,  0.656059,  
  0.642788,  0.629320,  0.615661,  0.601815,  0.587785,  
  0.573576,  0.559193,  0.544639,  0.529919,  0.515038,  
  0.500000,  0.484810,  0.469472,  0.453990,  0.438371,  
  0.422618,  0.406737,  0.390731,  0.374607,  0.358368,  
  0.342020,  0.325568,  0.309017,  0.292372,  0.275637,  
  0.258819,  0.241922,  0.224951,  0.207912,  0.190809,  
  0.173648,  0.156434,  0.139173,  0.121869,  0.104528,  
  0.087156,  0.069756,  0.052336,  0.034899,  0.017452,  
  0.000000,  -0.017452,  -0.034899,  -0.052336,  -0.069756,  
  -0.087156,  -0.104528,  -0.121869,  -0.139173,  -0.156434,  
  -0.173648,  -0.190809,  -0.207912,  -0.224951,  -0.241922,  
  -0.258819,  -0.275637,  -0.292372,  -0.309017,  -0.325568,  
  -0.342020,  -0.358368,  -0.374607,  -0.390731,  -0.406737,  
  -0.422618,  -0.438371,  -0.453990,  -0.469472,  -0.484810,  
  -0.500000,  -0.515038,  -0.529919,  -0.544639,  -0.559193,  
  -0.573576,  -0.587785,  -0.601815,  -0.615661,  -0.629320,  
  -0.642788,  -0.656059,  -0.669131,  -0.681998,  -0.694658,  
  -0.707107,  -0.719340,  -0.731354,  -0.743145,  -0.754710,  
  -0.766044,  -0.777146,  -0.788011,  -0.798636,  -0.809017,  
  -0.819152,  -0.829038,  -0.838671,  -0.848048,  -0.857167,  
  -0.866025,  -0.874620,  -0.882948,  -0.891007,  -0.898794,  
  -0.906308,  -0.913545,  -0.920505,  -0.927184,  -0.933580,  
  -0.939693,  -0.945519,  -0.951057,  -0.956305,  -0.961262,  
  -0.965926,  -0.970296,  -0.974370,  -0.978148,  -0.981627,  
  -0.984808,  -0.987688,  -0.990268,  -0.992546,  -0.994522,  
  -0.996195,  -0.997564,  -0.998630,  -0.999391,  -0.999848,  
  -1.000000,  -0.999848,  -0.999391,  -0.998630,  -0.997564,  
  -0.996195,  -0.994522,  -0.992546,  -0.990268,  -0.987688,  
  -0.984808,  -0.981627,  -0.978148,  -0.974370,  -0.970296,  
  -0.965926,  -0.961262,  -0.956305,  -0.951057,  -0.945519,  
  -0.939693,  -0.933580,  -0.927184,  -0.920505,  -0.913545,  
  -0.906308,  -0.898794,  -0.891007,  -0.882948,  -0.874620,  
  -0.866025,  -0.857167,  -0.848048,  -0.838671,  -0.829038,  
  -0.819152,  -0.809017,  -0.798636,  -0.788011,  -0.777146,  
  -0.766044,  -0.754710,  -0.743145,  -0.731354,  -0.719340,  
  -0.707107,  -0.694658,  -0.681998,  -0.669131,  -0.656059,  
  -0.642788,  -0.629320,  -0.615661,  -0.601815,  -0.587785,  
  -0.573576,  -0.559193,  -0.544639,  -0.529919,  -0.515038,  
  -0.500000,  -0.484810,  -0.469472,  -0.453990,  -0.438371,  
  -0.422618,  -0.406737,  -0.390731,  -0.374607,  -0.358368,  
  -0.342020,  -0.325568,  -0.309017,  -0.292372,  -0.275637,  
  -0.258819,  -0.241922,  -0.224951,  -0.207912,  -0.190809,  
  -0.173648,  -0.156434,  -0.139173,  -0.121869,  -0.104528,  
  -0.087156,  -0.069756,  -0.052336,  -0.034899,  -0.017452,  
  -0.000000,  0.017452,  0.034899,  0.052336,  0.069756,  
  0.087156,  0.104528,  0.121869,  0.139173,  0.156434,  
  0.173648,  0.190809,  0.207912,  0.224951,  0.241922,  
  0.258819,  0.275637,  0.292372,  0.309017,  0.325568,  
  0.342020,  0.358368,  0.374607,  0.390731,  0.406737,  
  0.422618,  0.438371,  0.453990,  0.469472,  0.484810,  
  0.500000,  0.515038,  0.529919,  0.544639,  0.559193,  
  0.573576,  0.587785,  0.601815,  0.615661,  0.629320,  
  0.642788,  0.656059,  0.669131,  0.681998,  0.694658,  
  0.707107,  0.719340,  0.731354,  0.743145,  0.754710,  
  0.766044,  0.777146,  0.788011,  0.798636,  0.809017,  
  0.819152,  0.829038,  0.838671,  0.848048,  0.857167,  
  0.866025,  0.874620,  0.882948,  0.891007,  0.898794,  
  0.906308,  0.913545,  0.920505,  0.927184,  0.933580,  
  0.939693,  0.945519,  0.951057,  0.956305,  0.961262,  
  0.965926,  0.970296,  0.974370,  0.978148,  0.981627,  
  0.984808,  0.987688,  0.990268,  0.992546,  0.994522,  
  0.996195,  0.997564,  0.998630,  0.999391,  0.999848 };
// t_sin[360]
  
static double t_cos[360] =
{0.000000,  0.017452,  0.034899,  0.052336,  0.069756,  
  0.087156,  0.104528,  0.121869,  0.139173,  0.156434,  
  0.173648,  0.190809,  0.207912,  0.224951,  0.241922,  
  0.258819,  0.275637,  0.292372,  0.309017,  0.325568,  
  0.342020,  0.358368,  0.374607,  0.390731,  0.406737,  
  0.422618,  0.438371,  0.453990,  0.469472,  0.484810,  
  0.500000,  0.515038,  0.529919,  0.544639,  0.559193,  
  0.573576,  0.587785,  0.601815,  0.615661,  0.629320,  
  0.642788,  0.656059,  0.669131,  0.681998,  0.694658,  
  0.707107,  0.719340,  0.731354,  0.743145,  0.754710,  
  0.766044,  0.777146,  0.788011,  0.798636,  0.809017,  
  0.819152,  0.829038,  0.838671,  0.848048,  0.857167,  
  0.866025,  0.874620,  0.882948,  0.891007,  0.898794,  
  0.906308,  0.913545,  0.920505,  0.927184,  0.933580,  
  0.939693,  0.945519,  0.951057,  0.956305,  0.961262,  
  0.965926,  0.970296,  0.974370,  0.978148,  0.981627,  
  0.984808,  0.987688,  0.990268,  0.992546,  0.994522,  
  0.996195,  0.997564,  0.998630,  0.999391,  0.999848,  
  1.000000,  0.999848,  0.999391,  0.998630,  0.997564,  
  0.996195,  0.994522,  0.992546,  0.990268,  0.987688,  
  0.984808,  0.981627,  0.978148,  0.974370,  0.970296,  
  0.965926,  0.961262,  0.956305,  0.951057,  0.945519,  
  0.939693,  0.933580,  0.927184,  0.920505,  0.913545,  
  0.906308,  0.898794,  0.891007,  0.882948,  0.874620,  
  0.866025,  0.857167,  0.848048,  0.838671,  0.829038,  
  0.819152,  0.809017,  0.798636,  0.788011,  0.777146,  
  0.766044,  0.754710,  0.743145,  0.731354,  0.719340,  
  0.707107,  0.694658,  0.681998,  0.669131,  0.656059,  
  0.642788,  0.629320,  0.615661,  0.601815,  0.587785,  
  0.573576,  0.559193,  0.544639,  0.529919,  0.515038,  
  0.500000,  0.484810,  0.469472,  0.453990,  0.438371,  
  0.422618,  0.406737,  0.390731,  0.374607,  0.358368,  
  0.342020,  0.325568,  0.309017,  0.292372,  0.275637,  
  0.258819,  0.241922,  0.224951,  0.207912,  0.190809,  
  0.173648,  0.156434,  0.139173,  0.121869,  0.104528,  
  0.087156,  0.069756,  0.052336,  0.034899,  0.017452,  
  0.000000,  -0.017452,  -0.034899,  -0.052336,  -0.069756,  
  -0.087156,  -0.104528,  -0.121869,  -0.139173,  -0.156434,  
  -0.173648,  -0.190809,  -0.207912,  -0.224951,  -0.241922,  
  -0.258819,  -0.275637,  -0.292372,  -0.309017,  -0.325568,  
  -0.342020,  -0.358368,  -0.374607,  -0.390731,  -0.406737,  
  -0.422618,  -0.438371,  -0.453990,  -0.469472,  -0.484810,  
  -0.500000,  -0.515038,  -0.529919,  -0.544639,  -0.559193,  
  -0.573576,  -0.587785,  -0.601815,  -0.615661,  -0.629320,  
  -0.642788,  -0.656059,  -0.669131,  -0.681998,  -0.694658,  
  -0.707107,  -0.719340,  -0.731354,  -0.743145,  -0.754710,  
  -0.766044,  -0.777146,  -0.788011,  -0.798636,  -0.809017,  
  -0.819152,  -0.829038,  -0.838671,  -0.848048,  -0.857167,  
  -0.866025,  -0.874620,  -0.882948,  -0.891007,  -0.898794,  
  -0.906308,  -0.913545,  -0.920505,  -0.927184,  -0.933580,  
  -0.939693,  -0.945519,  -0.951057,  -0.956305,  -0.961262,  
  -0.965926,  -0.970296,  -0.974370,  -0.978148,  -0.981627,  
  -0.984808,  -0.987688,  -0.990268,  -0.992546,  -0.994522,  
  -0.996195,  -0.997564,  -0.998630,  -0.999391,  -0.999848,  
  -1.000000,  -0.999848,  -0.999391,  -0.998630,  -0.997564,  
  -0.996195,  -0.994522,  -0.992546,  -0.990268,  -0.987688,  
  -0.984808,  -0.981627,  -0.978148,  -0.974370,  -0.970296,  
  -0.965926,  -0.961262,  -0.956305,  -0.951057,  -0.945519,  
  -0.939693,  -0.933580,  -0.927184,  -0.920505,  -0.913545,  
  -0.906308,  -0.898794,  -0.891007,  -0.882948,  -0.874620,  
  -0.866025,  -0.857167,  -0.848048,  -0.838671,  -0.829038,  
  -0.819152,  -0.809017,  -0.798636,  -0.788011,  -0.777146,  
  -0.766044,  -0.754710,  -0.743145,  -0.731354,  -0.719340,  
  -0.707107,  -0.694658,  -0.681998,  -0.669131,  -0.656059,  
  -0.642788,  -0.629320,  -0.615661,  -0.601815,  -0.587785,  
  -0.573576,  -0.559193,  -0.544639,  -0.529919,  -0.515038,  
  -0.500000,  -0.484810,  -0.469472,  -0.453990,  -0.438371,  
  -0.422618,  -0.406737,  -0.390731,  -0.374607,  -0.358368,  
  -0.342020,  -0.325568,  -0.309017,  -0.292372,  -0.275637,  
  -0.258819,  -0.241922,  -0.224951,  -0.207912,  -0.190809,  
  -0.173648,  -0.156434,  -0.139173,  -0.121869,  -0.104528,  
  -0.087156,  -0.069756,  -0.052336,  -0.034899,  -0.017452};
// t_cos[360]

// -----

void back (int n)
{
  turnleft (180);
  forwd (n);
  turnleft (180);
}

inline void backward (int n)
{
  back (n);
}

inline void bk (int n)
{
  back (n) ;
}

// -----

void forwd (int n)
{
  int newx, newy;
  
  newx = (int) ((double) n * t_cos[t_heading]);
  newy = (int) ((double) n * t_sin[t_heading]);
  
  // is the turtle visible?
  if (t_show_turtle)
    draw_turtle (); // delete previous
  
  // should we draw?
  if (t_pen_down)
    line (t_pos[0], t_pos[1], t_pos[0] + newx, t_pos[1] - newy);

  t_pos[0] += newx;
  t_pos[1] -= newy;
  
  // is wrapping active?
  if (t_turtle_wrap) {
    
    if (t_pos[0] < 0)
      t_pos[0] = getmaxx () + t_pos[0];
    
    if (t_pos[0] > getmaxx ())
      t_pos[0] = t_pos[0] - getmaxx ();
    
    if (t_pos[1] < 0)
      t_pos[1] = getmaxy () + t_pos[1];
    
    if (t_pos[1] > getmaxy ())
      t_pos[1] = t_pos[1] - getmaxy ();
    
  } // t_turtle_wrap
  
  // new position
  if (t_show_turtle)
    draw_turtle ();
  
} // forwd

inline void forward (int n)
{
  forwd (n);
}

inline void fd (int n)
{
  forwd (n);
}

// -----

void turnleft (int n)
{
  // delete the turtle
  if (t_show_turtle)
    draw_turtle ();
  t_heading -= n;
  if (t_heading < 0)
    t_heading += 360;
  if (t_show_turtle)
    draw_turtle ();
}

inline void left (int n)
{
  turnleft (n);
}

inline void lt (int n)
{
  turnleft (n);
}

// -----

void turnright (int n)
{
  // delete the turtle
  if (t_show_turtle)
    draw_turtle ();
  t_heading += n;
  if (t_heading >= 360)
    t_heading -= 360;
  if (t_show_turtle)
    draw_turtle ();
}

inline void right (int n)
{
  turnright (n);
}

inline void rt (int n)
{
  turnright (n);
}

// -----

void setposition (int x, int y)
{
  t_pos[0] = x;
  t_pos[1] = y;
}

inline void go_to (int x, int y)
{
  setposition (x, y);
}

inline void setpos (int x, int y)
{
  setposition (x, y);
}

// -----

void setx (int x)
{
  t_pos[0] = x;
}

// -----

void sety (int y)
{
  t_pos[1] = y;
}

// -----

void setheading (int angle)
{
  t_heading = angle % 360; // avoid overflows
}

inline void seth (int angle)
{
  setheading (angle);
}

// -----

void home (void)
{
  t_pos[0] = getmaxx () / 2;
  t_pos[1] = getmaxy () / 2;
  setheading (0);
}

// -----

int *position (void)
{
  return t_pos;
}

// -----

int xcor (void)
{
  return (t_pos[0]);
}

// -----

int ycor (void)
{
  return (t_pos[1]);
}

// -----

int heading (void)
{
  return (t_heading);
}

// -----

void savestate (void)
{
  t_tmp_pos[0] = t_pos[0];
  t_tmp_pos[1] = t_pos[1];
  t_tmp_heading = t_heading;
}

// -----

void restorestate (void)
{
  // has the state been saved?
  
  if (-1 != t_tmp_pos[0]) {
    t_pos[0] = t_tmp_pos[0];
    t_pos[1] = t_tmp_pos[1];
    t_heading = t_tmp_heading;
  }
  else
    home ();
}

// -----

void pendown (void)
{
  t_pen_down = T_TRUE;
}

inline void pd (void)
{
  pendown ();
}

inline void down (void)
{
  pendown ();
}

// -----

void penup (void)
{
  t_pen_down = T_FALSE;
}

inline void pu (void)
{
  penup ();
}

inline void up (void)
{
  penup ();
}

// -----

int isdown (void)
{
  return t_pen_down;
}

// -----

void turtleshape (int shape)
{
  t_turtle_shape = shape;
}

// -----

void draw_turtle_circle (void)
{
  int col = getcolor ();
  if (RED != getbkcolor ())
    setcolor (RED);
  else
    setcolor (WHITE);
  circle (t_pos[0], t_pos[1], t_turtle_size);
  setcolor (col);

}

// -----

void draw_turtle_triangle (void)
{
  // draws a triangular turtle. 
  
  int triangle[6];
  int col;
  
  triangle[0] = t_pos[0] +
    (int) ((double) t_turtle_size * t_cos[t_heading]);
  triangle[1] = t_pos[1] -
    (int) ((double) t_turtle_size * t_sin[t_heading]);
  
  t_heading -= 90;
  if (t_heading < 0)
    t_heading += 360;
  
  triangle[2] = t_pos[0] +
    (int) ((double) t_turtle_size/3 * t_cos[t_heading]);
  triangle[3] = t_pos[1] -
    (int) ((double) t_turtle_size/3 * t_sin[t_heading]);
  
  t_heading -= 180;
  if (t_heading < 0)
    t_heading += 360;
  
  triangle[4] = t_pos[0] +
    (int) ((double) t_turtle_size/3 * t_cos[t_heading]);
  triangle[5] = t_pos[1] -
    (int) ((double) t_turtle_size/3 * t_sin[t_heading]);
  
  // draw it
  col = getcolor ();
  if (RED != getbkcolor ())
    setcolor (RED);
  else
    setcolor (WHITE);
  line (triangle[0], triangle[1], triangle[2], triangle[3]);
  line (triangle[2], triangle[3], triangle[4], triangle[5]);
  line (triangle[4], triangle[5], triangle[0], triangle[1]);
  setcolor (col);
  
}

// -----

void draw_turtle (void)
{
  // int triangle[6];
  int tmp_heading = t_heading;
    
  setwritemode (XOR_PUT);
  
  switch (t_turtle_shape) {

  case T_CIRCLE:
    draw_turtle_circle ();
    break;
    
  case T_TRIANGLE:
  
  default:
    draw_turtle_triangle ();
    ;
    
  }

  setwritemode (COPY_PUT);
  t_turtle_drawn = T_TRUE;
  t_heading = tmp_heading;
}

// -----

void showturtle (void)
{
  draw_turtle ();
  t_show_turtle = T_TRUE;
}

inline void st (void)
{
  showturtle ();
}

// -----

void hideturtle (void)
{
  draw_turtle ();
  t_show_turtle = T_FALSE;
}

inline void ht (void)
{
  hideturtle ();
}

// -----

void turtlesize (int size)
{
  t_turtle_size = size;
}

// -----

int isvisible (void)
{
  return t_show_turtle;
}

// -----

void wrap (void)
{
  t_turtle_wrap = T_TRUE;
}

// -----

void nowrap (void)
{
  t_turtle_wrap = T_FALSE;
}

// ----- end of file turtle.c
