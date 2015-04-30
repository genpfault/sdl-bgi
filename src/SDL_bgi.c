// SDL_bgi.c	-*- C -*-

// A BGI (Borland Graphics Library) implementation based on SDL2.
// Easy to use, and useful for porting old programs.
// Guido Gonzato, May 2015.

#include <math.h>
#include "SDL_bgi.h"
#include "SDL_bgi_font.h"

#define VERSION 2.0.0

// stuff gets drawn here

SDL_Window   *bgi_window;
SDL_Renderer *bgi_renderer;
SDL_Texture  *bgi_texture;

static Uint32
  *bgi_vpage[VPAGES], // array of visual pages
  *bgi_activepage,    // active (= being drawn on) page, possibly hidden
  *bgi_visualpage;    // visualised page

// the palette contains the BGI colors, entries 0:MAXCOLORS;
// then three entries for temporary fg, bg, fill RGB colors allocated 
// with COLOR(); then user-defined RGB colors

static Uint32 palette[1 + MAXCOLORS + 3 + PALETTE_SIZE]; // all colors

static Uint32 bgi_palette[1 + MAXCOLORS] = { // 0 - 15
  0xff000000, // BLACK
  0xff0000ff, // BLUE
  0xff00ff00, // GREEN
  0xff00ffff, // CYAN
  0xffff0000, // RED
  0xffff00ff, // MAGENTA
  0xffa52a2a, // BROWN
  0xffd3d3d3, // LIGHTGRAY
  0xffa9a9a9, // DARKGRAY
  0xffadd8e6, // LIGHTBLUE
  0xff90ee90, // LIGHTGREEN
  0xffe0ffff, // LIGHTCYAN
  0xfff08080, // LIGHTRED
  0xffdb7093, // LIGHTMAGENTA
  0xffffff00, // YELLOW
  0xffffffff  // WHITE
};

static Uint32 
  bgi_tmp_color_argb;     // temporary color set up by COLOR()

static int
  bgi_fg_color = WHITE,   // index of BGI foreground color
  bgi_bg_color = BLACK,   // index of BGI background color
  bgi_fill_color = WHITE, // index of BGI fill color
  bgi_mouse_x,            // coordinates of last mouse click
  bgi_mouse_y, 
  bgi_font_width = 8,     // default font width and height  
  bgi_font_height = 8,
  bgi_fast_mode = 1,      // needs screen update?
  bgi_cp_x = 0,           // current position
  bgi_cp_y = 0,
  bgi_maxx,               // screen dimensions
  bgi_maxy,
  bgi_gm,                 // graphics mode
  bgi_argb_mode = 0,      // BGI or ARGB colors
  bgi_writemode,          // plotting method (COPY_PUT, XOR_PUT...)
  bgi_ap,                 // active page number
  bgi_vp,                 // visual page number
  bgi_np = 0;             // # of actual pages

static float
  bgi_font_mag_x = 1.0,  // font magnification
  bgi_font_mag_y = 1.0;

// pointer to font array. Add fonts?
static const Uint8 *fontptr = gfxPrimitivesFontdata;

static struct arccoordstype last_arc;
static struct fillsettingstype fill_style;
static struct linesettingstype line_style;
static struct textsettingstype txt_style;
static struct viewporttype vp;
static struct palettetype pal;

// utility functions

static void initpalette (void);
static void putpixel_copy (int x, int y, Uint32 pixel);
static void putpixel_xor  (int x, int y, Uint32 pixel);
static void putpixel_and  (int x, int y, Uint32 pixel);
static void putpixel_or   (int x, int y, Uint32 pixel);
static void putpixel_not  (int x, int y, Uint32 pixel);
static Uint32 getpixel_raw (int, int);

static void line_copy (int, int, int, int);
static void line_xor  (int, int, int, int);
static void line_and  (int, int, int, int);
static void line_or   (int, int, int, int);
static void line_not  (int, int, int, int);

static void line_fast  (int, int, int, int);
static void updaterect (int, int, int, int);

static void swap_if_greater (int *, int *);
static int  octant (int, int);

// -----

void unimplemented (char *msg)
{
  fprintf (stderr, "%s() is not yet implemented.\n", msg);
}

// -----

#define PI_CONV (3.1415926 / 180.0)

void arc (int x, int y, int stangle, int endangle, int radius)
{
  // quick and dirty for now, Bresenham-based later (maybe)
  int angle;
  
  if (0 == radius)
    return;
  
  if (endangle < stangle)
    endangle += 360;
  
  last_arc.x = x;
  last_arc.y = y;
  last_arc.xstart = x + (radius * cos (stangle * PI_CONV));
  last_arc.ystart = y - (radius * sin (stangle * PI_CONV));
  last_arc.xend = x + (radius * cos (endangle * PI_CONV));
  last_arc.yend = y - (radius * sin (endangle * PI_CONV));
  
  for (angle = stangle; angle < endangle; angle++)
    line_fast (x + (radius * cos (angle * PI_CONV)),
	       y - (radius * sin (angle * PI_CONV)),
	       x + (radius * cos ((angle+1) * PI_CONV)),
	       y - (radius * sin ((angle+1) * PI_CONV)));
  
  if (! bgi_fast_mode)
    refresh ();
  
} // arc ()

// -----

void bar3d (int left, int top, int right, int bottom, int depth, int topflag)
{
  Uint32 tmp, tmpcolor;
  
  tmp = bgi_fg_color;
  
  if (EMPTY_FILL == fill_style.pattern)
    tmpcolor = bgi_bg_color;
  else // all other styles
    tmpcolor = fill_style.color;
  
  setcolor (tmpcolor); // fill
  bar (left, top, right, bottom);
  setcolor (tmp); // outline
  if (depth > 0) {
    line_fast (left, top, left + depth, top - depth);
    line_fast (left + depth, top - depth, right + depth, top - depth);
    line_fast (right, top, right + depth, top - depth);
    line_fast (right, bottom, right + depth, bottom - depth);
    line_fast (right + depth, bottom - depth, right + depth, top - depth);
  }
  rectangle (left, top, right, bottom);
  
  // topflag - what should I do with it?
  
  if (! bgi_fast_mode)
    refresh ();
  
} // bar3d ()

// -----

void bar (int left, int top, int right, int bottom)
{
  int
    y, 
    tmp, tmpcolor, tmpthickness;
  
  tmp = bgi_fg_color;
  
  if (EMPTY_FILL == fill_style.pattern)
    tmpcolor = bgi_bg_color;
  else // all other styles
    tmpcolor = fill_style.color;
  
  setcolor (tmpcolor);
  tmpthickness = line_style.thickness;
  line_style.thickness = NORM_WIDTH;
  
  for (y = top; y <= bottom; y++)
    line_fast (left, y, right, y);
  
  setcolor (tmp);
  line_style.thickness = tmpthickness;
  
  if (! bgi_fast_mode)
    refresh ();
  
} // bar ()

// -----

