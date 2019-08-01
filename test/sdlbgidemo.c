/* sdlbgidemo.c  -*- C -*-
 * 
 * To compile:
 * gcc -o sdlbgidemo sdlbgidemo.c -lSDL_bgi -lSDL2 -lm
 * 
 * By Guido Gonzato, June 2018.
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

#define PI_CONV (3.1415926 / 180.0)

int 
  maxx, maxy,
  width, height;

int gd, gm;

struct viewporttype viewport;

void wait (int);
void get_click (void);
void ket_key (void);
void pause (void);
void init_sdlbgi (void);
void message (char *str);
void mainwindow (void);

// -----

void wait (int msec)
{
  int stop = 0;
  Uint32
    ticks,
    now = SDL_GetTicks ();
  
  while (! stop) {
    if (kbhit ())
      stop = 1;
    
    ticks = SDL_GetTicks ();
    if (ticks - now > msec)
      stop = 1;
  }
  
} // wait ()

// -----

void get_click (void)
{
  // wait for a mouse click
  while (! ismouseclick (WM_LBUTTONDOWN))
    ;
  while (ismouseclick (WM_LBUTTONDOWN))
    ;

} // get_click ()

// -----

void pause (void)
{
  // pause and exit if right click
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
  // initialise SDL_bgi system
  setwinoptions ("", 
		 SDL_WINDOWPOS_UNDEFINED,
		 SDL_WINDOWPOS_UNDEFINED,
		 SDL_WINDOW_FULLSCREEN);
  detectgraph (&gd, &gm);
  initgraph (&gd, &gm, "");
  sdlbgifast ();
  maxx = getmaxx ();
  maxy = getmaxy ();
} // init_sdlbgi ()

// -----

void message (char *str)
{
  // output a message at the screen bottom
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
  // set up main window
  setcolor (WHITE);
  setviewport (0, 0, maxx, maxy, 1);
  rectangle (39, 19, maxx - 39, maxy - 19);
  setviewport (40, 20, maxx - 40, maxy - 20, 1);
  getviewsettings (&viewport);
  setbkcolor (COLOR (0, 0, 20));
  clearviewport ();
  refresh ();
  width = viewport.right - viewport.left + 1;
  height = viewport.bottom - viewport.top + 1;
}

// -----

Uint32 speedtest (void)
{
  Uint32
    start, pixels = 0;
  
  start = SDL_GetTicks ();
  do {
    putpixel (random (maxx), random (maxy), random (MAXCOLORS));
    pixels++;
  } while (SDL_GetTicks () < start + 1000);
  
  clearviewport ();
  return (pixels);
}

// -----

void load_logo ()
{
  setwinoptions ("", 
  		 SDL_WINDOWPOS_CENTERED,
  		 SDL_WINDOWPOS_CENTERED,
  		 SDL_WINDOW_BORDERLESS);
  initwindow (440, 150);
  readimagefile ("logo.bmp", 0, 0, 440, 150);
  wait (2000);
  closewindow (0);
  
}

// -----

void sdlbgi_info (void)
{
  // display information on SDL_bgi settings
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
    pixels_per_second;

  char *driver, *mode;
  int x, y, dy;

  message ("Please wait one second...");
  pixels_per_second = speedtest ();
  message ("Information on SDL_bgi Graphics");
  mainwindow ();
  setcolor (RED);
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 2);
  settextjustify (RIGHT_TEXT, TOP_TEXT);
  
  getviewsettings (&viewport); // get viewport info
  getlinesettings (&lineinfo); // line style info
  getfillsettings (&fillinfo); // fill style info
  gettextsettings (&textinfo); // text style info
  getpalette (&palette );
  driver = getdrivername ();
  mode = getmodename (gm);
  
  x = width / 2; // screen centre
  y = 40; 
  dy = textheight ("X") * 3;
  
  outtextxy (x, y += dy, "Graphics device: ");
  outtextxy (x, y += dy, "Graphics mode: ");
  outtextxy (x, y += dy, "Screen resolution: ");
  outtextxy (x, y += dy, "Current viewport: ");
  outtextxy (x, y += dy, "Viewport clipping: ");
  outtextxy (x, y += dy, "Current background colour: ");
  outtextxy (x, y += dy, "Current foreground colour: ");
  outtextxy (x, y += dy, "Current fill colour: ");
  outtextxy (x, y += dy, "Character size: ");
  outtextxy (x, y += dy, "Available pages: ");
  outtextxy (x, y += dy, "Pixels per second: ");
  
  settextjustify (LEFT_TEXT, TOP_TEXT);
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
  sprintf (str, "0x%02X%02X%02X", 
	   RED_VALUE (getbkcolor ()),
	   GREEN_VALUE (getbkcolor ()),
	   BLUE_VALUE (getbkcolor ()));
  outtextxy (x, y += dy, str);
  sprintf (str, "0x%02X%02X%02X", 
	   RED_VALUE (getcolor ()),
	   GREEN_VALUE (getcolor ()),
	   BLUE_VALUE (getcolor ()));
  outtextxy (x, y += dy, str);
  sprintf (str, "0x%02X%02X%02X", 
	   RED_VALUE (fillinfo.color),
	   GREEN_VALUE (fillinfo.color),
	   BLUE_VALUE (fillinfo.color));
  outtextxy (x, y += dy, str);
  sprintf (str, "%d", textinfo.charsize);
  outtextxy (x, y += dy, str);
  sprintf (str, "%d", VPAGES);
  outtextxy (x, y += dy, str);
  sprintf (str, "%d", pixels_per_second);
  outtextxy (x, y += dy, str);

  refresh ();
  pause ();

} // sdlbgi_info ()

// -----

void modedemo ()
{
  int
    dx, y, size;
  void
    *m;
  
  message ("Plotting Bitwise Operators Demonstration");
  mainwindow ();
  getviewsettings (&viewport);
  dx = (viewport.right - viewport.left - 100) / 5;
  y = (viewport.bottom - viewport.top) / 3;
  
  /* draw a red rectangle and store it */
  settextjustify (LEFT_TEXT, TOP_TEXT);
  setfillstyle (SOLID_FILL, RED);
  bar (50, y, 50 + dx - 10, y + dx - 10);
  size = imagesize (50, y, 50 + dx - 10, y + dx - 10);
  m = malloc (size);
  getimage (50, y, 50 + dx - 10, y + dx - 10, m);
  
  /* draw five yellow rectangles */
  setcolor (WHITE);
  setfillstyle (SOLID_FILL, YELLOW);
  for (int i = 0 ; i < 5; i++)
    bar (50 + i*dx, y, 50 + i*dx + dx - 10, y + dx - 10);
  
  outtextxy (50 + 0*dx, y - 20, "COPY_PUT");
  outtextxy (50 + 1*dx, y - 20, "XOR_PUT");
  outtextxy (50 + 2*dx, y - 20, "OR_PUT");
  outtextxy (50 + 3*dx, y - 20, "AND_PUT");
  outtextxy (50 + 4*dx, y - 20, "NOT_PUT");
  setcolor (BLACK);
  
  outtextxy (60, y + 10, "ffff00 (yellow)");
  
  // COPY_PUT: red: ff0000
  putimage  (50 + 0*dx, y + dx/2, m, COPY_PUT);
  outtextxy (60 + 0*dx, y + dx/2 + 20, "ff0000");
  outtextxy (60 + 0*dx, y + dx/2 + 30, "(red)");
  
  // XOR_PUT: red xor yellow: ff0000 ^ ffff00 = 00ff00 (green)
  putimage  (50 + 1*dx, y + dx/2, m, XOR_PUT);
  outtextxy (60 + 1*dx, y + dx/2 + 20, "ff0000 ^ ffff00 = 00ff00");
  outtextxy (60 + 1*dx, y + dx/2 + 30, "(green)");
  
  // OR_PUT: red or yellow: ff0000 | ffff00 = ffff00 (yellow)
  putimage  (50 + 2*dx, y + dx/2, m, OR_PUT);
  outtextxy (60 + 2*dx, y + dx/2 + 20, "ff0000 | ffff00 = ffff00");
  outtextxy (60 + 2*dx, y + dx/2 + 30, "(yellow)");
  
  // AND_PUT: red and yellow: ff0000 & ffff00 = ff0000 (red)
  putimage  (50 + 3*dx, y + dx/2, m, AND_PUT);
  outtextxy (60 + 3*dx, y + dx/2 + 20, "ff0000 & ffff00 = 00ff00");
  outtextxy (60 + 3*dx, y + dx/2 + 30, "(red)");
  
  // NOT_PUT: ~ red: 00ffff (cyan)
  putimage  (50 + 4*dx, y + dx/2, m, NOT_PUT);
  outtextxy (60 + 4*dx, y + dx/2 + 20, "~ ff0000 = 00ffff");
  outtextxy (60 + 4*dx, y + dx/2 + 30, "(cyan)");

  refresh ();
  pause ();
  
} // modedemo ()

