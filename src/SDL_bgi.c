/* SDL_bgi.c	-*- C -*-
 *
 * A BGI-like graphics library based on SDL and SDL_gfx.
 * Easy to use and useful for porting old programs.
 * Guido Gonzato, December 2014.
 * 
 */

#include "SDL_bgi.h"
#include "SDL_bgi_fonts.h"

#define VERSION 1.0.1

SDL_Surface *BGI;  // we draw stuff here

// the palette contains the BGI colors, entries 0:MAXCOLORS;
// then two entries for temporary RGB colors allocated with COLOR();
// then user-defined RGB colors

static Uint32 palette[1 + MAXCOLORS + 2 + PALETTE_SIZE]; // all colors
static Uint32 bgi_palette[1 + MAXCOLORS] = { // 0 - 15
    0x000000ff, // BLACK
  0x0000ffff, // BLUE
  0x00ff00ff, // GREEN
  0x00ffffff, // CYAN
  0xff0000ff, // RED
  0xff00ffff, // MAGENTA
  0xa52a2aff, // BROWN
  0xd3d3d3ff, // LIGHTGRAY
  0xa9a9a9ff, // DARKGRAY
  0xadd8e6ff, // LIGHTBLUE
  0x90ee90ff, // LIGHTGREEN
  0xe0ffffff, // LIGHTCYAN
  0xf08080ff, // LIGHTRED
  0xdb7093ff, // LIGHTMAGENTA
  0xffff00ff, // YELLOW
  0xffffffff  // WHITE
};

static Uint32 tmp_color;  // temporary color set up by COLOR()

static int
  fg_color = WHITE, // index of BGI foreground color
  bg_color = BLACK, // index of BGI background color
  mouse_x, mouse_y, // coordinates of last mouse click
  font_width = 8,   // default font width and height  
  font_height = 8,
  fast_mode = 1,    // needs screen update?
  cp_x = 0,         // current position
  cp_y = 0,
  maxx, maxy,       // screen dimensions
  gm,               // graphics mode
  rgb_mode = 0,     // BGI or RGB colors
  writemode;        // plotting method

static char
  last_key = 0;     // last key pressed

static float
  font_mag_x = 1.0, // font magnification
  font_mag_y = 1.0;

static const void *fontptr; // pointer to font array

// static struct bgi_info *vga_palette;
// static struct rgb_colour *rgb_palette;
static struct arccoordstype last_arc;
static struct fillsettingstype fill_style;
static struct linesettingstype line_style;
static struct textsettingstype txt_style;
static struct viewporttype vp;
static struct palettetype pal;

static void initpalette (void);

static Uint32 getpixel_raw (int, int);
static void putpixel_raw (int, int, Uint32, int);

// -------------------------------------------------- //

void putpixel_raw (int x, int y, Uint32 pixel, int op)
{
  int bpp = BGI->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)BGI->pixels + y * BGI->pitch + x * bpp;

  switch (op) {
    
  case COPY_PUT:
    
    switch (bpp) {
  
    case 1:
      *p = pixel;
      break;
      
    case 2:
      *(Uint16 *)p = pixel;
      break;
      
    case 3:
      
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
	p[0] = (pixel >> 16) & 0xff;
	p[1] = (pixel >> 8) & 0xff;
	p[2] = pixel & 0xff;
      } else {
	p[0] = pixel & 0xff;
	p[1] = (pixel >> 8) & 0xff;
	p[2] = (pixel >> 16) & 0xff;
      }
      break;
    
    case 4:
      *(Uint32 *)p = pixel;
      break;
      
    } // switch (bpp)
    break;
    
  case XOR_PUT:
    
    switch (bpp) {
  
    case 1:
      *p ^= pixel;
      break;
      
    case 2:
      *(Uint16 *)p ^= pixel;
      break;
      
    case 3:
      
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
	p[0] ^= (pixel >> 16) & 0xff;
	p[1] ^= (pixel >> 8) & 0xff;
	p[2] ^= pixel & 0xff;
      } else {
	p[0] ^= pixel & 0xff;
	p[1] ^= (pixel >> 8) & 0xff;
	p[2] ^= (pixel >> 16) & 0xff;
      }
      break;
    
    case 4:
      *(Uint32 *)p ^= pixel;
      break;
      
    } // switch (bpp)
    break;

  case AND_PUT:
    
    switch (bpp) {
  
    case 1:
      *p &= pixel;
      break;
      
    case 2:
      *(Uint16 *)p &= pixel;
      break;
      
    case 3:
      
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
	p[0] &= (pixel >> 16) & 0xff;
	p[1] &= (pixel >> 8) & 0xff;
	p[2] &= pixel & 0xff;
      } else {
	p[0] &= pixel & 0xff;
	p[1] &= (pixel >> 8) & 0xff;
	p[2] &= (pixel >> 16) & 0xff;
      }
      break;
    
    case 4:
      *(Uint32 *)p &= pixel;
      break;
      
    } // switch (bpp)
    break;

  case OR_PUT:
    
    switch (bpp) {
  
    case 1:
      *p |= pixel;
      break;
      
    case 2:
      *(Uint16 *)p |= pixel;
      break;
      
    case 3:
      
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
	p[0] |= (pixel >> 16) & 0xff;
	p[1] |= (pixel >> 8) & 0xff;
	p[2] |= pixel & 0xff;
      } else {
	p[0] |= pixel & 0xff;
	p[1] |= (pixel >> 8) & 0xff;
	p[2] |= (pixel >> 16) & 0xff;
      }
      break;
    
    case 4:
      *(Uint32 *)p |= pixel;
      break;
      
    } // switch (bpp)
    break;

  } // switch (op)

  // to do: NOT_PUT
  
}