int BLUE_VALUE (int color)
{
  // return the blue component of 'color' in the extended palette
  return (palette[color] & 0xFF);

} // BLUE_VALUE ()

// -----

void circle (int x, int y, int radius)
{
  // a Bresenham approach would be better, but let's use the basic
  // algorithm for BGI compatibility (line thickness)
  
  arc (x, y, 0, 360, radius);

} // circle ();

// -----

void cleardevice (void)
{
  int x, y;

  bgi_cp_x = bgi_cp_y = 0;
  
  for (x = 0; x < bgi_maxx + 1; x++)
    for (y = 0; y < bgi_maxy + 1; y++)
      bgi_activepage [y * (bgi_maxx + 1) + x] = palette[bgi_bg_color];
  
  if (! bgi_fast_mode)
    refresh ();
  
} // cleardevice ()

// -----

void clearviewport (void)
{
  int x, y;
  
  bgi_cp_x = bgi_cp_y = 0;
  
  for (x = vp.left; x < vp.right + 1; x++)
    for (y = vp.top; y < vp.bottom + 1; y++)
      bgi_activepage [y * (bgi_maxx + 1) + x] = palette[bgi_bg_color];
  
  if (! bgi_fast_mode)
    refresh ();

} // clearviewport ()

// -----

void closegraph (void)
{
  int page;
  
  for (page = 0; page < bgi_np; page++)
    free (bgi_vpage[page]);
  SDL_DestroyWindow (bgi_window);
  SDL_Quit ();

} // closegraph ()

// -----

int COLOR (int r, int g, int b)
{
  // set up the temporary color
  bgi_tmp_color_argb = 0xff000000 | r << 16 | g << 8 | b;
  return -1;
  
} // COLOR ()

// -----

void delay (int msec)
{
  SDL_Delay (msec);
} // delay ()

// -----

void detectgraph (int *graphdriver, int *graphmode)
{
  *graphdriver = SDL;
  *graphmode = SDL_FULLSCREEN;
} // detectgraph ()

// -----

void drawpoly (int numpoints, int *polypoints)
{
  int n;

  for (n = 0; n < numpoints - 1; n++)
    line_fast (polypoints[2*n], polypoints[2*n + 1],
	       polypoints[2*n + 2], polypoints[2*n + 3]);
  // close the polygon
  line_fast (polypoints[2*n], polypoints[2*n + 1],
	     polypoints[0], polypoints[1]);
  
  if (! bgi_fast_mode)
    refresh ();

} // drawpoly ()

// -----

void swap_if_greater (int *x1, int *x2)
{
  int tmp;
  
  if (*x1 > *x2) {
    tmp = *x1;
    *x1 = *x2;
    *x2 = tmp;
  }

} // swap_if_greater ()

// -----

#if 0
int is_in_range (x, x1, x2)
{
  swap_if_greater (&x1, &x2);
  return ( x >= x1 && x <= x2);
}
#endif

// -----

void ellipse (int x, int y, int stangle, int endangle, 
              int xradius, int yradius)
{
  // quick and dirty for now, Bresenham-based later.
  int angle;
  
  if (0 == xradius && 0 == yradius)
    return;

  if (endangle < stangle)
    endangle += 360;
  
  // needed?
  last_arc.x = x;
  last_arc.y = y;
  
  for (angle = stangle; angle < endangle; angle++)
    line_fast (x + (xradius * cos (angle * PI_CONV)),
	       y - (yradius * sin (angle * PI_CONV)),
	       x + (xradius * cos ((angle+1) * PI_CONV)),
	       y - (yradius * sin ((angle+1) * PI_CONV)));
  
  if (! bgi_fast_mode)
    refresh ();

} // ellipse ()

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
} // event ()

// -----

void fillellipse (int cx, int cy, int xradius, int yradius)
{
  // from "A Fast Bresenham Type Algorithm For Drawing Ellipses"
  // by John Kennedy

  int
    x, y,
    xchange, ychange,
    ellipseerror,
    TwoASquare, TwoBSquare,
    StoppingX, StoppingY,
    tmpcolor = bgi_fg_color;

  if (0 == xradius && 0 == yradius)
    return;
  
  TwoASquare = 2*xradius*xradius;
  TwoBSquare = 2*yradius*yradius;
  x = xradius;
  y = 0;
  xchange = yradius*yradius*(1 - 2*xradius);
  ychange = xradius*xradius;
  ellipseerror = 0;
  StoppingX = TwoBSquare*xradius;
  StoppingY = 0;
  setcolor (fill_style.color);
  
  while (StoppingX >= StoppingY) {
    
    // 1st set of points, y' > -1
 
    line_fast (cx + x, cy - y, cx - x, cy - y);
    line_fast (cx - x, cy + y, cx + x, cy + y);
    y++;
    StoppingY += TwoASquare;
    ellipseerror += ychange;
    ychange +=TwoASquare; 
    
    if ((2*ellipseerror + xchange) > 0 ) {
      x--;
      StoppingX -= TwoBSquare;
      ellipseerror +=xchange;
      xchange += TwoBSquare;
    }
  } // while
  
  // 1st point set is done; start the 2nd set of points
  
  x = 0;
  y = yradius;
  xchange = yradius*yradius;
  ychange = xradius*xradius*(1 - 2*yradius);
  ellipseerror = 0;
  StoppingX = 0;
  StoppingY = TwoASquare*yradius;
  
  while (StoppingX <= StoppingY ) {
    
    // 2nd set of points, y' < -1
    
    line_fast (cx + x, cy - y, cx - x, cy - y);
    line_fast (cx - x, cy + y, cx + x, cy + y);
    x++;
    StoppingX += TwoBSquare;
    ellipseerror += xchange;
    xchange +=TwoBSquare;
    if ((2*ellipseerror + ychange) > 0) {
      y--,
	StoppingY -= TwoASquare;
      ellipseerror += ychange;
      ychange +=TwoASquare;
    }
  }
  
  setcolor (tmpcolor);
  
  if (! bgi_fast_mode)
    refresh ();  
  
} // fillellipse ()

// -----

void fillpoly (int numpoints, int *polypoints)
{
  // !!! only convex polygons for now
  int
    tmpcol = bgi_fg_color,
    x, y;
  
  // setcolor (fill_style.color);
  drawpoly (numpoints, polypoints);
  
  if (numpoints < 4) { // triangle - find centroid
    x = (polypoints[0] + polypoints[2] + polypoints[4]) / 3;
    y = (polypoints[1] + polypoints[3] + polypoints[5]) / 3;
  }
  else {
    x = (polypoints[0] + polypoints[4]) / 2; // 1st and 3rd vertices
    y = (polypoints[1] + polypoints[5]) / 2;
  }
    
  // !!! won't work for generic polygon!
  floodfill (x, y, fill_style.color);
  setcolor (tmpcol);
  
  if (! bgi_fast_mode)
    refresh ();

} // fillpoly ()

// -----

