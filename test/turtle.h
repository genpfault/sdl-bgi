/*
 * turtle.h	-*- C -*-
 * 
 * turtle graphics for BGI-compatible libraries
 * Tested with xbgi, SDL_bgi and grx
 * 
 * By Guido Gonzato <guido.gonzato at gmail.com>
 * February 2016
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

/* the following functions have aliases with the same name
 * as their Python counterparts; please see
 * https://docs.python.org/2/library/turtle.html
 */

/* turtle motion: move and draw */

void back (int);              /* move backwards */
void backward (int);
void bk (int);
void forwd (int);             /* move forwards */
void forwardd (int);
void fd (int);
void turnleft (int);          /* turn the turtle left */
void left (int);
void lt (int);
void turnright (int);         /* turn the turtle right */
void right (int);
void rt (int);
void setposition (int, int);  /* move the turtle to new coordinates */
void go_to (int, int);        /* "goto" is a reserved word in C! */
void setpos (int, int);
void setx (int);              /* set the X coordinate */
void sety (int);              /* set the Y coordinate */
void setheading (int);        /* set the turtle heading */
void seth (int);
void home (void);             /* move the turtle to the screen centre */

/* turtle's state */

int *position (void);         /* return the turtle's position */
int  xcor (void);             /* return the turtle's X */
int  ycor (void);             /* return the turtle's Y */
int  heading (void);          /* return the turtle heading (angle) */

/* pen control: drawing state */

void pendown (void);          /* activate drawing */
void pd (void);
void down (void);
void penup (void);            /* deactivate drawing */
void pu (void);
void up (void);
int  isdown (void);           /* is the pen down? */

/* turtle state and visibility */

void hideturtle (void);       /* hide the turtle */
void showturtle (void);       /* show the turtle */
void turtlesize (int);        /* set turtle size */
int  isvisible (void);        /* is the turtle visible? */
void wrap (void);             /* wrap around the window */
void nowrap (void);           /* don't wrap around the window */

#define  T_TRUE  1
#define  T_FALSE 0

#define  T_NORTH           0
#define  T_NORTHEAST      45
#define  T_EAST           90
#define  T_SOUTHEAST     135
#define  T_SOUTH         180
#define  T_SOUTHWEST     225
#define  T_WEST          270
#define  T_NORTWEST      315

#define  T_TWELVE    T_NORTH
#define  T_THREE      T_EAST
#define  T_SIX       T_SOUTH
#define  T_NINE       T_WEST

// ----- end of file turtle.h