// -----

void ifswap (int *x1, int *x2)
{
  int tmp;
  
  if (*x1 > *x2) {
    tmp = *x1;
    *x1 = *x2;
    *x2 = tmp;
  }
}

// -----

void fixrange (int *x)
{
  if (*x < 0)
    *x = 0;
  if (*x > (BGI->w - 1)) // max x
    *x = BGI->w - 1; // max y
}

// -----

void unimplemented (char *msg)
{
  fprintf (stderr, "%s() is not yet implemented.\n", msg);
}

// -----

void updaterect (int x1, int y1, int x2, int y2)
{
  fixrange (&x1);
  fixrange (&y1);
  fixrange (&x2);
  fixrange (&y2);
  ifswap (&x1, &x2);
  ifswap (&y1, &y2);
  SDL_UpdateRect (BGI, (Sint32) x1,(Sint32) y1, 
		  (Sint32) (x2 - x1 + 1), (Sint32) (y2 - y1 + 1));
}

// -----

#define PI_CONV (3.1415926 / 180.0)

void arc (int x, int y, int stangle, int endangle, int radius)
{
  arcColor (BGI, (Sint16)x + vp.left, (Sint16)y + vp.top, (Sint16)radius,
	    (Sint16)-endangle, (Sint16)-stangle, palette[fg_color]);
  
  last_arc.x = x;
  last_arc.y = y;
  last_arc.xstart = x + (int)(radius * cos (stangle * PI_CONV));
  last_arc.ystart = y + (int)(radius * sin (stangle * PI_CONV));
  last_arc.xend = x + (int)(radius * cos (endangle * PI_CONV));
  last_arc.yend = y + (int)(radius * sin (endangle * PI_CONV));
  
  if (! fast_mode)
    updaterect (last_arc.xstart, last_arc.ystart,
		last_arc.xend, last_arc.yend);
}

// -----

void _putpixel (int x, int y)
{
  pixelColor (BGI, (Sint16)x, (Sint16)y, palette[fg_color]);
  // putpixel_raw ((Sint16)x, (Sint16)y, palette[fg_color]>> 8, 0);
}

// -----

void bar3d (int left, int top, int right, int bottom, int depth, int topflag)
{
  Uint32 tmpcolor;
  int x1, y1, x2, y2;
  
  if (EMPTY_FILL == fill_style.pattern)
    tmpcolor = palette[bg_color];
  else
    tmpcolor = palette[fill_style.color];
  
  boxColor (BGI, left + vp.left, top + vp.top,
            right + vp.left, bottom + vp.top, tmpcolor);
  rectangleColor (BGI, left + vp.left, top + vp.top,
            right + vp.left, bottom + vp.top, palette[fg_color]);
  
  lineColor (BGI,
             left + vp.left, top + vp.top,
             left + depth + vp.left, top - depth + vp.top,
             palette[fg_color]);
  lineColor (BGI,
             left + depth + vp.left, top - depth + vp.top,
             right + depth + vp.left, top - depth + vp.top,
             palette[fg_color]);
  lineColor (BGI,
             right + vp.left, top + vp.top,
             right + depth + vp.left, top - depth + vp.top,
             palette[fg_color]);
  lineColor (BGI,
             right + vp.left, bottom + vp.top,
             right + depth + vp.left, bottom - depth + vp.top,
             palette[fg_color]);
  lineColor (BGI,
             right + depth + vp.left, bottom - depth + vp.top,
             right + depth + vp.left, top - depth + vp.top,
             palette[fg_color]);

  if (! fast_mode) {
    x1 = left + vp.left;
    y1 = top + vp.top - depth;
    x2 = right + vp.left + depth;
    y2 = bottom + vp.top;
    updaterect (x1, y1, x2, y2);
  }
}

// -----


void bar (int left, int top, int right, int bottom)
{
  int x1, y1, x2, y2;
  Uint32 tmpcolor;
  
  x1 = left + vp.left;
  y1 = top + vp.top;
  x2 = right + vp.left;
  y2 = bottom + vp.top;
  
  if (EMPTY_FILL == fill_style.pattern)
    tmpcolor = palette[bg_color];
  else
    tmpcolor = palette[fill_style.color];
  boxColor (BGI, (Sint16)x1, (Sint16)y1,
	      (Sint16)x2, (Sint16)y2, tmpcolor);
  
  if (! fast_mode)
    updaterect (x1, y1, x2, y2);
}

// -----

int BLUE_VALUE (int color)
{
  // return the blue component of 'color' in the extended palette
  return (palette[1 + MAXCOLORS + 2 + color] >> 8 & 0xFF);
}

// -----

void circle (int x, int y, int radius)
{
  int x1, y1, x2, y2;
  
  x += vp.left;
  y += vp.top;
    
  circleColor (BGI, x, y, radius, palette[fg_color]);

  if (! fast_mode) {
    x1 = x - radius;
    y1 = y - radius;
    x2 = x + radius;
    y2 = y + radius;
    updaterect (x1, y1, x2, y2);
  }
}