// the following code is adapted from "A Seed Fill Algorithm"
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
    if (sp < stack+STACKSIZE && Y+(DY) >= 0 && Y+(DY) <= bgi_maxy ) \
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
      for (x1 = x; x <= bgi_maxx && getpixel(x, y) != b; x++)
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
  
  if (! bgi_fast_mode)
    refresh ();
  
} // floodfill ()

// -----

int getactivepage (void)
{
  return (bgi_ap);
} // getactivepage ()

// -----

void getarccoords (struct arccoordstype *arccoords)
{
  arccoords->x = last_arc.x;
  arccoords->y = last_arc.y;
  arccoords->xstart = last_arc.xstart;
  arccoords->ystart = last_arc.ystart;
  arccoords->xend = last_arc.xend;
  arccoords->yend = last_arc.yend;
} // getarccoords ()

// -----

void getaspectratio (int *xasp, int *yasp)
{
  *xasp = 10000;
  *yasp = 10000;
} // getaspectratio ()

// -----

int getbkcolor (void)
{
  return bgi_bg_color;
} // getbkcolor ()

// -----

int getch (void)
{
  SDL_Event event;

  while (1)
    while (SDL_PollEvent (&event))
      if (event.type == SDL_KEYDOWN)
        return event.key.keysym.sym;
} // getch ()

// -----

int getcolor (void)
{
  return bgi_fg_color;
} // getcolor ()

// -----

struct palettetype *getdefaultpalette (void)
{
  return &pal;
} // getdefaultpalette ()

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
        bgi_mouse_x = event.button.x;
        bgi_mouse_y = event.button.y;
        return event.button.button;
        break;
      
      case SDL_KEYDOWN:
        bgi_mouse_x = bgi_mouse_y = -1;
        return event.key.keysym.sym;
        break;
    
      default:
        ; 
      }
  }
} // getevent ()

// -----

void getfillpattern (char *pattern)
{
  unimplemented ("getfillpattern");
} // getfillpattern ()

// -----

void getfillsettings (struct fillsettingstype *fillinfo)
{
  fillinfo->pattern = SOLID_FILL;
  fillinfo->color = WHITE;
} // getfillsettings ()

// -----

int getgraphmode (void)
{
  return bgi_gm;
} // getgraphmode ()

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

  // copy image to bitmap
  for (y = top + vp.top; y <= bottom + vp.top; y++)
    for (x = left + vp.left; x <= right + vp.left; x++)
      tmp [i++] = getpixel_raw (x, y);
} // getimage ()

// -----

void getlinesettings (struct linesettingstype *lineinfo)
{
  lineinfo->linestyle = SOLID_LINE; // !!! for now
  lineinfo->upattern = 1;
  lineinfo->thickness = 1;
} // getlinesettings ();

// -----

int getmaxcolor (void)
{
  // !!! what if RGB mode?
  return WHITE;
} // getmaxcolor ()

// -----

int getmaxmode (void)
{
  return SDL_FULLSCREEN;
} // getmaxmode ()

// -----

int getmaxx ()
{
  return bgi_maxx;
} // getmaxx ()

// -----

int getmaxy ()
{
  return bgi_maxy;
} // getmaxy ()

// -----

char *getmodename (int mode_number)
{
  switch (mode_number) {
    
  case 1:
    return "SDL_CGAHI";
    break;
    
  case 2:
    return "SDL_EGA";
    break;
    
  case 3:
    return "SDL_VGA";
    break;

  case 4:
    return "SDL_HERC";
    break;
    
  case 5:
    return "SDL_PC3270";
    break;

  case 7:
    return "SDL_1024x768";
    break;

  case 8:
    return "SDL_1152x900";
    break;
    
  case 9:
    return "SDL_1280x1024";
    break;

  case 10:
    return "SDL_1366x768";
    break;
    
  case 11:
    return "SDL_USER";
    break;
  
  case 12:
    return "SDL_FULLSCREEN";
    break;
  
  default:
  case 6:
    return "SDL_800x600";
    break;
    
  } // switch

} // getmodename ()

// -----

void getmoderange (int graphdriver, int *lomode, int *himode)
{
  // return dummy values
  *lomode = 0;
  *himode = 0;
} // getmoderange ()

// -----

void getpalette (struct palettetype *palette)
{
  int i;
  
  for (i = 0; i <= MAXCOLORS; i++)
    palette->colors[i] = pal.colors[i];

} // getpalette ()

// -----

int getpalettesize (struct palettetype *palette)
{
  return 1 + MAXCOLORS + 2 + PALETTE_SIZE;
} // getpalettesize ()

// -----

Uint32 getpixel_raw (int x, int y)
{
  return bgi_activepage [y * (bgi_maxx + 1) + x];
} // getpixel_raw ()

// -----

unsigned int getpixel (int x, int y)
{
  int col;
  Uint32 tmp;
  
  x += vp.left;
  y += vp.top;
  
  tmp = getpixel_raw (x, y);
  // now find the colour
  for (col = BLACK; col < WHITE + 1; col++)
    if (tmp == palette[col])
      return col;
  // if it's not a BGI color, just return the 0xAARRGGBB value
  return tmp;
} // getpixel ()

// -----

void gettextsettings (struct textsettingstype *texttypeinfo)
{
  texttypeinfo->font = txt_style.font;
  texttypeinfo->direction = txt_style.direction;
  texttypeinfo->charsize = txt_style.charsize;
  texttypeinfo->horiz = txt_style.horiz;
  texttypeinfo->vert = txt_style.vert;
} // gettextsettings ()

// -----

void getviewsettings (struct viewporttype *viewport)
{
  viewport->left = vp.left;
  viewport->top = vp.top;
  viewport->right = vp.right;
  viewport->bottom = vp.bottom;
  viewport->clip = vp.clip;
} // getviewsettings ()

// -----

int getvisualpage (void)
{
  return (bgi_vp);
} // getvisualpage ()

// -----

int getx (void)
{
  return bgi_cp_x;
} // getx ()

// -----

int gety (void)
{
  return bgi_cp_y;
} // gety ()

// -----

char *grapherrormsg (int errorcode)
{
  // pointless
  return NULL;
} // grapherrormsg ()

// -----

void graphdefaults (void)
{
  int i;
  
  initpalette ();

  // initialise the graphics writing mode
  bgi_writemode = COPY_PUT;
  
  // initialise the viewport
  vp.left = 0;
  vp.top = 0;
  
  vp.right = bgi_maxx;
  vp.bottom = bgi_maxy;
  vp.clip = 0;
  
  // initialise the CP
  bgi_cp_x = 0;
  bgi_cp_y = 0;
  
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
 
} // graphdefaults ()

// -----

int graphresult (void)
{
  return grOk;
} // graphresult ()

// -----


unsigned imagesize (int left, int top, int right, int bottom)
{
  return 2 * sizeof(Uint32) + // witdth, height
    (right - left + 1) * (bottom - top + 1) * sizeof (Uint32);
} // imagesize ()

// -----