// -----

void colordemo (void)
{
  // show RGB colours
  int
    stop = 0,
    i, x, y,
    col1, col2, col3;
  
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
      // draw lines
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
  // plot pixels quickly
  int
    stop = 0,
    cnt = 0;
  
  message ("putpixel() Demonstration");
  mainwindow ();
  
  while (! stop) {
    
    setcolor (COLOR (random (256), random (256), random (256)));
    putpixel (random (maxx), random (maxy), getcolor ());
    // or: _putpixel (random (maxx), random (maxy));
    cnt++;
    
    // refresh the screen every 5000 pixels plotted
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
  // draw lines and rectangles
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
      // the line is clipped anyway
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
  // show how to use line styles and relative lines
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

    // create a polygon, at least 5 sides
    npoints = 5 + random (MAXPOINTS - 5 + 1);
    if (npoints % 2) // even number of sides
      setlinestyle (0, 0, THICK_WIDTH);
    else // odd number of sides
      setlinestyle (0, 0, NORM_WIDTH);
    clearviewport ();
    setcolor (BLUE + random (MAXCOLORS - 1));
    
    angle = 0.0;
    step = 360.0 / npoints;
    
    // evaluate polygon vertices
    for (i= 0; i < npoints; i++) {
      pts_x[i] = x + (int)(cos (angle * PI_CONV) * radius);
      pts_y[i] = y - (int)(sin (angle * PI_CONV) * radius);
      angle += step;
    }

    circle (x, y, radius );

    // draw diagonals
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
  // draw filled bars
  int
    stop = 0,
    cnt = 0,
    x1, y1, x2, y2;
  
  message ("bar() Demonstration");
  mainwindow ();
  
  while (! stop) {
    
    // fill colour
    setfillstyle (random (USER_FILL),
		  COLOR (random (256), random (256), random (256)));
    x1 = random (maxx);
    y1 = random (maxx);
    x2 = x1 + random (maxx / 4);
    y2 = y1 + random (maxy / 4);
    // outline colour
    setcolor (COLOR (random (256), random (256), random (256)));
    bar3d (x1, y1, x2, y2, 0, 0); // outlined bars
    
    cnt++;
    if (0 == cnt % 200) // refresh every 200 bars
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

void polygondemo (void)
{
  // draw filled polygons
  int
    stop = 0,
    cnt = 0,
    i,
    numpoints,
    poly[32];
  
  message ("fillpoly() Demonstration");
  mainwindow ();
  
  while (! stop) {
    
    // fill colour
    setfillstyle (random (USER_FILL),
		  COLOR (random (256), random (256), random (256)));
    numpoints = 3 + random (14); // max 16
    for (i = 0; i < 2 * numpoints; i += 2) {
      poly[i]     = random (maxx);
      poly[i + 1] = random (maxy);
    }
    // outline colour
    
    setcolor (COLOR (random (256), random (256), random (256)));
    fillpoly (numpoints, poly); // outlined filled polygons
    
    cnt++;
    if (0 == cnt % 50) // refresh every 50 polygons
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

} // polygondemo ()

// -----

void circledemo (void)
{
  // draw arcs and ellipses
  int
    stop = 0,
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
  // draw filled ellipses
  int
    stop = 0,
    r = 100,
    cnt = 0;
  
  message ("fillellipse() Demonstration");
  mainwindow ();
  
  while (! stop) {
    
    setfillstyle (random (USER_FILL), 
		  COLOR (random (256), random (256), random (256)));
    setcolor (COLOR (random (256), random (256), random (256)));
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

int
  sierp_x,
  sierp_y, 
  len;

static void sierp_a (int i);
static void sierp_b (int i);
static void sierp_c (int i);
static void sierp_d (int i);

// Sierpinsky curve; taken from Niklaus Wirth's
// Algorithms + Data Structures = Programs

static void sierp_a (int level)
{
  if (level > 0) {
    sierp_a (level - 1);
    sierp_x += len;
    sierp_y -= len;
    lineto (sierp_x, sierp_y);
    sierp_b (level - 1);
    sierp_x += 2*len;
    lineto (sierp_x, sierp_y);
    sierp_d (level - 1);
    sierp_x += len;
    sierp_y += len;
    lineto (sierp_x, sierp_y);
    sierp_a (level - 1);
  }
}

static void sierp_b (int level)
{
  if (level > 0) {
    sierp_b(level - 1);
    sierp_x -= len; 
    sierp_y -= len;
    lineto (sierp_x, sierp_y);
    sierp_c (level - 1);
    sierp_y -= 2*len;
    lineto (sierp_x, sierp_y);
    sierp_a (level - 1);
    sierp_x += len;
    sierp_y -= len;
    lineto (sierp_x, sierp_y);
    sierp_b (level - 1);
  }
}

static void sierp_c (int level)
{
  if (level > 0) {
    sierp_c (level - 1);
    sierp_x -= len;
    sierp_y += len;
    lineto (sierp_x, sierp_y);
    sierp_d (level - 1);
    sierp_x -= 2*len;
    lineto (sierp_x, sierp_y);
    sierp_b (level - 1);
    sierp_x -= len;
    sierp_y -= len;
    lineto (sierp_x, sierp_y);
    sierp_c (level - 1);
  }
}

static void sierp_d (int level)
{
  if (level > 0) {
    sierp_d (level - 1);
    sierp_x += len;
    sierp_y += len;
    lineto (sierp_x, sierp_y);
    sierp_a (level - 1);
    sierp_y += 2*len;
    lineto (sierp_x, sierp_y);
    sierp_c (level - 1);
    sierp_x -= len;
    sierp_y += len;
    lineto (sierp_x, sierp_y);
    sierp_d (level - 1);
  }
}

void sierpinski (int level)
{
  sierp_a (level);
  sierp_x += len;
  sierp_y -= len;
  lineto (sierp_x, sierp_y);
  
  sierp_b (level);
  sierp_x -= len;
  sierp_y -= len;
  lineto (sierp_x, sierp_y);
  
  sierp_c (level);
  sierp_x -= len;
  sierp_y += len;
  lineto (sierp_x, sierp_y);
  
  sierp_d (level);
  sierp_x += len;
  sierp_y += len;
  lineto (sierp_x, sierp_y);
}

// -----

void floodfilldemo (void)
{
  // show how to use floodfill ()
  int
    sierp_width,
    stop = 0;
  
  message ("floodfill() Demonstration");
  mainwindow ();
  width = viewport.right - viewport.left + 1;
  height = viewport.bottom - viewport.top + 1;
  
  setcolor (WHITE);
  
  // each "petal" is (len*6) pixels wide; at recursion level "N", we have
  // 2^(N-1) petals connected by 2^(N-1)-1 segments, which are (2*len)
  // pixels long. The overall width of the Sierpinski curve is then:
  // W = (len*6)*2^(N-1) + (len*2)*2^(N-1)-1
  // For example: given len=5, at recursion level 5 we'll have 16
  // petals connected by 15 segments; width = 5*8*16 + 5*2*15 = 630 pixels.

  // find max possible length
  len = 15;
  while ( (sierp_width = len*6*16 + len*2*15) > height - 4)
    len--;
  
  sierp_width = (len*6)*16 + (len*2)*15;
  sierp_x = width / 2 - sierp_width / 2;
  sierp_y = height - (height - sierp_width) / 2;
  moveto (sierp_x, sierp_y);
  sierpinski (5);
  
  while (! stop) {
    
    setfillstyle (SOLID_FILL, 1 + random (YELLOW));
    // center of the Sierpinski curve
    floodfill (width / 2, height / 2, WHITE);
    refresh ();
    delay (100);
  
    setfillstyle (SOLID_FILL, 1 + random (YELLOW));
    // outside
    floodfill (1, 1, WHITE);
    refresh ();
    delay (100);
    
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    
    if (ismouseclick (WM_RBUTTONDOWN)) {
      closegraph ();
      exit (1);
    }
    
  } // while
  
  refresh ();
  pause ();
  
} // floodfilldemo ()

// -----

void alphademo (void)
{
  int
    stop = 0,
    x1, y1, x2, y2;
  
  message ("setalpha() Demonstration");
  mainwindow ();
  
  while (! stop) {
    
    setcolor (COLOR (random (256), random (256), random (256)));
    setalpha (getcolor (), random (64)); // alpha component of ARGB

    setfillstyle (SOLID_FILL, getcolor ());
    x1 = random (maxx);
    y1 = random (maxx);
    x2 = x1 + random (maxx / 4);
    y2 = y1 + random (maxy / 4);
    bar (x1, y1, x2, y2);
    // alpha is applied ONLY when refresh () is called
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

void readimagedemo (void)
{
  // load a .bmp file
  int
    stop = 0,
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

} // readimagedemo ()

// -----

void putimagedemo (void)
{
  // show how to use old-style image "blitting";
  // you'll want to use native SDL functions instead!
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
  setfillstyle (SOLID_FILL, COLOR (240, 0, 0));
  fillellipse (x, y, 100, 50);
  setfillstyle (SOLID_FILL, COLOR (180, 0, 0));
  fillellipse (x, y, 70, 40);
  setfillstyle (SOLID_FILL, COLOR (120, 0, 0));
  fillellipse (x, y, 50, 30);
  setfillstyle (SOLID_FILL, COLOR (80, 0, 0));
  fillellipse (x, y, 30, 20);
  setfillstyle (SOLID_FILL, COLOR (40, 0, 0));
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
    
    // draw it in XOR mode
    putimage (x, y, image, XOR_PUT);
    putimage (width - x, y - 150, image, XOR_PUT);
    putimage (width - x, y + 150, image, XOR_PUT);
    putimage (x, y - 300, image, XOR_PUT);
    putimage (x, y + 300, image, XOR_PUT);
    refresh ();
    
    delay (10);
    
    // erase it by redrawing it
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
  free (image); // remember to free the image eventually

} // putimagedemo ()

// -----

void sdlmixdemo (void)
{
  // show how to use SDL native functions along with SDL_bgi
  int 
    i, inc, stop = 0;
  SDL_Surface
    *src;
  SDL_Texture
    *bm_texture;
  SDL_Rect
    rect1, rect2;    
    
  message ("SDL_BGI and Native Functions Demonstration");
  mainwindow ();
  line (0, height / 2, width, height / 2);
  line (width / 2, 0, width / 2, height);
  refresh ();
  
  // load a bitmap from file
  src = SDL_LoadBMP ("./plasma.bmp"); // 600 x 600
  if (NULL == src) {
    fprintf (stderr, "Can't load bitmap\n");
    return;
  }

  // source rect
  rect1.x = 0;
  rect1.y = 0;
  rect1.w = 300;
  rect1.h = 300;
  
  // destination - twice as big
  rect2.x = getmaxx () / 2 - 300;
  rect2.y = getmaxy () / 2 - 300;
  rect2.w = 600;
  rect2.h = 600;
  
  i = 0;
  inc = 1;
  
  while (!stop) {
    
    // pan the image
    
    rect1.x += inc;
    rect1.y += inc;
    
    // bgi_renderer, along with bgi_window and bgi_texture,
    // can be accessed by the programmer
    
    bm_texture = SDL_CreateTextureFromSurface (bgi_renderer, src);
    SDL_RenderCopy (bgi_renderer, bm_texture, &rect1, &rect2);
    SDL_RenderPresent (bgi_renderer);
    
    if (300 == ++i) {
      i = 0;
      inc = -inc;
    }
    if (ismouseclick (WM_LBUTTONDOWN))
      stop = 1;
    if (ismouseclick (WM_RBUTTONDOWN)) {
      closegraph ();
      exit (1);
    }
  
  } // while
    
  get_click ();

} // sdlmixdemo ()

// -----

void textdemo (void)
{
  // show how to use text styles
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

void pagedemo (void)
{
  // show how to use visual pages
  int
    xm, ym,
    num_page;
  char 
    title [100];

  message ("setactivepage() / setvisualpage () Demonstration");
  mainwindow ();
  
  getviewsettings (&viewport);
  xm = (viewport.right - viewport.left) / 2;
  ym = (viewport.bottom - viewport.top ) / 2;
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 6);
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  
  for (num_page = 0; num_page < VPAGES; num_page++) {
    setactivepage (num_page);
    setbkcolor (COLOR (20*num_page, 20*num_page, 20*num_page));
    clearviewport ();
    setcolor (BLUE + num_page);
    settextstyle (DEFAULT_FONT, HORIZ_DIR, 6);
    sprintf (title, "P A G E   %d", num_page);
    outtextxy (xm, ym/2, title);
    settextstyle (DEFAULT_FONT, HORIZ_DIR, 2);
    sprintf (title, "This is page %d", num_page);
    outtextxy (xm, ym, title);
    outtextxy (xm, ym + 30, "Left click to go to next page");
  }

  for (num_page = 0; num_page < VPAGES; num_page++) {
    setvisualpage (num_page);
    refresh ();
    pause ();
  }
  
  setactivepage (0);
  setvisualpage (0);
  setcolor (RED);
  outtextxy (xm, ym + 60, "Welcome back to page #0!");
  refresh ();
  
  pause ();

} // pagedemo ()

// -----

void theend (void)
{
  // the end of the game
  int
    stop = 0,
    xm, ym,
    col,
    r, maxr;

  message ("That's All Folks!");
  mainwindow ();

  // draw the red circles
  
  xm = width / 2;
  ym = height / 2;
  maxr = sqrt (width*width/4 + height*height/4);
  col = 0;
  for (r = 200; r < maxr; r++) {
    setcolor (COLOR(256 - col, 40, 0));
    circle (xm, ym, r);
    col += 4;
    if (col > 200)
      col = 0;
  }
  
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 6);
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  
  // draw the flashing string
  
  while (! stop) {
    
    for (col = 0; col < 256; col += 3) {
      setcolor (COLOR (col, col, col));
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

int main (int argc, char *argv[])
{
  load_logo ();
  
  init_sdlbgi ();
  sdlbgi_info ();
  modedemo ();
  colordemo ();
  pixeldemo ();
  linedemo ();
  linereldemo ();
  boxdemo ();
  polygondemo ();
  circledemo ();
  ellipsedemo ();
  floodfilldemo ();
  alphademo ();
  readimagedemo ();
  putimagedemo ();
  sdlmixdemo ();
  textdemo ();
  pagedemo ();
  theend ();
  
  return 0;
}