// -----

void cleardevice (void)
{
  // clear the screen regardless of clipping
  SDL_Rect rect;
  
  rect.x = vp.left;
  rect.y = vp.top;
  rect.w = vp.right - vp.left + 1;
  rect.h = vp.bottom - vp.top + 1;
  
  SDL_SetClipRect (BGI, NULL);
  boxColor (BGI, 0, 0, maxx, maxy, palette[bg_color]);
  SDL_UpdateRect (BGI, 0, 0, 0, 0);
  SDL_SetClipRect (BGI, &rect);
}

// -----

void clearviewport (void)
{
  boxColor (BGI, vp.left, vp.top, vp.right, vp.bottom, palette[bg_color]);
  updaterect (vp.left, vp.top, vp.right, vp.bottom);
}

// -----

void closegraph (void)
{
  SDL_FreeSurface (BGI);
  SDL_Quit ();
}

// -----

int COLOR (int r, int g, int b)
{
  // set up the temporary color
  tmp_color = r << 24 | g << 16 | b << 8 | 0xff;
  
  return -1;
}

// -----

void detectgraph (int *graphdriver, int *graphmode)
{
  *graphdriver = X11;
  *graphmode = X11_FULLSCREEN;
}

// -----

void drawpoly (int numpoints, int *polypoints)
{
  int i, x1, y1, x2, y2;
  Sint16 *x, *y;
  
  // temporary arrays
  x = calloc (numpoints, sizeof (int));
  y = calloc (numpoints, sizeof (int));
  
  for (i = 0; i < numpoints; i++) {
    x[i] = polypoints[2 * i] + vp.left;
    y[i] = polypoints[2 * i + 1] + vp.top;
  }

  polygonColor (BGI, x, y, numpoints, palette[fg_color]);
  
  if (! fast_mode) {
    x1 = x2 = x[0];
    y1 = y2 = y[0];
    for (i = 0; i < numpoints; i++) {
      if (x[i] < x1)
	x1 = x[i];
      if (y[i] < y1)
	y1 = y[i];
      if (x[i] > x2)
	x2 = x[i];
      if (y[i] > y2)
	y2 = y[i];
    }
    updaterect (x1, y1, x2, y2);
  }
  free (x);
  free (y);
}

// -----

void ellipse (int x, int y, int stangle, int endangle, 
	      int xradius, int yradius)
{
  int x1, y1, x2, y2;
  
  x += vp.left;
  y += vp.top;
  
  ellipseColor (BGI, x, y, xradius, yradius, palette[fg_color]);
  
  if (! fast_mode) {
    x1 = x - xradius;
    y1 = y - yradius;
    x2 = x + xradius;
    y2 = y + yradius;
    updaterect (x1, y1, x2, y2);
  }
}

// -----

int event (void)
{
  SDL_Event event;
  
  if (SDL_PollEvent (&event)) {
    if ( (event.type == SDL_KEYDOWN) ||
	 (event.type == SDL_MOUSEBUTTONDOWN) )
      return 1;
  }
  return 0;
}

// -----

void fillellipse (int x, int y, int xradius, int yradius)
{
  int x1, y1, x2, y2;

  x += vp.left;
  y += vp.top;
  
  filledEllipseColor (BGI, x, y, xradius, yradius, palette[fg_color]);
  
  if (! fast_mode) {
    x1 = x - xradius;
    y1 = y - yradius;
    x2 = x + xradius;
    y2 = y + yradius;
    updaterect (x1, y1, x2, y2);
  }
}

// -----

// TODO: use texturedPolygon

void fillpoly (int numpoints, int *polypoints)
{
  int i, x1, y1, x2, y2;
  Sint16 *x, *y;
  
  // temporary arrays
  x = calloc (numpoints, sizeof (int));
  y = calloc (numpoints, sizeof (int));
  
  for (i = 0; i < numpoints; i++) {
    x[i] = polypoints[2 * i] + vp.left;
    y[i] = polypoints[2 * i + 1] + vp.top;
  }

  filledPolygonColor (BGI, x, y, numpoints, palette[fill_style.color]);
  polygonColor (BGI, x, y, numpoints, palette[fg_color]);
  
  if (! fast_mode) {
    x1 = x2 = x[0];
    y1 = y2 = y[0];
    for (i = 0; i < numpoints; i++) {
      if (x[i] < x1)
	x1 = x[i];
      if (y[i] < y1)
	y1 = y[i];
      if (x[i] > x2)
	x2 = x[i];
      if (y[i] > y2)
	y2 = y[i];
    }
    updaterect (x1, y1, x2, y2);
  }
  free (x);
  free (y);
}

// -----

// the following code was adapted from "A Seed Fill Algorithm"
// by Paul Heckbert, "Graphics Gems", Academic Press, 1990

typedef struct {
  int y, xl, xr, dy;
} Segment;

/*
 * Filled horizontal segment of scanline y for xl<=x<=xr.
 * Parent segment was on line y-dy.  dy=1 or -1
 */

#define STACKSIZE 2000               /* max depth of stack */

#define PUSH(Y, XL, XR, DY)     /* push new segment on stack */ \
    if (sp < stack+STACKSIZE && Y+(DY) >= 0 && Y+(DY) <= maxy ) \
    {sp->y = Y; sp->xl = XL; sp->xr = XR; sp->dy = DY; sp++;}