void initgraph (int *graphdriver, int *graphmode, char *pathtodriver)
{
  bgi_fast_mode = 0;     // BGI compatibility
  
  // the graphics driver parameter is ignored and is always
  // set to SDL; graphics modes may vary; the path parameter is
  // also ignored.

  if (NULL != graphmode)
    bgi_gm = *graphmode;
  else
    bgi_gm = SDL_800x600; // default
  
  switch (bgi_gm) {
    
  case SDL_640x480:
    initwindow (640, 480);
    break;
    
  case SDL_1024x768:
    initwindow (1024, 768);
    break;
    
  case SDL_1152x900:
    initwindow (1152, 900);
    break;

  case SDL_1280x1024:
    initwindow (1280, 1024);
    break;

  case SDL_1366x768:
    initwindow (1366, 768);
    break;

  case SDL_FULLSCREEN:
    initwindow (0, 0);
    break;
  
  default:
  case SDL_800x600:
    initwindow (800, 600);
    break;
  
  } // switch
  
} // initgraph ()

// -----

void initpalette (void)
{
  int i;
  
  for (i = BLACK; i < WHITE + 1; i++)
    palette[i] = bgi_palette[i];
} // initpalette ()

// -----

static int
  window_flag = 0;

void initwindow (int width, int height)
{
  int
    display_count = 0,
    display_index = 0,
    mode_index = 0,
    page;
  
  SDL_DisplayMode mode = 
  { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };
  
  SDL_Init (SDL_INIT_VIDEO); // initialize SDL2

  // any display available?
  if ((display_count = SDL_GetNumVideoDisplays ()) < 1) {
    SDL_Log ("SDL_GetNumVideoDisplays returned: %i\n", display_count);
    return;
  }

  // get display mode
  if (SDL_GetDisplayMode (display_index, mode_index, &mode) != 0) {
    SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
    return;       
  }
  
  bgi_maxx = mode.w - 1;
  bgi_maxy = mode.h - 1;
  window_flag = SDL_WINDOW_FULLSCREEN_DESKTOP;
  
  if (0 == bgi_fast_mode) {  // called by initgraph ()
    if (!width || !height) { // fullscreen
      bgi_maxx = mode.w - 1;
      bgi_maxy = mode.h - 1;
    }
    else {
      bgi_maxx = width - 1;
      bgi_maxy = height - 1;
      window_flag = 0;
    }
  }
  else 
    if ( (0 != width) && (0 != height) ) {
      bgi_maxx = width - 1;
      bgi_maxy = height - 1;
      window_flag = 0;
    }
    
  bgi_window = SDL_CreateWindow ("SDL_bgi",
				 SDL_WINDOWPOS_UNDEFINED, // x
				 SDL_WINDOWPOS_UNDEFINED, // y
				 bgi_maxx + 1,
				 bgi_maxy + 1,
				 window_flag);
  // is the window OK?
  if (NULL == bgi_window) {
    printf ("Could not create window: %s\n", SDL_GetError ());
    return;
  }

  bgi_renderer = SDL_CreateRenderer (bgi_window, -1, SDL_RENDERER_SOFTWARE);
  bgi_texture = SDL_CreateTexture (bgi_renderer,
				   SDL_PIXELFORMAT_ARGB8888,
				   SDL_TEXTUREACCESS_STREAMING,
				   bgi_maxx + 1,
				   bgi_maxy + 1);

  for (page = 0; page < VPAGES; page++) {
    bgi_vpage[page] = malloc ( (bgi_maxx + 1) *
			       (bgi_maxx + 1) *
			       sizeof (Uint32));
    if (NULL == bgi_vpage[page]) {
      SDL_Log ("Can't allocate memory for visual page %d.", page);
      break;
    }
    else
      bgi_np++;
  }
  bgi_activepage = bgi_visualpage = bgi_vpage[0];
  bgi_ap = bgi_vp = 0;
  
  graphdefaults ();
  
} // initwindow ()

// -----

int installuserdriver (char *name, int *detect)
{
  return 0;
} // installuserdriver ()

// -----

int installuserfont (char *name)
{
  return 0;
} // installuserfont ()

// -----

int IS_BGI_COLOR (int color)
{
  // the color argument is redundant
  return ! bgi_argb_mode;
} // IS_BGI_COLOR ()

// -----

int kbhit (void)
{
  const Uint8 *keys;
  int i, n, pressed;
  
  SDL_PumpEvents ();
  keys = SDL_GetKeyboardState (&n);
  pressed = 0;
  
  for (i = 0; i < n; i++)
    if (keys[i])
      pressed = 1;
  
  return (pressed);
} // kbhit ()

// -----

// Bresenham's line algorithm routines (copy, xor, and, or, not)

void line_copy (int x1, int y1, int x2, int y2)
{
  int dx = abs (x2 - x1), sx = x1 < x2 ? 1 : -1;
  int dy = abs (y2 - y1), sy = y1 < y2 ? 1 : -1; 
  int err = (dx > dy ? dx : -dy) / 2, e2;
 
  for (;;) {
    putpixel_copy (x1, y1, palette[bgi_fg_color]);
    if (x1 == x2 && y1 == y2)
      break;
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y1 += sy;
    }
  } // for
  
} // line_copy ()

// -----

void line_xor (int x1, int y1, int x2, int y2)
{
  int dx = abs (x2 - x1), sx = x1 < x2 ? 1 : -1;
  int dy = abs (y2 - y1), sy = y1 < y2 ? 1 : -1; 
  int err = (dx > dy ? dx : -dy) / 2, e2;
 
  for (;;) {
    putpixel_xor (x1, y1, palette[bgi_fg_color]);
    if (x1 == x2 && y1 == y2)
      break;
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y1 += sy;
    }
  } // for
  
} // line_xor ()

// -----

void line_and (int x1, int y1, int x2, int y2)
{
  int dx = abs (x2 - x1), sx = x1 < x2 ? 1 : -1;
  int dy = abs (y2 - y1), sy = y1 < y2 ? 1 : -1; 
  int err = (dx > dy ? dx : -dy) / 2, e2;
 
  for (;;) {
    putpixel_and (x1, y1, palette[bgi_fg_color]);
    if (x1 == x2 && y1 == y2)
      break;
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y1 += sy;
    }
  } // for
  
} // line_and ()

// -----

void line_or (int x1, int y1, int x2, int y2)
{
  int dx = abs (x2 - x1), sx = x1 < x2 ? 1 : -1;
  int dy = abs (y2 - y1), sy = y1 < y2 ? 1 : -1; 
  int err = (dx > dy ? dx : -dy) / 2, e2;
 
  for (;;) {
    putpixel_or (x1, y1, palette[bgi_fg_color]);
    if (x1 == x2 && y1 == y2)
      break;
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y1 += sy;
    }
  } // for
  
} // line_or ()

// -----

