/*
 * turtle.h	-*- C -*-
 * 
 * turtle graphics for BGI-compatible libraries
 * Tested with xbgi, SDL_bgi and grx
 * 
 * By Guido Gonzato <guido.gonzato at gmail.com>
 * January 2013
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

void back (int);              /* move backwards */
void forwd (int);             /* move forwards */
int  heading (void);          /* return the turtle heading (angle) */
void hideturtle (void);       /* hide the turtle */
void home (void);             /* move the turtle to the screen centre */
void pendown (void);          /* activate drawing */
void penup (void);            /* deactivate drawing */
void setheading (int);        /* set the turtle heading */
void setposition (int, int);  /* move the turtle to new coordinates */
void showturtle (void);       /* show the turtle */
void turnleft (int);          /* turs the turtle left */
void turnright (int);         /* turn the turtle right */
void wrap (void);             /* wrap around the window */
void nowrap (void);           /* don't wrap around the window */
int  xcor (void);             /* return the turtle's X */
int  ycor (void);             /* return the turtle's Y */

#define  T_TRUE  1
#define  T_FALSE 0

#define  T_NORTH 0
#define  T_EAST  90
#define  T_SOUTH 180
#define  T_WEST  270

// ----- end of file turtle.h