#define POP(Y, XL, XR, DY)      /* pop segment off stack */ \
    {sp--; Y = sp->y+(DY = sp->dy); XL = sp->xl; XR = sp->xr;}

/*
 * fill: set the pixel at (x,y) and all of its 4-connected neighbors
 * with the same pixel value to the new pixel value nv.
 * A 4-connected neighbor is a pixel above, below, left, or right of a pixel.
 */

void floodfill (int x, int y, int border)
{
  int start, x1, x2, dy = 0;
  unsigned long oc, nc, b; // border pixel value
  Segment stack[STACKSIZE], *sp = stack; // stack of filled segments
  
  x += vp.left;
  y += vp.top;
  
  oc = getpixel (x, y);
  b = border; // vga_palette[border].pixel_value;
  nc = getcolor (); // vga_palette[getcolor()].pixel_value;
  if (oc == b || oc == nc)
    return;
  
  PUSH(y, x, x, 1);           /* needed in some cases */
  PUSH(y + 1, x, x, -1);      /* seed segment (popped 1st) */

  while (sp > stack) {
    /* pop segment off stack and fill a neighboring scan line */
    POP(y, x1, x2, dy);
     /* segment of scan line y-dy for x1<=x<=x2 was previously filled,
        now explore adjacent pixels in scan line y
      */
    for (x = x1; x >= 0 && getpixel(x, y) == oc; x--)
      _putpixel(x, y);
    if (x >= x1) {
      for (x++; x <= x2 && getpixel(x, y) == b; x++)
        ;
      start = x;
      if (x > x2)
        continue;
      } else {
      start = x + 1;
      if (start < x1)
        PUSH(y, start, x1 - 1, -dy);    /* leak on left? */
      x = x1 + 1;
    }
    do {
      for (x1 = x; x <= maxx && getpixel(x, y) != b; x++)
        _putpixel(x, y);
      PUSH(y, start, x - 1, dy);
      if (x > x2 + 1)
        PUSH(y, x2 + 1, x - 1, -dy);    /* leak on right? */
      for (x++; x <= x2 && getpixel(x, y) == b; x++)
        ;
      start = x;
    }
    while (x <= x2);
  }
}

// -----

void getarccoords (struct arccoordstype *arccoords)
{
  arccoords->x = last_arc.x;
  arccoords->y = last_arc.y;
  arccoords->xstart = last_arc.xstart;
  arccoords->ystart = last_arc.ystart;
  arccoords->xend = last_arc.xend;
  arccoords->yend = last_arc.yend;
}

// -----

void getaspectratio (int *xasp, int *yasp)
{
  *xasp = 10000;
  *yasp = 10000;
}

// -----

int getbkcolor (void)
{
  return bg_color;
}

// -----

int getch (void)
{
  SDL_Event event;

  //if (last_key) // kbhit () was there
    //return last_key;
  
  while (1)
    while (SDL_PollEvent (&event))
      if (event.type == SDL_KEYDOWN)
	return event.key.keysym.sym;
}

// -----

int getcolor (void)
{
  return fg_color;
}

// -----

struct palettetype *getdefaultpalette (void)
{
  return &pal;
}

// -----

char *getdrivername (void)
{
  return ("SDL_bgi");
}

// -----

int getevent (void)
{
  SDL_Event event;
  
  // wait for an event
  while (1) {
    while (SDL_PollEvent(&event))
    
      switch (event.type) {
      
      case SDL_MOUSEBUTTONDOWN:
	mouse_x = event.button.x;
	mouse_y = event.button.y;
	return event.button.button;
	break;
      
      case SDL_KEYDOWN:
	mouse_x = mouse_y = -1;
	return event.key.keysym.sym;
	break;
    
      default:
	; 
      }
  }
}

// -----

void getfillpattern (char *pattern)
{
  // no pattern!
  ;
}

// -----

void getfillsettings (struct fillsettingstype *fillinfo)
{
  fillinfo->pattern = SOLID_FILL;
  fillinfo->color = WHITE;
}

// -----

int getgraphmode (void)
{
  return gm;
}

// -----

void getimage (int left, int top, int right, int bottom, void *bitmap)
{
  Uint32 bitmap_w, bitmap_h, *tmp;
  int i = 2, x, y;

  // bitmap has already been malloc()'ed by the user.
  tmp = bitmap;
  bitmap_w = right - left + 1;
  bitmap_h = bottom - top + 1;
  
  // copy width and height to the beginning of bitmap
  memcpy (tmp, &bitmap_w, sizeof (Uint32));
  memcpy (tmp + 1, &bitmap_h, sizeof (Uint32));

  // copy image to bitmap - mind the viewport!
  for (y = top + vp.top; y <= bottom + vp.top; y++)
    for (x = left + vp.left; x <= right + vp.left; x++)
      tmp [i++] = getpixel_raw (x, y);
}

// -----

void getlinesettings (struct linesettingstype *lineinfo)
{
  lineinfo->linestyle = SOLID_LINE;
  lineinfo->upattern = 1;
  lineinfo->thickness = 1;
}

// -----

int getmaxcolor (void)
{
  return WHITE;
}

// -----

int getmaxmode (void)
{
  return X11_FULLSCREEN;
}