void line_not (int x1, int y1, int x2, int y2)
{
  int dx = abs (x2 - x1), sx = x1 < x2 ? 1 : -1;
  int dy = abs (y2 - y1), sy = y1 < y2 ? 1 : -1; 
  int err = (dx > dy ? dx : -dy) / 2, e2;
 
  for (;;) {
    putpixel_not (x1, y1, palette[bgi_fg_color]);
    if (x1 == x2 && y1 == y2)
      break;
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y1 += sy;
    }
  } // for
  
} // line_not ()

// -----

static int octant (int x, int y)
{
  if (x >= 0) { // octants 1, 2, 7, 8
    
    if (y >= 0)
      return (x > y) ? 1 : 2;
    else 
      return (x > -y) ? 8 : 7;
    
  } // if (x > 0)
  
  else { // x < 0; 3, 4, 5, 6
    
    if (y >= 0)
      return (-x > y) ? 4 : 3;
    else
      return (-x > -y) ? 5 : 6;
  
  } // else
  
} // octant()

// -----

void line (int x1, int y1, int x2, int y2)
{
  int oct;
  
  // viewport
  x1 += vp.left;
  y1 += vp.top;
  x2 += vp.left;
  y2 += vp.top;
  
  switch (bgi_writemode) {
    
  case COPY_PUT:
    line_copy (x1, y1, x2, y2);
    break;
    
  case AND_PUT:
    line_and (x1, y1, x2, y2);
    break;
    
  case XOR_PUT:
    line_xor (x1, y1, x2, y2);
    break;
    
  case OR_PUT:
    line_or (x1, y1, x2, y2);
    break;
    
  case NOT_PUT:
    line_not (x1, y1, x2, y2);
    break;
    
  } // switch
  
  if (THICK_WIDTH == line_style.thickness) {
    
    oct = octant (x2 - x1, y1 - y2);
    
    switch (oct) { // draw thick line
      
    case 1:
    case 4:
    case 5:
    case 8:
      switch (bgi_writemode) {
      case COPY_PUT:
	line_copy (x1, y1 - 1, x2, y2 - 1);
	line_copy (x1, y1 + 1, x2, y2 + 1);
	break;
      case AND_PUT:
	line_and (x1, y1 - 1, x2, y2 - 1);
	line_and (x1, y1 + 1, x2, y2 + 1);
	break;
      case XOR_PUT:
	line_xor (x1, y1 - 1, x2, y2 - 1);
	line_xor (x1, y1 + 1, x2, y2 + 1);
	break;
      case OR_PUT:
	line_or (x1, y1 - 1, x2, y2 - 1);
	line_or (x1, y1 + 1, x2, y2 + 1);
	break;
      case NOT_PUT:
	line_not (x1, y1 - 1, x2, y2 - 1);
	line_not (x1, y1 + 1, x2, y2 + 1);
	break;
      } // switch
      break;
  
    case 2:
    case 3:
    case 6:
    case 7:
      switch (bgi_writemode) {
      case COPY_PUT:
	line_copy (x1 - 1, y1, x2 - 1, y2);
	line_copy (x1 + 1, y1, x2 + 1, y2);
	break;
      case AND_PUT:
	line_and (x1 - 1, y1, x2 - 1, y2);
	line_and (x1 + 1, y1, x2 + 1, y2);
	break;
      case XOR_PUT:
	line_xor (x1 - 1, y1, x2 - 1, y2);
	line_xor (x1 + 1, y1, x2 + 1, y2);
	break;
      case OR_PUT:
	line_or (x1 - 1, y1, x2 - 1, y2);
	line_or (x1 + 1, y1, x2 + 1, y2);
	break;
      case NOT_PUT:
	line_not (x1 - 1, y1, x2 - 1, y2);
	line_not (x1 + 1, y1, x2 + 1, y2);
	break;
      } // switch
      break;
      
    } // switch
    
  } // if (THICK_WIDTH...)
  
  if (! bgi_fast_mode)
    refresh ();

} // line ()

// -----

void line_fast (int x1, int y1, int x2, int y2)
{
  int 
    fastmode = bgi_fast_mode;
  
  bgi_fast_mode = 1; // draw if fast mode
  line (x1, y1, x2, y2);
  bgi_fast_mode = fastmode;
  
} // line_fast ()

// -----

void linerel (int dx, int dy)
{
  line (bgi_cp_x, bgi_cp_y, bgi_cp_x + dx, bgi_cp_y + dy);
  bgi_cp_x += dx;
  bgi_cp_y += dy;
} // linerel ()

// -----

void lineto (int x, int y)
{
  line (bgi_cp_x, bgi_cp_y, x, y);
  bgi_cp_x = x;
  bgi_cp_y = y;
} // lineto ()

// -----

