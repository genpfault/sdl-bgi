/* sdlbgidemo.c  -*- C -*-
 * 
 * To compile:
 * gcc -o sdlbgidemo sdlbgidemo.c -lSDL_bgi -lSDL2
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

#include <SDL2/SDL_bgi.h>

#define PI_CONV (3.1415926 / 180.0)

int 
  maxx, maxy,
  width, height;

int gd, gm;

struct viewporttype viewport;

void get_click (void);
void ket_key (void);
void pause (void);
void init_sdlbgi (void);
void message (char *str);
void mainwindow (void);

// -----

void get_click (void)
{
  
  while (! ismouseclick (WM_LBUTTONDOWN))
    ;
  while (ismouseclick (WM_LBUTTONDOWN))
    ;

} // get_click ()

// -----

void get_key (void)
{
  int 
    ch = getch ();
  
  if (KEY_ESC == ch) {
    closegraph ();
    exit (0);
  }

} // get_key ()

// -----

void pause (void)
{
  int
    stop = 0;
  
  while (! stop) {
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    if (ismouseclick (WM_RBUTTONDOWN)) {
      closegraph ();
      exit (1);
    }
  }
  
  while (ismouseclick (WM_LBUTTONDOWN))
    ;

} // pause ()

// -----

void init_sdlbgi (void)
{
  detectgraph (&gd, &gm);
  initgraph (&gd, &gm, "");
  sdlbgifast ();
  maxx = getmaxx ();
  maxy = getmaxy ();
} // initgraph ()

// -----

void message (char *str)
{
  setviewport (0, 0, maxx, maxy, 0);
  setbkcolor (COLOR (40, 0, 0));
  cleardevice ();
  setcolor (YELLOW);
  
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 1);
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  outtextxy (maxx / 2, 10, str);
  setcolor (WHITE);
  outtextxy (maxx / 2, maxy - 10,
	     "=== Left click to continue, right click to exit ===");
  refresh ();
} // message ()

// -----

void mainwindow (void)
{
  setcolor (WHITE);
  rectangle (39, 19, maxx - 39, maxy - 19);
  setviewport (40, 20, maxx - 40, maxy - 20, 1);
  setbkcolor (COLOR (0, 0, 20));
  clearviewport ();
  refresh ();
  width = viewport.right - viewport.left + 1;
  height = viewport.bottom - viewport.top + 1;
}

// -----

void sdlbgi_info (void)
{
  char
    str[1024];
  
  struct linesettingstype
    lineinfo;
  struct fillsettingstype
    fillinfo;
  struct textsettingstype
    textinfo;
  struct palettetype
    palette;
  
  Uint32
    time1, time2;

  char *driver, *mode;
  int i, x, y, dy;

  message ("Information on SDL_bgi Graphics");
  mainwindow ();
  
  setcolor (RED);
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 2);
  settextjustify (LEFT_TEXT, TOP_TEXT);
  
  getviewsettings (&viewport);
  getlinesettings (&lineinfo);
  getfillsettings (&fillinfo);
  gettextsettings (&textinfo);
  getpalette (&palette );
  driver = getdrivername ();
  mode = getmodename (gm);
  
  x = 150;
  y = 40; 
  dy = textheight ("X") * 3;
  
  outtextxy (x, y += dy, "Graphics device:");
  outtextxy (x, y += dy, "Graphics mode:");
  outtextxy (x, y += dy, "Screen resolution:");
  outtextxy (x, y += dy, "Current viewport:");
  outtextxy (x, y += dy, "Viewport clipping:");
  outtextxy (x, y += dy, "Current background colour:");
  outtextxy (x, y += dy, "Current foreground colour:");
  outtextxy (x, y += dy, "Current fill colour:");
  outtextxy (x, y += dy, "Character size:");
  outtextxy (x, y += dy, "Available pages:");
  
  x = 150 + textwidth ("Current background colour:   ");
  y = 40;
  
  setcolor (COLOR (240, 240, 0));
  sprintf (str, "%s", driver);
  outtextxy (x, y += dy, str);
  sprintf (str, "%s", mode);
  outtextxy (x, y += dy, str);
  sprintf (str, "%d x %d", maxx + 1, maxy + 1);
  outtextxy (x, y += dy, str);
  sprintf (str, "(%d, %d) - (%d, %d)", 
	   viewport.left, viewport.top, 
	   viewport.right, viewport.bottom);
  outtextxy (x, y += dy, str);
  sprintf (str, "%s", viewport.clip ? "ON" : "OFF");
  outtextxy (x, y += dy, str);
  sprintf (str, "%02X %02X %02X", 
	   RED_VALUE (getbkcolor ()),
	   GREEN_VALUE (getbkcolor ()),
	   BLUE_VALUE (getbkcolor ()));
  outtextxy (x, y += dy, str);
  sprintf (str, "%02X %02X %02X", 
	   RED_VALUE (getcolor ()),
	   GREEN_VALUE (getcolor ()),
	   BLUE_VALUE (getcolor ()));
  outtextxy (x, y += dy, str);
  sprintf (str, "%02X %02X %02X", 
	   RED_VALUE (fillinfo.color),
	   GREEN_VALUE (fillinfo.color),
	   BLUE_VALUE (fillinfo.color));
  outtextxy (x, y += dy, str);
  sprintf (str, "%d", textinfo.charsize);
  outtextxy (x, y += dy, str);
  sprintf (str, "%d", VPAGES);
  outtextxy (x, y += dy, str);

  refresh ();
  pause ();

} // sdlbgi_info ()

// -----

void colordemo (void)
{
  int
    stop = 0,
    i, x, y,
    col1, col2, col3,
    cnt = 0,
    rnd;
  
  message ("setcolor(COLOR(r,g,b)) Demonstration");
  mainwindow ();
  
  getviewsettings (&viewport);
  x = (viewport.right - viewport.left) / 2 - (256 * 3) / 2;
  y = (viewport.bottom - viewport.top) / 2;
  
  while (! stop) {
    
    // assign two RGB components, then vary the third
    col1 = random (256);
    col2 = random (256);
    
    for (col3 = 0; col3 < 256; col3++) {
      setcolor (COLOR (col1, col2, col3));
      for (i = 0; i < 3; i++) {
	line (x, y - maxy / 3, x, y + maxy / 3);
	x++;
      }
      if (ismouseclick (WM_RBUTTONDOWN)) {
	closegraph ();
	exit (1);
      }
    } // for
    
    refresh ();
    x = (viewport.right - viewport.left) / 2 - (256 * 3) / 2;
    
    delay (100);
    
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    
    if (ismouseclick (WM_RBUTTONDOWN)) {
      closegraph ();
      exit (1);
    }
  
  } // while
  
  refresh ();
  get_click ();

} // colordemo ()

// -----

void pixeldemo (void)
{
  int
    stop = 0,
    cnt = 0;
  
  message ("putpixel() Demonstration");
  mainwindow ();
  
  while (! stop) {
    
    setcolor (COLOR (random (256), random (256), random (256)));
    putpixel (random (maxx), random (maxy), getcolor ());
    cnt++;
    
   if (0 == cnt % 5000)
      refresh ();
    
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    
    if (ismouseclick (WM_RBUTTONDOWN)) {
      closegraph ();
      exit (1);
    }
  
  } // while
  
  refresh ();
  get_click ();

} // pixelslow ()

// -----

void linedemo (void)
{
  int
    stop = 0,
    x1, y1, x2, y2,
    cnt = 0,
    rnd;
  
  message ("line(), rectangle() Demonstration");
  mainwindow ();
  
  while (! stop) {
    
    rnd = random (2);
    setcolor (COLOR (random (256), random (256), random (256)));
    
    switch (rnd) {
      
    case 0:
      line (random (maxx), random (maxy),
	    random (maxx), random (maxy));
      break;
      
    case 1:
      x1 = random (viewport.right - viewport.left);
      y1 = random (viewport.bottom - viewport.top);
      x2 = x1 + random (viewport.right - viewport.left) / 4;
      y2 = y1 + random (viewport.bottom - viewport.top) / 4;
      rectangle (x1, y1, x2, y2);
      break;
      
    } // switch
    
    cnt++;
    if (0 == cnt % 200)
      refresh ();
    
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    
    if (ismouseclick (WM_RBUTTONDOWN)) {
      closegraph ();
      exit (1);
    }
  
  } // while
  
  refresh ();
  get_click ();

} // linedemo ()

// -----

#define MAXPOINTS 25
#define PI_CONV (3.1415926 / 180.0)

void linereldemo (void)
{
  int
    stop = 0,
    x, y,
    width, height,
    radius,
    i, j, npoints,
    pts_x[MAXPOINTS],
    pts_y[MAXPOINTS];
  
  double
    angle,
    step;
  
  message ("setlinestyle(), moveto(), lineto() Demonstration");
  mainwindow ();
  
  width = viewport.right - viewport.left + 1;
  height = viewport.bottom - viewport.top + 1;
  x = width / 2;
  y = height / 2;
  radius = height / 2 - 20;
  
  while (! stop) {

    npoints = 5 + random (MAXPOINTS - 5 + 1);
    if (npoints % 2)
      setlinestyle (0, 0, THICK_WIDTH);
    else
      setlinestyle (0, 0, NORM_WIDTH);
    clearviewport ();
    setcolor (BLUE + random (MAXCOLORS - 1));
    
    angle = 0.0;
    step = 360.0 / npoints;
    
    for (i= 0; i < npoints; i++) {
      pts_x[i] = x + (int)(cos (angle * PI_CONV) * radius);
      pts_y[i] = y - (int)(sin (angle * PI_CONV) * radius);
      angle += step;
    }

    circle (x, y, radius );

    for (i = 0; i < npoints; i++) {
      for (j = i; j < npoints; j++) {
	moveto (pts_x[i], pts_y[i]);
        lineto (pts_x[j], pts_y[j]);
      }
    }
    
    refresh ();
    delay (500);
    
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    
    if (ismouseclick (WM_RBUTTONDOWN)) {
      closegraph ();
      exit (1);
    }
  
  } // while
  
  refresh ();
  setlinestyle (0, 0, NORM_WIDTH);
  get_click ();

} // linedemo ()

// -----

void boxdemo (void)
{
  int
    stop = 0,
    x = maxx / 2,
    y = maxy / 2,
    cnt = 0,
    x1, y1, x2, y2;
  
  message ("bar() Demonstration");
  mainwindow ();
  
  while (! stop) {
    
    // fill colour
    setfillstyle (0, COLOR (random (256), random (256), random (256)));
    x1 = random (maxx);
    y1 = random (maxx);
    x2 = x1 + random (maxx / 4);
    y2 = y1 + random (maxy / 4);
    // outline colour
    setcolor (COLOR (random (256), random (256), random (256)));
    bar3d (x1, y1, x2, y2, 0, 0);
    
    cnt++;
    if (0 == cnt % 200)
      refresh ();
  
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    
    if (ismouseclick (WM_RBUTTONDOWN)) {
      closegraph ();
      exit (1);
    }
  
  } // while
  
  refresh ();
  get_click ();

} // boxdemo ()

// -----

void circledemo (void)
{
  int
    stop = 0,
    x = maxx / 2,
    y = maxy / 2,
    r = 100,
    cnt = 0,
    rnd;
  
  message ("arc(), circle(), ellipse() Demonstration");
  mainwindow ();
  
  while (! stop) {
    
    rnd = random (3);
    setcolor (COLOR (random (256), random (256), random (256)));
    
    switch (rnd) {
      
    case 0:
      arc (random (maxx), random (maxy),
	   random (360), random (360), random (r));
      break;
      
    case 1:
      circle (random (maxx), random (maxy), random (r));
      break;
      
    case 2:
      ellipse (random (maxx), random (maxy),
	       random (360), random (360), 
	       random (r), random (r));
      break;
      
    } // switch
    
    cnt++;
    if (0 == cnt % 200)
      refresh ();
    
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    
    if (ismouseclick (WM_RBUTTONDOWN)) {
      closegraph ();
      exit (1);
    }
    
  } // while
  
  refresh ();
  get_click ();
  
} // circledemo ()

// -----

void ellipsedemo (void)
{
  int
    stop = 0,
    x = maxx / 2,
    y = maxy / 2,
    r = 100,
    cnt = 0;
  
  message ("fillellipse() Demonstration");
  mainwindow ();
  
  while (! stop) {
    
    setfillstyle (0, COLOR (random (256), random (256), random (256)));
    fillellipse (random (maxx), random (maxy),
		 random (r), random (r));
    
    cnt++;
    if (0 == cnt % 200)
      refresh ();
    
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    
    if (ismouseclick (WM_RBUTTONDOWN)) {
      closegraph ();
      exit (1);
    }
    
  } // while
  
  refresh ();
  get_click ();
  
} // ellipsedemo ()

// -----

static int is_in_range (int x, int x1, int x2)
{
  return ( x >= x1 && x <= x2);
}

// -----

void floodfilldemo (void)
{
  int
    i, j,
    p, stop = 0;
  
  message ("floodfill() Demonstration (click around!)");
  mainwindow ();
  
  setcolor (RED);
  // draw something to fill
  for (j = 25; j < maxy; j += 20)
    for (i = 0; i < maxx; i += 50) {
      line (i, j, i + 25, j - 25);
      line (i + 25, j - 25, i + 50, j);
    }
  refresh ();
  
  while (! stop) {
  
    if ((p = mouseclick ())) {
      
      if ( (WM_LBUTTONDOWN == p) &&
	   (is_in_range (mousex (), 0, width)) &&
	   (is_in_range (mousey (), 0, height)) ) {
        setcolor (COLOR (random (255), random (255), random (255)));
        floodfill (mousex (), mousey (), RED);
	refresh ();
      }
      else 
        if (WM_RBUTTONDOWN == p) 
          stop = 1;
    }
    
  } // while
  
  refresh ();
  if (! stop)
    get_click ();
  
} // ellipsedemo ()

// -----

void alphademo (void)
{
  int
    stop = 0,
    cnt = 0,
    rnd,
    x1, y1, x2, y2;
  
  message ("setalpha() Demonstration");
  mainwindow ();
  
  while (! stop) {
    
    setcolor (COLOR (random (256), random (256), random (256)));
    setalpha (getcolor (), random (64));

    setfillstyle (0, getcolor ());
    x1 = random (maxx);
    y1 = random (maxx);
    x2 = x1 + random (maxx / 4);
    y2 = y1 + random (maxy / 4);
    bar (x1, y1, x2, y2);
    refresh ();
    
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    
    if (ismouseclick (WM_RBUTTONDOWN)) {
      closegraph ();
      exit (1);
    }
    
  } // while
  
  refresh ();
  get_click ();

} // alphademo ()

// -----

void loadimagedemo (void)
{
  int
    stop = 0,
    cnt = 0,
    rnd,
    x, y;
  
  message ("readimagefile() Demonstration");
  mainwindow ();
  
  while (! stop) {
    
    // out of the viewport, too
    x = random (maxx);
    y = random (maxy);
    
    // bitmap is 512x180 px
    readimagefile ("logo.bmp", x, y, // position
		   x + random (512), // size
		   y + random (180));
    delay (100);
    
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    
    if (ismouseclick (WM_RBUTTONDOWN)) {
      closegraph ();
      exit (1);
    }
    
  } // while
  
  refresh ();
  get_click ();

} // loadimagedemo ()

// -----

void putimagedemo (void)
{
  int
    stop = 0,
    x, y, width, height,
    size;
  
  void *image;
  
  message ("getimage() / putimage() Demonstration");
  mainwindow ();
  
  getviewsettings (&viewport);
  width = viewport.right - viewport.left + 1;
  height = viewport.bottom - viewport.top + 1;
  x = width / 2;
  y = height / 2;
  
  // draw something, 200x100 px
  setfillstyle (0, COLOR (240, 0, 0));
  fillellipse (x, y, 100, 50);
  setfillstyle (0, COLOR (180, 0, 0));
  fillellipse (x, y, 70, 40);
  setfillstyle (0, COLOR (120, 0, 0));
  fillellipse (x, y, 50, 30);
  setfillstyle (0, COLOR (80, 0, 0));
  fillellipse (x, y, 30, 20);
  setfillstyle (0, COLOR (40, 0, 0));
  fillellipse (x, y, 10, 10);
  
  size = imagesize (x - 100, y - 50, x + 100, y + 50);
  image = malloc (size); // no checks!
  getimage (x - 100, y - 50, x + 100, y + 50, image);
  clearviewport ();
  
  setcolor (WHITE);
  line (0, 0, width, height);
  line (0, height, width, 0);
  
  x -= 100;
  y -= 50;
  
  while (! stop) {
    
    // draw it
    putimage (x, y, image, XOR_PUT);
    putimage (width - x, y - 150, image, XOR_PUT);
    putimage (width - x, y + 150, image, XOR_PUT);
    putimage (x, y - 300, image, XOR_PUT);
    putimage (x, y + 300, image, XOR_PUT);
    refresh ();
    
    delay (10);
    
    // erase it
    putimage (x, y, image, XOR_PUT);
    putimage (width - x, y - 150, image, XOR_PUT);
    putimage (width - x, y + 150, image, XOR_PUT);
    putimage (x, y - 300, image, XOR_PUT);
    putimage (x, y + 300, image, XOR_PUT);
    refresh ();
    
    x += 5;
    if (x +100 > width)
      x = 0;
    
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    
    if (ismouseclick (WM_RBUTTONDOWN)) {
      free (image);
      closegraph ();
      exit (1);
    }
    
  } // while
  
  refresh ();
  get_click ();
  free (image);

} // putimagedemo ()

// -----

void textdemo (void)
{
  int
    xm, ym;
  
  message ("outtext(), settextstyle(), settextjustify() Demonstration");
  mainwindow ();
  getviewsettings (&viewport);
  xm = (viewport.right - viewport.left) / 2;
  ym = (viewport.bottom - viewport.top ) / 2;
  
  setcolor (RED);
  line (0, ym, 2 * xm, ym);
  line (xm, 0, xm, 2 * ym);

  settextstyle (DEFAULT_FONT, HORIZ_DIR, 2);
  settextjustify (LEFT_TEXT, TOP_TEXT);
  
  setcolor (GREEN);
  outtextxy (30, 30, "settextstyle (DEFAULT_FONT, HORIZ_DIR, 2);");
  outtextxy (30, 50, "settextjustify (...);");
  
  setcolor (YELLOW);
  outtextxy (xm, ym, "LEFT_TEXT, TOP_TEXT");
  settextjustify (LEFT_TEXT, BOTTOM_TEXT);
  outtextxy (xm, ym, "LEFT_TEXT, BOTTOM_TEXT");
  settextjustify (RIGHT_TEXT, BOTTOM_TEXT);
  outtextxy (xm, ym, "RIGHT_TEXT, BOTTOM_TEXT");
  settextjustify (RIGHT_TEXT, TOP_TEXT);
  outtextxy (xm, ym, "RIGHT_TEXT, TOP_TEXT");
  refresh ();
  
  pause ();
  
  clearviewport ();
  setcolor (RED);
  line (0, ym, 2 * xm, ym);
  line (xm, 0, xm, 2 * ym);
  
  setcolor (GREEN);
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 2);
  settextjustify (LEFT_TEXT, TOP_TEXT);
  outtextxy (30, 30, "settextstyle (DEFAULT_FONT, VERT_DIR, 2);");
  outtextxy (30, 50, "settextjustify (...);");
  
  setcolor (YELLOW);
  settextstyle (DEFAULT_FONT, VERT_DIR, 2);
  settextjustify (LEFT_TEXT, TOP_TEXT);
  outtextxy (xm, ym, "LEFT_TEXT, TOP_TEXT");
  settextjustify (LEFT_TEXT, BOTTOM_TEXT);
  outtextxy (xm, ym, "LEFT_TEXT, BOTTOM_TEXT");
  settextjustify (RIGHT_TEXT, BOTTOM_TEXT);
  outtextxy (xm, ym, "RIGHT_TEXT, BOTTOM_TEXT");
  settextjustify (RIGHT_TEXT, TOP_TEXT);
  outtextxy (xm, ym, "RIGHT_TEXT, TOP_TEXT");
  refresh ();
  
  pause ();
  
  clearviewport ();
  setcolor (RED);
  line (0, ym, 2 * xm, ym);
  line (xm, 0, xm, 2 * ym);
  
  setcolor (GREEN);
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 2);
  settextjustify (LEFT_TEXT, TOP_TEXT);
  outtextxy (30, 30, "settextstyle (DEFAULT_FONT, HORIZ_DIR, 1);");
  outtextxy (30, 50, "setusercharsize (4, 1, 10, 1);");
  outtextxy (30, 70, "settextstyle (DEFAULT_FONT, VERT_DIR, 2);");

  setcolor (YELLOW);
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 1);
  setusercharsize (4, 1, 10, 1);
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  outtextxy (xm, ym, "CENTER_TEXT, CENTER_TEXT");
  settextstyle (DEFAULT_FONT, VERT_DIR, 2);
  outtextxy (xm, ym, "CENTER_TEXT, CENTER_TEXT");
  refresh ();
  
  get_click ();
  
} // textdemo ()

// -----

void theend (void)
{
  int
    stop = 0,
    xm, ym,
    col;

  struct viewporttype
    viewport;
  
  message ("This is the end");
  mainwindow ();
  
  getviewsettings (&viewport);
  xm = (viewport.right - viewport.left) / 2;
  ym = (viewport.bottom - viewport.top ) / 2;
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 5);
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  
  while (! stop) {
    
    for (col = 0; col < 256; col += 5) {
      setcolor (COLOR (0, col, 0));
      outtextxy (xm, ym, "That's all, folks!");
      refresh ();
      if (ismouseclick (WM_RBUTTONDOWN)) {
	closegraph ();
	exit (1);
      }
    }
    
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    
    if (ismouseclick (WM_RBUTTONDOWN)) {
      closegraph ();
      exit (1);
    }
  
  } // while
  
  refresh ();
  closegraph ();

} // theend ()

// -----

void pagedemo (void)
{
  int
    stop = 0,
    xm, ym,
    col;

  struct viewporttype
    viewport;
  
  message ("This is the end");
  mainwindow ();
  
  getviewsettings (&viewport);
  xm = (viewport.right - viewport.left) / 2;
  ym = (viewport.bottom - viewport.top ) / 2;
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 2);
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  setcolor (YELLOW);
  
  outtextxy (xm, ym, "This is active page (and visual page) #0");
  outtextxy (xm, ym + 30, "Press a key to switch to page #1");
  refresh ();
  get_key ();
  
  setactivepage (1);
  setvisualpage (1);
  setbkcolor (RED);
  clearviewport ();
  
  outtextxy (xm, ym, "This is active page (and visual page) #1");
  outtextxy (xm, ym + 30, "Press a key to go back to page #0");
  refresh ();
  get_key ();
  
  setactivepage (0);
  setvisualpage (0);
  setcolor (RED);
  outtextxy (xm, ym + 60, "Welcome back to page #0!");
  refresh ();
  
  pause ();

} // pagedemo ()

// -----

int main (void)
{
  
  init_sdlbgi ();
  sdlbgi_info ();
  colordemo ();
  pixeldemo ();
  linedemo ();
  linereldemo ();
  boxdemo ();
  circledemo ();
  ellipsedemo ();
  floodfilldemo ();
  alphademo ();
  loadimagedemo ();
  putimagedemo ();
  textdemo ();
  pagedemo ();
  theend ();
  
  return 0;
}