// -----

int getmaxx ()
{
  return maxx;
}

// -----

int getmaxy ()
{
  return maxy;
}

// -----

char *getmodename (int mode_number)
{
  switch (mode_number) {
    
  case 1:
    return "X11_CGAHI";
    break;
    
  case 2:
    return "X11_EGA";
    break;
    
  case 3:
    return "X11_VGA";
    break;

  case 4:
    return "X11_HERC";
    break;
    
  case 5:
    return "X11_PC3270";
    break;

  case 7:
    return "X11_1024x768";
    break;

  case 8:
    return "X11_1152x900";
    break;
    
  case 9:
    return "X11_1280x1024";
    break;

  case 10:
    return "X11_1366x768";
    break;
    
  case 11:
    return "X11_USER";
    break;
  
  case 12:
    return "X11_FULLSCREEN";
    break;
  
  default:
  case 6:
    return "X11_800x600";
    break;
  
  }

}

// -----

void getmoderange (int graphdriver, int *lomode, int *himode)
{
  // return dummy values
  *lomode = 0;
  *himode = 0;
}

// -----

void getpalette (struct palettetype *palette)
{
  int i;
  
  for (i = 0; i <= MAXCOLORS; i++)
    palette->colors[i] = pal.colors[i];
}

// -----

int getpalettesize (struct palettetype *palette)
{
  return 1 + MAXCOLORS + 2 + PALETTE_SIZE;
}

// -----

Uint32 getpixel_raw (int x, int y)
{
  int bpp = BGI->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)BGI->pixels + y * BGI->pitch + x * bpp;

  switch(bpp) {
  case 1:
    return *p;
    break;
    
  case 2:
    return *(Uint16 *)p;
    break;

  case 3:
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
    break;
    
  case 4:
    return *(Uint32 *)p;
    break;
    
  default:
    return 0;
  }
}

// -----

unsigned int getpixel (int x, int y)
{
  int col;
  Uint32 tmp;
  
  x += vp.left;
  y += vp.top;
  
  // really needed?
  // SDL_LockSurface (BGI);
  tmp = getpixel_raw (x, y) << 8 | 0xff;
  // SDL_UnlockSurface (BGI);
  // find it
  for (col = BLACK; col < WHITE + 1; col++)
    if (tmp == palette[col])
      return col;
  // if it's not a BGI color, just return the 0xRRGGBBAA value
  return tmp;
}

// -----

void gettextsettings (struct textsettingstype *texttypeinfo)
{
  texttypeinfo->font = txt_style.font;
  texttypeinfo->direction = txt_style.direction;
  texttypeinfo->charsize = txt_style.charsize;
  texttypeinfo->horiz = txt_style.horiz;
  texttypeinfo->vert = txt_style.vert;
}

// -----

void getviewsettings (struct viewporttype *viewport)
{
  viewport->left = vp.left;
  viewport->top = vp.top;
  viewport->right = vp.right;
  viewport->bottom = vp.bottom;
  viewport->clip = vp.clip;
}

// -----

int getx (void)
{
  return cp_x;
}

// -----

int gety (void)
{
  return cp_y;
}

// -----

char *grapherrormsg (int errorcode)
{
  return NULL;
}

// -----

void graphdefaults (void)
{
  int i;
  
  initpalette ();

  // initialise the graphics writing mode
  writemode = COPY_PUT;
  
  // initialise the viewport
  vp.left = 0;
  vp.top = 0;
  
  vp.right = getmaxx ();
  vp.bottom = getmaxy ();
  vp.clip = 0;
  
  // initialise the CP
  cp_x = 0;
  cp_y = 0;
  
  // initialise the text settings
  txt_style.font = DEFAULT_FONT;
  txt_style.direction = HORIZ_DIR;
  txt_style.charsize = 1;
  txt_style.horiz = LEFT_TEXT;
  txt_style.vert = TOP_TEXT;
  
  // initialise the fill settings
  fill_style.pattern = SOLID_FILL;
  fill_style.color = WHITE;
  
  // initialise the line settings
  line_style.linestyle = SOLID_LINE;
  line_style.upattern = SOLID_FILL;
  line_style.thickness = NORM_WIDTH;

  // initialise the palette
  pal.size = 1 + MAXCOLORS;
  for (i = 0; i < MAXCOLORS + 1; i++)
    pal.colors[i] = i;
 
}

// -----

int graphresult (void)
{
  return grOk;
}

// -----

unsigned imagesize (int left, int top, int right, int bottom)
{
  return 2 * sizeof(Uint32) + // witdth, height
    (right - left + 1) * (bottom - top + 1) * sizeof (Uint32);
}

// -----

void initgraph (int *graphdriver, int *graphmode, char *pathtodriver)
{
  
  fast_mode = 0;     // BGI compatibility

  // the graphics driver parameter is ignored and is always
  // set to X11; graphics modes may vary

  if (NULL != graphmode)
    gm = *graphmode;
  else
    gm = X11_800x600;
  
  switch (gm) {
    
  case X11_640x480:
    initwindow (640, 480);
    break;
    
  case X11_1024x768:
    initwindow (1024, 768);
    break;
    
  case X11_1152x900:
    initwindow (1152, 900);
    break;

  case X11_1280x1024:
    initwindow (1280, 1024);
    break;

  case X11_1366x768:
    initwindow (1366, 768);
    break;

  case X11_FULLSCREEN:
    initwindow (0, 0);
    break;
  
  default:
  case X11_800x600:
    initwindow (800, 600);
    break;
  
  } // switch
  
}