int mouseclick (void)
{
  SDL_Event event;

  while (1) {
  
    if (SDL_PollEvent (&event)) {
    
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        bgi_mouse_x = event.button.x;
        bgi_mouse_y = event.button.y;
        return (event.button.button);
      }
      else
        if (event.type == SDL_MOUSEMOTION) {
          bgi_mouse_x = event.motion.x;
          bgi_mouse_y = event.motion.y;
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
  
} // mouseclick ()

// -----

int ismouseclick (int kind)
{
  SDL_PumpEvents ();

  switch (kind) {
   
  case SDL_BUTTON_LEFT:
    return (SDL_GetMouseState (&bgi_mouse_x, &bgi_mouse_y) & SDL_BUTTON(1));
    break;
  
  case SDL_BUTTON_MIDDLE:
    return (SDL_GetMouseState (&bgi_mouse_x, &bgi_mouse_y) & SDL_BUTTON(2));
    break;
    
  case SDL_BUTTON_RIGHT:
    return (SDL_GetMouseState (&bgi_mouse_x, &bgi_mouse_y) & SDL_BUTTON(3));
    break;
  
  }
  return 0;
} // ismouseclick ()

// -----

void getmouseclick (int kind, int *x, int *y)
{
  *x = bgi_mouse_x;
  *y = bgi_mouse_y;
} // getmouseclick ()

// -----

int mousex (void)
{
  return bgi_mouse_x;
} // mousex ()

// -----

int mousey (void)
{
  return bgi_mouse_y;
} // mousey ()

// -----

void moverel (int dx, int dy)
{
  bgi_cp_x += dx;
  bgi_cp_y += dy;
} // moverel ()

// -----

void moveto (int x, int y)
{
  bgi_cp_x = x;
  bgi_cp_y = y;
} // moveto ()

// -----

void _bar (int left, int top, int right, int bottom)
{
  int tmp, y;
  
  // like bar (), but uses bgi_fg_color
  
  tmp = bgi_fg_color;
  // setcolor (bgi_fg_color);
  for (y = top; y <= bottom; y++)
    line_fast (left, y, right, y);
  
  setcolor (tmp);
} // _bar ()

// -----

void drawchar (unsigned char ch)
{
  unsigned char i, j, k;
  int x, y, tmp;

  tmp = bgi_bg_color;
  bgi_bg_color = bgi_fg_color; // for bar ()
  setcolor (bgi_bg_color);
  
  // for each of the 8 bytes that make up the font

  for (i = 0; i < 8; i++) {
    
    k = fontptr[8*ch + i];
    
    // scan horizontal line
    
    for (j = 0; j < 8; j++)
      
      if ( (k << j) & 0x80) { // bit set to 1
	if (HORIZ_DIR == txt_style.direction) {
	  x = bgi_cp_x + j * bgi_font_mag_x;
	  y = bgi_cp_y + i * bgi_font_mag_y;
	  // putpixel (x, y, bgi_fg_color);
	  _bar (x, y, x + bgi_font_mag_x - 1, y + bgi_font_mag_y - 1);
	}
        else {
	  x = bgi_cp_x + i * bgi_font_mag_y;
	  y = bgi_cp_y - j * bgi_font_mag_x;
	  // putpixel (bgi_cp_x + i, bgi_cp_y - j, bgi_fg_color);
	  _bar (x, y, x + bgi_font_mag_x - 1, y + bgi_font_mag_y - 1);
	}
      }
  }
  
  if (HORIZ_DIR == txt_style.direction)
    bgi_cp_x += 8*bgi_font_mag_x;
  else
    bgi_cp_y -= 8*bgi_font_mag_y;

  bgi_bg_color = tmp;
} // drawchar ()

// -----

void outtext (char *textstring)
{
  outtextxy (bgi_cp_x, bgi_cp_y, textstring);
  if ( (HORIZ_DIR == txt_style.direction) &&
       (LEFT_TEXT == txt_style.horiz))
    bgi_cp_x += textwidth (textstring);
} // outtext ()

// -----

void outtextxy (int x, int y, char *textstring)
{
  int i, x1 = 0, y1 = 0;
  int tw, th;
  
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
      y1 = y;
    
    if (CENTER_TEXT == txt_style.horiz)
      y1 = y + tw / 2;
    
    if (RIGHT_TEXT == txt_style.horiz)
      y1 = y + tw;
    
    if (CENTER_TEXT == txt_style.vert)
      x1 = x - th / 2;
    
    if (TOP_TEXT == txt_style.vert)
      x1 = x;
    
    if (BOTTOM_TEXT == txt_style.vert)
      x1 = x - th;
    
  } // VERT_DIR

  moveto (x1, y1);
  for (i = 0; i < strlen (textstring); i++)
    drawchar (textstring[i]);
  
  if (! bgi_fast_mode)
    refresh ();
  
} // outtextxy ()

// -----

void pieslice (int x, int y, int stangle, int endangle, int radius)
{
  // quick and dirty for now, Bresenham-based later.
  int angle, tmpcolor;
  
  if (0 == radius || stangle == endangle)
    return;
  
  if (endangle < stangle)
    endangle += 360;
  
  if (0 == radius)
    return;
  
  last_arc.x = x;
  last_arc.y = y;
  last_arc.xstart = x + (radius * cos (stangle * PI_CONV));
  last_arc.ystart = y - (radius * sin (stangle * PI_CONV));
  last_arc.xend = x + (radius * cos (endangle * PI_CONV));
  last_arc.yend = y - (radius * sin (endangle * PI_CONV));
  
  for (angle = stangle; angle < endangle; angle++)
    line_fast (x + (radius * cos (angle * PI_CONV)),
	       y - (radius * sin (angle * PI_CONV)),
	       x + (radius * cos ((angle+1) * PI_CONV)),
	       y - (radius * sin ((angle+1) * PI_CONV)));
  line_fast (x, y, last_arc.xstart, last_arc.ystart);
  line_fast (x, y, last_arc.xend, last_arc.yend);
  
  tmpcolor = bgi_fg_color;
  setcolor (fill_style.color); // for floodfill ()
  angle = (stangle + endangle) / 2;
  
  // find a point within the pieslice
  /*
  putpixel (x + (radius * cos (angle * PI_CONV)) / 2,
	    y - (radius * sin (angle * PI_CONV)) / 2,
	    tmpcolor);
  refresh ();
  getch ();
   */
  floodfill (x + (radius * cos (angle * PI_CONV)) / 2,
	     y - (radius * sin (angle * PI_CONV)) / 2,
	     tmpcolor);
  setcolor (tmpcolor);
  
  if (! bgi_fast_mode)
    refresh ();
  
} // arc ()

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
  for (y = top + vp.top; y < top + vp.top + bitmap_h; y++)
    for (x = left + vp.left; x < left + vp.left + bitmap_w; x++) {
      
      switch (op) {
	
      case COPY_PUT:
	putpixel_copy (x, y, tmp[i++]);
	break;
      
      case AND_PUT:
	putpixel_and (x, y, tmp[i++]);
	break;
	
      case XOR_PUT:
	putpixel_xor (x, y, tmp[i++]);
	break;

      case OR_PUT:
	putpixel_or (x, y, tmp[i++]);
	break;
      
      case NOT_PUT:
	putpixel_not (x, y, tmp[i++]);
	break;
      } // switch
    }
  if (! bgi_fast_mode)
    refresh ();
} // putimage ()

// -----

void _putpixel (int x, int y)
{
  // line putpixel (), but not updated
  
  // viewport range is taken care of by this function only,
  // since all others use it to draw.
  
  x += vp.left;
  y += vp.top;
  
  switch (bgi_writemode) {
    
  case XOR_PUT:
    putpixel_xor  (x, y, palette[bgi_fg_color]);
    break;
    
  case AND_PUT:
    putpixel_and  (x, y, palette[bgi_fg_color]);
    break;
    
  case OR_PUT:
    putpixel_or   (x, y, palette[bgi_fg_color]);
    break;

  case NOT_PUT:
    putpixel_not  (x, y, palette[bgi_fg_color]);
    break;
    
  default:
  case COPY_PUT:
    putpixel_copy (x, y, palette[bgi_fg_color]);
    
  } // switch

} // putpixel ()

// -----

void putpixel_copy (int x, int y, Uint32 pixel)
{
  // plain putpixel - no logical operations

  if (x < 0 || x > bgi_maxx || y < 0 || y > bgi_maxy)
    return;
  
  if (1 == vp.clip)
    if (x < vp.left || x > vp.right || y < vp.top || y > vp.bottom)
      return;
  
  bgi_activepage [y * (bgi_maxx + 1) + x] = pixel;
  
} // putpixel_copy ()

// -----

void putpixel_xor (int x, int y, Uint32 pixel)
{
  // XOR'ed putpixel

  if (x < 0 || x > bgi_maxx || y < 0 || y > bgi_maxy)
    return;

  if (1 == vp.clip)
    if (x < vp.left || x > vp.right || y < vp.top || y > vp.bottom)
      return;
  
  bgi_activepage [y * (bgi_maxx + 1) + x] ^= (pixel & 0x00ffffff);
  
} // putpixel_xor ()

// -----