// -----

void initpalette (void)
{
  int i;
  
  for (i = BLACK; i < WHITE + 1; i++)
    palette[i] = bgi_palette[i];
}

// -----

void initwindow (int width, int height)
{
  const SDL_VideoInfo *info;
  
  SDL_Init (SDL_INIT_VIDEO);
  atexit (SDL_Quit);
  info = SDL_GetVideoInfo();
  if ( (0 == width) || (0 == height) ) // fullscreen
    BGI = SDL_SetVideoMode (info->current_w,
			    info->current_h, 
			    0, // hardware bpp
			    SDL_HWSURFACE | 
			    SDL_FULLSCREEN | 
			    SDL_DOUBLEBUF);
  else
    BGI = SDL_SetVideoMode (width, height, 0, SDL_SWSURFACE);
  if (BGI == NULL ) {
    fprintf (stderr, "Could not initialize SDL: %s\n", SDL_GetError ());
    exit (1);
  }
  
  maxx = BGI->w - 1;
  maxy = BGI->h - 1;
  SDL_WM_SetCaption ("SDL_bgi", "SDL_bgi");
  SDL_EnableKeyRepeat(200, 30);
  
  // initialise BGI colors - don't use SDL_MapRGB(), but 0xrrggbbaa
  graphdefaults ();

}

// -----

int installuserdriver (char *name, int *detect)
{
  return 0;
}

// -----

int installuserfont (char *name)
{
  return 0;
}

// -----

int IS_BGI_COLOR (int color)
{
  // the color argument is redundant
  return ! rgb_mode;
}

// -----

int kbhit (void)
{
  // this function does not work correctly.

  SDL_Event event;

  while (SDL_PollEvent (&event)) {
    
    if (event.type == SDL_KEYDOWN) {
      last_key = event.key.keysym.sym;
      return 1;
    }
  }
  return 0;
}

// -----

void line (int x1, int y1, int x2, int y2)
{
  
  x1 += vp.left;
  y1 += vp.top;
  x2 += vp.left;
  y2 += vp.top;
  
  // it exposes a bug in Ubuntu's oldish SDL_gfx!
  // thickLineColor (BGI, x1, y1, x2, y2, line_style.thickness,
  // 		  palette[fg_color]);

  lineColor (BGI, x1, y1, x2, y2, palette[fg_color]);
  
  if (! fast_mode)
    updaterect (x1, y1, x2, y2);
}

// -----

void linerel (int dx, int dy)
{
  line (cp_x, cp_y, cp_x + dx, cp_y + dy);
  cp_x += dx;
  cp_y += dy;
}

// -----

void lineto (int x, int y)
{
  line (cp_x, cp_y, x, y);
  cp_x = x;
  cp_y = y;
}

// -----

int mouseclick (void)
{
  SDL_Event event;
 
  while (1) {
  
    if (SDL_PollEvent (&event)) {
    
      if (event.type == SDL_MOUSEBUTTONDOWN) {
	mouse_x = event.button.x;
	mouse_y = event.button.y;
	return (event.button.button);
      }
      else
	if (event.type == SDL_MOUSEMOTION) {
	  mouse_x = event.motion.x;
	  mouse_y = event.motion.y;
	  return (WM_MOUSEMOVE);
	}
      else {
	SDL_PushEvent (&event); // don't disrupt the keyboard
	return 0;
      }
      return 0;
      
    } // if
    else
      return 0;
	
  } // while
  
}

// -----

int ismouseclick (int kind)
{
  SDL_PumpEvents ();

  switch (kind) {
   
  case SDL_BUTTON_LEFT:
    return (SDL_GetMouseState (&mouse_x, &mouse_y) & SDL_BUTTON(1));
    break;
  
  case SDL_BUTTON_MIDDLE:
    return (SDL_GetMouseState (&mouse_x, &mouse_y) & SDL_BUTTON(2));
    break;
    
  case SDL_BUTTON_RIGHT:
    return (SDL_GetMouseState (&mouse_x, &mouse_y) & SDL_BUTTON(3));
    break;
  
  }
  return 0;
  
}

// -----

void getmouseclick (int kind, int *x, int *y)
{
  *x = mouse_x;
  *y = mouse_y;
}

// -----

int mousex (void)
{
  return mouse_x;
}

// -----

int mousey (void)
{
  return mouse_y;
}

// -----

void moverel (int dx, int dy)
{
  cp_x += dx;
  cp_y += dy;
}

// -----

void moveto (int x, int y)
{
  cp_x = x;
  cp_y = y;
}

// -----

void outtext (char *textstring)
{
  outtextxy (cp_x, cp_y, textstring);
  if ( (HORIZ_DIR == txt_style.direction) &&
       (LEFT_TEXT == txt_style.horiz))
    cp_x += textwidth (textstring);
}

// -----