void putpixel_and (int x, int y, Uint32 pixel)
{
  // AND-ed putpixel
  
  if (x < 0 || x > bgi_maxx || y < 0 || y > bgi_maxy)
    return;

  if (1 == vp.clip)
    if (x < vp.left || x > vp.right || y < vp.top || y > vp.bottom)
      return;
  
  bgi_activepage [y * (bgi_maxx + 1) + x] &= (pixel & 0x00ffffff);

} // putpixel_and ()

// -----

void putpixel_or (int x, int y, Uint32 pixel)
{
  // OR-ed putpixel

  if (x < 0 || x > bgi_maxx || y < 0 || y > bgi_maxy)
    return;
  
  if (1 == vp.clip)
    if (x < vp.left || x > vp.right || y < vp.top || y > vp.bottom)
      return;
  
  bgi_activepage [y * (bgi_maxx + 1) + x] |= (pixel & 0x00ffffff);

} // putpixel_or ()

// -----

void putpixel_not (int x, int y, Uint32 pixel)
{
  // NOT-ed putpixel

  if (x < 0 || x > bgi_maxx || y < 0 || y > bgi_maxy)
    return;
  
  if (1 == vp.clip)
    if (x < vp.left || x > vp.right || y < vp.top || y > vp.bottom)
      return;

  // !!!BUG???
  bgi_activepage [y * (bgi_maxx + 1) + x] = 
    ~ bgi_activepage [y * (bgi_maxx + 1) + x];
} // putpixel_not ()

// -----

void putpixel (int x, int y, int color)
{
  int tmpcolor;
  
  x += vp.left;
  y += vp.top;
  
  // clip
  if (1 == vp.clip)
    if (x < vp.left || x > vp.right || y < vp.top || y > vp.bottom)
      return;
  
  if (-1 == color) { // COLOR () set up the WHITE + 1 color
    bgi_argb_mode = 1;
    tmpcolor = WHITE + 1;
    palette[tmpcolor] = bgi_tmp_color_argb;
  }
  else {
    bgi_argb_mode = 0;
    tmpcolor = color;
  }
  
  switch (bgi_writemode) {
    
  case XOR_PUT:
    putpixel_xor  (x, y, palette[tmpcolor]);
    break;
    
  case AND_PUT:
    putpixel_and  (x, y, palette[tmpcolor]);
    break;
    
  case OR_PUT:
    putpixel_or   (x, y, palette[tmpcolor]);
    break;

  case NOT_PUT:
    putpixel_not  (x, y, palette[tmpcolor]);
    break;
    
  default:
  case COPY_PUT:
    putpixel_copy (x, y, palette[tmpcolor]);
    
  } // switch
  
  if (! bgi_fast_mode)
    updaterect (x, y, x, y);

} // putpixel ()

// -----

void readimagefile (char *bitmapname, int x1, int y1, int x2, int y2)
{
  SDL_Surface *bm_surface;
  SDL_Texture *bm_texture;
  SDL_Rect rect;
  
  bm_surface = SDL_LoadBMP (bitmapname);
  
  if (NULL == bm_surface) // failed to load bitmap
    return;
  
  bm_texture = SDL_CreateTextureFromSurface (bgi_renderer, bm_surface);
  rect.x = x1;
  rect.y = y1;
  
  if (0 == x2 || 0 == y2) { // keep the original size
    rect.w = bm_surface->w;
    rect.h = bm_surface->h;
  }
  else {
    rect.w = x2 - x1 + 1; // stretch it to user-defined size
    rect.h = y2 - y1 + 1;
  }
  
  // clip if out of viewport?
  
  /*
  if (vp.left + x1 + rect.w > vp.right)
    rect.w = vp.right - x1 + 1;
  if (vp.top + y1 + rect.h > vp.bottom)
    rect.h = vp.bottom - y1 + 1;
  */
  
  SDL_RenderCopy (bgi_renderer, bm_texture, NULL, &rect);
  SDL_FreeSurface (bm_surface);
  SDL_DestroyTexture (bm_texture);
  // always!
  SDL_RenderPresent (bgi_renderer);
  
} // readimagefile ()

// -----

void rectangle (int x1, int y1, int x2, int y2)
{
  line_fast (x1, y1, x2, y1);
  line_fast (x2, y1, x2, y2);
  line_fast (x2, y2, x1, y2);
  line_fast (x1, y2, x1, y1);
  
  if (! bgi_fast_mode)
    refresh ();
  
} // rectangle ()

// -----

void refresh (void)
{
  SDL_UpdateTexture (bgi_texture, 
		     NULL,
		     bgi_visualpage,
		     (bgi_maxx + 1) * sizeof (Uint32));
  SDL_SetTextureBlendMode (bgi_texture, SDL_BLENDMODE_BLEND);
  SDL_RenderCopy (bgi_renderer, bgi_texture, NULL, NULL);
  SDL_RenderPresent (bgi_renderer);
} // refresh ()

// -----

int registerbgidriver (void *driver)
{
  return 0;  
} // registerbgidriver ()

// -----

int registerbgifont (void *font)
{
  return 0;
} // registerbgifont ()

// -----

void restorecrtmode (void)
{

  SDL_HideWindow (bgi_window);

} // restorecrtmode ()

// -----

void sdlbgifast (void)
{
  bgi_fast_mode = 1;
} // sdlbgifast ()

// -----

void bgislow (void)
{
  bgi_fast_mode = 0;
} // sdlbgislow ()

// -----

void sector (int x, int y, int stangle, int endangle,
             int xradius, int yradius)
{
  // quick and dirty for now, Bresenham-based later.
  int angle, tmpcolor;
  
  if (0 == xradius && 0 == yradius)
    return;
  
  if (endangle < stangle)
    endangle += 360;
  
  // really needed?
  last_arc.x = x;
  last_arc.y = y;
  last_arc.xstart = x + (xradius * cos (stangle * PI_CONV));
  last_arc.ystart = y - (yradius * sin (stangle * PI_CONV));
  last_arc.xend = x + (xradius * cos (endangle * PI_CONV));
  last_arc.yend = y - (yradius * sin (endangle * PI_CONV));
  
  for (angle = stangle; angle < endangle; angle++)
    line_fast (x + (xradius * cos (angle * PI_CONV)),
	       y - (yradius * sin (angle * PI_CONV)),
	       x + (xradius * cos ((angle+1) * PI_CONV)),
	       y - (yradius * sin ((angle+1) * PI_CONV)));
  line_fast (x, y, last_arc.xstart, last_arc.ystart);
  line_fast (x, y, last_arc.xend, last_arc.yend);
  
  tmpcolor = bgi_fg_color;
  setcolor (fill_style.color);
  angle = (stangle + endangle) / 2;
  // find a point within the sector
  floodfill (x + (xradius * cos (angle * PI_CONV)) / 2,
	     y - (yradius * sin (angle * PI_CONV)) / 2,
	     tmpcolor);
  
  if (! bgi_fast_mode)
    refresh ();

} // sector ()

// -----

void setactivepage (int page)
{
  if (page > -1 && page < bgi_np)
    bgi_activepage = bgi_vpage[page];
} // setactivepage ()