void outtextxy (int x, int y, char *textstring)
{
  Sint32 x1 = 0, y1 = 0;
  Uint32 tw, th;
  SDL_Surface *text, *zoomed, *rotated;
  SDL_Rect rect;
  
  tw = textwidth (textstring);
  if (0 == tw)
    return;
  
  th = textheight (textstring);
  
  if (HORIZ_DIR == txt_style.direction) {
    
    if (LEFT_TEXT == txt_style.horiz)
      x1 = x;
    
    if (CENTER_TEXT == txt_style.horiz)
      x1 = x - tw / 2;
    
    if (RIGHT_TEXT == txt_style.horiz)
      x1 = x - tw;
    
    if (CENTER_TEXT == txt_style.vert)
      y1 = y - th / 2;
    
    if (TOP_TEXT == txt_style.vert)
      y1 = y;
    
    if (BOTTOM_TEXT == txt_style.vert)
      y1 = y - th;
    
  }
  else { // VERT_DIR
    
    if (LEFT_TEXT == txt_style.horiz)
      y1 = y - tw;
    
    if (CENTER_TEXT == txt_style.horiz)
      y1 = y - tw / 2;
    
    if (RIGHT_TEXT == txt_style.horiz)
      y1 = y;
    
     if (CENTER_TEXT == txt_style.vert)
      x1 = x - th / 2;
    
    if (TOP_TEXT == txt_style.vert)
      x1 = x;
    
    if (BOTTOM_TEXT == txt_style.vert)
      x1 = x - th;
    
  }
  
  x1 += vp.left;
  y1 += vp.top;
  text = SDL_CreateRGBSurface (0, 
                               font_width * strlen (textstring), 
                               font_height, 32, 0,0,0,0);
  stringRGBA (text, 0, 0, textstring,
              palette[fg_color] >> 24,
              palette[fg_color] >> 16,
              palette[fg_color] >> 8,
              255);
  
  rect.x = x1;
  rect.y = y1;
  
  zoomed = zoomSurface (text, font_mag_x, font_mag_y, 0);
  
  if (VERT_DIR == txt_style.direction)
    rotated = rotozoomSurfaceXY (zoomed, 90, 1.0, 1.0, 0);
  else
    rotated = rotozoomSurfaceXY (zoomed, 0, 1.0, 1.0, 0);
  
  SDL_BlitSurface (rotated, NULL, BGI, &rect);
  SDL_FreeSurface (text);
  SDL_FreeSurface (zoomed);
  SDL_FreeSurface (rotated);
  
  // stringColor (BGI, x1, y1, textstring, palette[fg_color]);
  
  // text should always be displayed immediately
  updaterect (x1, y1, x1 + tw, y1 + th);
}

// -----

void pieslice (int x, int y, int stangle, int endangle, int radius)
{
  int x1, y1, x2, y2;
  
  // in SDL_gfx, the angle grows clockwise
  filledPieColor (BGI, x, y, radius, -endangle, -stangle,
		  palette[fill_style.color]);
  pieColor (BGI, x, y, radius, -endangle, -stangle,
	    palette[fg_color]);
  
  if (! fast_mode) {
    x1 = x - radius;
    y1 = y - radius;
    x2 = x + radius;
    y2 = y + radius;
    updaterect (x1, y1, x2, y2);
  }
}

// -----

void putimage (int left, int top, void *bitmap, int op)
{
  Uint32 bitmap_w, bitmap_h, *tmp;
  int i = 2, x, y;
  
  tmp = bitmap;
  
  // get width and height info from bitmap
  memcpy (&bitmap_w, tmp, sizeof (Uint32));
  memcpy (&bitmap_h, tmp + 1, sizeof (Uint32));
  
  // put bitmap to the screen
  for (y = top + vp.top; y < top + bitmap_h + vp.top; y++)
    for (x = left + vp.left; x < left + bitmap_w + vp.left; x++)
      putpixel_raw (x, y, tmp[i++], op);
  
  if (! fast_mode)
    updaterect (left + vp.left, top + vp.top,
		left + vp.left + bitmap_w, top + vp.top + bitmap_h);
}

// -----

void putpixel (int x, int y, int col)
{
  int c;
  if (-1 == col) { // COLOR () set up the WHITE + 1 color
    rgb_mode = 1;
    c = WHITE + 1;
    palette[c] = tmp_color;
  }
  else {
    rgb_mode = 0;
    c = col;
  }
  x += vp.left;
  y += vp.top;
  pixelColor (BGI, x, y, palette[c]);
  if (! fast_mode)
    SDL_UpdateRect (BGI, x, y, 1, 1);
}

// -----

void rectangle (int x1, int y1, int x2, int y2)
{
  x1 += vp.left;
  y1 += vp.top;
  x2 += vp.left;
  y2 += vp.top;
  
  rectangleColor (BGI, x1, y1, x2, y2, palette[fg_color]);
  
  if (! fast_mode)
    updaterect (x1, y1, x2, y2);
}

// -----

void refresh ()
{
  // SDL_UpdateRect (BGI, 0, 0, 0, 0);
  SDL_Flip (BGI);
}

// -----

int registerbgidriver (void *driver)
{
  return 0;  
}

// -----

int registerbgifont (void *font)
{
  return 0;
}

// -----

void restorecrtmode (void)
{
  // fake it
  cleardevice ();
}

// -----

void sector (int x, int y, int stangle, int endangle,
	     int xradius, int yradius)
{
  // yradius is ignored
  pieslice (x, y, stangle, endangle, xradius);
}

// -----