// -----

void setallpalette (struct palettetype *palette)
{
  int i;
  
  for (i = 0; i <= MAXCOLORS; i++)
    if (palette->colors[i] != -1)
      setpalette (i, palette->colors[i]);
} // setallpalette ()

// -----

void setaspectratio (int xasp, int yasp)
{
  // ignored
  return;
} // setaspectratio ()

// -----

void setbkcolor (int col)
{
  if (-1 == col) { // COLOR () set up the WHITE + 2 color
    bgi_argb_mode = 1;
    bgi_bg_color = WHITE + 2;
    palette[bgi_bg_color] = bgi_tmp_color_argb;
  }
  else {
    bgi_argb_mode = 0;
    bgi_bg_color = col;
  }
} // setbkcolor ()

// -----

void setbkrgbcolor (int index)
{
  bgi_bg_color = 1 + MAXCOLORS + 2 + index;
} // setbkrgbcolor ()

// -----

void setcolor (int col)
{
  if (-1 == col) { // COLOR () set up the WHITE + 1 color
    bgi_argb_mode = 1;
    bgi_fg_color = WHITE + 1;
    palette[bgi_fg_color] = bgi_tmp_color_argb;
  }
  else {
    bgi_argb_mode = 0;
    bgi_fg_color = col;
  }
} // setcolor ()

// -----

void setalpha (int col, Uint8 alpha)
{
  Uint32 tmp;
  
  if (-1 == col) { // COLOR () set up the WHITE + 1 color
    bgi_argb_mode = 1;
    bgi_fg_color = WHITE + 1;
  }
  else {
    bgi_argb_mode = 0;
    bgi_fg_color = col;
  }
  tmp = palette[bgi_fg_color] << 8; // get rid of alpha
  tmp = tmp >> 8;
  palette[bgi_fg_color] = ((Uint32)alpha << 24) | tmp;
  
  // bgi_alpha = 1;
  
} // setcoloralpha ()

// -----

void setfillpattern (char *upattern, int color)
{
  unimplemented ("setfillpattern");
} // setfillpattern ()

// -----

void setfillstyle (int pattern, int color)
{
  // pattern is ignored - for now

  if (-1 == color) { // COLOR () set up the WHITE + 3 color
    bgi_argb_mode = 1;
    bgi_fill_color = WHITE + 3;
    palette[bgi_fill_color] = bgi_tmp_color_argb;
    fill_style.color = bgi_fill_color;
  }
  else {
    bgi_argb_mode = 0;
    fill_style.color = color;
  }
  
} // setfillstyle ()

// -----

unsigned int setgraphbufsize (unsigned int mode)
{
  // do nothing
  return 0;
} // setgraphbufsize ()

// -----

void setgraphmode (int mode)
{
  // mode is ignored. Just create a new window
  
  SDL_ShowWindow (bgi_window);

} // setgraphmode ()

// -----

void setlinestyle (int linestyle, unsigned upattern, int thickness)
{
  // other settings are not implemented
  // line_style.linestyle = linestyle;
  // line_style.upattern = upattern;
  
  line_style.thickness = thickness;

} // setlinestyle ()

// -----

void setpalette (int colornum, int color)
{
  palette[colornum] = bgi_palette[color];
} // setpalette ()

// -----

void setrgbcolor (int index)
{
  bgi_fg_color = 1 + MAXCOLORS + 2 + index;
} // setrgbcolor ()

// -----

void setrgbpalette (int colornum, int red, int green, int blue)
{
  palette[1 + MAXCOLORS + 2 + colornum] = 
    0xff000000 | red << 16 | green << 8 | blue;
} // setrgbpalette ()

// -----

void settextjustify (int horiz, int vert)
{
  txt_style.horiz = horiz;
  txt_style.vert = vert;
} // settextjustify ()

// -----

void settextstyle (int font, int direction, int charsize)
{
  // only change the direction
  if (VERT_DIR == direction)
    txt_style.direction = VERT_DIR;
  else
    txt_style.direction = HORIZ_DIR;
  txt_style.charsize = bgi_font_mag_x = bgi_font_mag_y = charsize;
  
} // settextstyle ()

// -----

void setusercharsize (int multx, int divx, int multy, int divy)
{
  bgi_font_mag_x = (float)multx / (float)divx;
  bgi_font_mag_y = (float)multy / (float)divy;
} // setusercharsize ()

// -----

void setviewport (int left, int top, int right, int bottom, int clip)
{
  if (left < 0 || right > bgi_maxx || top < 0 || bottom > bgi_maxy)
    return;
  
  vp.left = left;
  vp.top = top;
  vp.right = right;
  vp.bottom = bottom;
  vp.clip = clip;
  
  bgi_cp_x = 0;
  bgi_cp_y = 0;
} // setviewport ()

// -----

void setvisualpage (int page)
{
  if (page > -1 && page < bgi_np) {
    // cleardevice ();
    bgi_visualpage = bgi_vpage[page];
    if (! bgi_fast_mode)
      refresh ();
  }
} // setvisualpage ()

// -----

void setwritemode (int mode)
{
  bgi_writemode = mode;
} // setwritemode ()

// -----

void swapbuffers (void)
{
  int oldv = getvisualpage( );
  int olda = getactivepage( );
  setvisualpage (olda);
  setactivepage (oldv);
} // swapbuffers ()

// -----

int textheight (char *textstring)
{
  // text height in pixels
  return bgi_font_mag_y * bgi_font_height;
} // textheight ()

// -----

int textwidth (char *textstring)
{
  // text width in pixels
  return (strlen (textstring) * bgi_font_width * bgi_font_mag_x);
} // textwidth ()

// -----

int RED_VALUE (int color)
{
  // return the red component of 'color' in the extended palette
  return ((palette[color] >> 16) & 0xFF);

} // RED_VALUE ()

// -----

int GREEN_VALUE (int color)
{
  // return the green component of 'color' in the extended palette
  return ((palette[color] >> 8) & 0xFF);

} // GREEN_VALUE ()

// -----

void updaterect (int x1, int y1, int x2, int y2)
{
  SDL_Rect rect, rect2;

  swap_if_greater (&x1, &x2);
  swap_if_greater (&y1, &y2);
  
  rect.x = x1;
  rect.y = y1;
  rect.w = x2 - x1 + 1;
  rect.h = y2 - y1 + 1;
  
  // this works: but is THIS the expected behaviour?
  rect2.x = 0;
  rect2.y = 0;
  rect2.w = x2 + 1;
  rect2.h = y2 + 1;

  SDL_UpdateTexture (bgi_texture, 
		     &rect2,
                     bgi_activepage,
                     (bgi_maxx + 1) * sizeof (Uint32));
  SDL_RenderCopy (bgi_renderer, bgi_texture, &rect, &rect);
  SDL_RenderPresent (bgi_renderer);

} // updaterect ()

// --- end of file SDL_bgi.c