void setactivepage (int page)
{
  unimplemented ("setactivepage");
}

// -----

void setallpalette (struct palettetype *palette)
{
  int i;
  
  for (i = 0; i <= MAXCOLORS; i++)
    if (palette->colors[i] != -1)
      setpalette (i, palette->colors[i]);
}

// -----

void setaspectratio (int xasp, int yasp)
{
  // ignored
  ;
}

// -----

void setbkcolor (int col)
{
  if (-1 == col) { // COLOR () set up the WHITE + 2 color
    rgb_mode = 1;
    bg_color = WHITE + 2;
    palette[bg_color] = tmp_color;
  }
  else {
    rgb_mode = 0;
    bg_color = col;
  }
}

// -----

void setbkrgbcolor (int index)
{
  bg_color = 1 + MAXCOLORS + 2 + index;
}

// -----

void setcolor (int col)
{
  if (-1 == col) { // COLOR () set up the WHITE + 1 color
    rgb_mode = 1;
    fg_color = WHITE + 1;
    palette[fg_color] = tmp_color;
  }
  else {
    rgb_mode = 0;
    fg_color = col;
  }
}

// -----

void setfillpattern (char *upattern, int color)
{
  unimplemented ("setfillpattern");
}

// -----

void setfillstyle (int pattern, int color)
{
  fill_style.pattern = pattern;
  if (-1 == color) {
    fg_color = WHITE + 1;
    palette[fg_color] = tmp_color;
    fill_style.color = fg_color;
  }
  else
    fill_style.color = color;
}

// -----

unsigned int setgraphbufsize (unsigned int mode)
{
  // ignored
  return 0;
}

// -----

void setgraphmode (int mode)
{
  ; // do nothing
}

// -----

void setlinestyle (int linestyle, unsigned upattern, int thickness)
{
  // other settings are not implemented
  // line_style.linestyle = linestyle;
  // line_style.upattern = upattern;
  line_style.thickness = thickness;
}

// -----

void setpalette (int colornum, int color)
{
  palette[colornum] = bgi_palette[color];
}

// -----

void setrgbcolor (int index)
{
  fg_color = 1 + MAXCOLORS + 2 + index;
}

// -----

void setrgbpalette (int colornum, int red, int green, int blue)
{
  palette[1 + MAXCOLORS + 2 + colornum] = 
    red << 24 | green << 16 | blue << 8 | 0xff;
}

// -----

void settextjustify (int horiz, int vert)
{
  txt_style.horiz = horiz;
  txt_style.vert = vert;
}

// -----

void settextstyle (int font, int direction, int charsize)
{
  switch (font) {
   
  case DEFAULT_FONT:
    fontptr = NULL; // default
    break;
    
  case TRIPLEX_FONT:
    fontptr = (const void *) font_7x13;
    break;
    
  case SMALL_FONT:
    fontptr = (const void *) font_5x8;
    break;
    
  case SANSSERIF_FONT:
    fontptr = (const void *) font_9x18;
    break;
    
  case GOTHIC_FONT:
  case SCRIPT_FONT:
  case SIMPLEX_FONT:
  case TRIPLEX_SCR_FONT:
  case COMPLEX_FONT:
  case EUROPEAN_FONT:
  case BOLD_FONT:
    fontptr = (const void *) font_10x20;
    break;
    
  case BIG_FONT:
    fontptr = (const void *) font_16x26;
    
  }

  // font = 0; // force 8x8
  gfxPrimitivesSetFont (fontptr, font_w[font], font_h[font]);
  font_width = font_w[font];
  font_height = font_h[font];
  txt_style.charsize = charsize;
  if (USER_CHAR_SIZE != charsize) {
    font_mag_x = (float)charsize;
    font_mag_y = (float)charsize;
  }
  
  if (VERT_DIR == direction)
    txt_style.direction = VERT_DIR;
  else
    txt_style.direction = HORIZ_DIR;
  
}

// -----

void setusercharsize (int multx, int divx, int multy, int divy)
{
  font_mag_x = (float)multx / (float)divx;
  font_mag_y = (float)multy / (float)divy;
}

// -----

void setviewport (int left, int top, int right, int bottom, int clip)
{
  SDL_Rect rect;
  
  vp.left = left;
  vp.top = top;
  vp.right = right;
  vp.bottom = bottom;
  vp.clip = clip;
  
  rect.x = left;
  rect.y = top;
  rect.w = right - left + 1;
  rect.h = bottom - top + 1;
  
  if (clip)
    SDL_SetClipRect (BGI, &rect);
  
  cp_x = 0;
  cp_y = 0;
}

// -----

void setvisualpage (int page)
{
  unimplemented ("setvisualpage");
}

// -----

void setwritemode (int mode)
{
  writemode = mode;
}

// -----

int textheight (char *textstring)
{
  return font_mag_y * font_height;
}

// -----

int textwidth (char *textstring)
{
  return (strlen (textstring) * font_width * font_mag_x);
}

// -----

int RED_VALUE (int color)
{
  // return the red component of 'color' in the extended palette
  return ((palette[1 + MAXCOLORS + 2 + color] >> 24) & 0xFF);
}

// -----

int GREEN_VALUE (int color)
{
  // return the green component of 'color' in the extended palette
  return ((palette[1 + MAXCOLORS + 2 + color] >> 16) & 0xFF);
}

// -----

