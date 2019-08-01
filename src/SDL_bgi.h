// SDL_bgi.h	-*- C -*-

// A BGI (Borland Graphics Library) implementation based on SDL2.
// Easy to use, pretty fast, and useful for porting old programs.
// Guido Gonzato, PhD
// August 1, 2019

/*
This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _SDL_BGI_H
#define _SDL_BGI_H

// SDL2 stuff
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>

#include <stdio.h>   // for fprintf()
#include <stdlib.h>  // for exit(), calloc()
#include <math.h>    // for sin(), cos()
#include <string.h>  // for strlen(), memcpy()

#define SDL_BGI_VERSION 2.3.0

enum { NOPE, YEAH };
#define BGI_WINTITLE_LEN 512 // more than enough

// number of concurrent windows that can be created

#define NUM_BGI_WIN 16

// everything gets drawn here

extern SDL_Window   *bgi_window;
extern SDL_Renderer *bgi_renderer;
extern SDL_Texture  *bgi_texture;

// available visual pages

#define VPAGES 4

// BGI fonts

// only DEFAULT_FONT (8x8) is implemented
enum {
  DEFAULT_FONT, TRIPLEX_FONT, SMALL_FONT, SANSSERIF_FONT,
  GOTHIC_FONT, BIG_FONT, SCRIPT_FONT, SIMPLEX_FONT,
  TRIPLEX_SCR_FONT, COMPLEX_FONT, EUROPEAN_FONT, BOLD_FONT
};

enum { HORIZ_DIR, VERT_DIR };

#define USER_CHAR_SIZE 0

enum {
  LEFT_TEXT, CENTER_TEXT, RIGHT_TEXT,
  BOTTOM_TEXT = 0, TOP_TEXT = 2
};

// BGI colours

enum {
  BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN,
  LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN,
  LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE, MAXCOLORS = 15
};

// temporary colours

enum { TMP_FG_COL = 16, TMP_BG_COL = 17, TMP_FILL_COL = 18 };

// line style, thickness, and drawing mode

enum { NORM_WIDTH = 1, THICK_WIDTH = 3 };

enum { SOLID_LINE, DOTTED_LINE, CENTER_LINE, DASHED_LINE, USERBIT_LINE };

enum { COPY_PUT, XOR_PUT, OR_PUT, AND_PUT, NOT_PUT };

// fill styles

enum {
  EMPTY_FILL, SOLID_FILL, LINE_FILL, LTSLASH_FILL, SLASH_FILL,
  BKSLASH_FILL, LTBKSLASH_FILL, HATCH_FILL, XHATCH_FILL,
  INTERLEAVE_FILL, WIDE_DOT_FILL, CLOSE_DOT_FILL, USER_FILL
};

// mouse buttons

#define WM_LBUTTONDOWN  SDL_BUTTON_LEFT
#define WM_MBUTTONDOWN  SDL_BUTTON_MIDDLE
#define WM_RBUTTONDOWN  SDL_BUTTON_RIGHT
#define WM_WHEEL        SDL_MOUSEWHEEL
#define WM_WHEELUP      SDL_USEREVENT
#define WM_WHEELDOWN    SDL_USEREVENT + 1
#define WM_MOUSEMOVE    SDL_MOUSEMOTION

#define PALETTE_SIZE    4096

#define KEY_HOME        SDLK_HOME
#define KEY_LEFT        SDLK_LEFT
#define KEY_UP          SDLK_UP
#define KEY_RIGHT       SDLK_RIGHT
#define KEY_DOWN        SDLK_DOWN
#define KEY_PGUP        SDLK_PAGEUP
#define KEY_PGDN        SDLK_PAGEDOWN
#define KEY_END         SDLK_END
#define KEY_INSERT      SDLK_INSERT
#define KEY_DELETE      SDLK_DELETE
#define KEY_F1          SDLK_F1
#define KEY_F2          SDLK_F2
#define KEY_F3          SDLK_F3
#define KEY_F4          SDLK_F4
#define KEY_F5          SDLK_F5
#define KEY_F6          SDLK_F6
#define KEY_F7          SDLK_F7
#define KEY_F8          SDLK_F8
#define KEY_F9          SDLK_F9
#define KEY_F10         SDLK_F10
#define KEY_F11         SDLK_F11
#define KEY_F12         SDLK_F12
#define KEY_CAPSLOCK    SDLK_CAPSLOCK
#define KEY_LEFT_CTRL   SDLK_LCTRL
#define KEY_RIGHT_CTRL  SDLK_RCTRL
#define KEY_LEFT_SHIFT  SDLK_LSHIFT
#define KEY_RIGHT_SHIFT SDLK_RSHIFT
#define KEY_LEFT_ALT    SDLK_LALT
#define KEY_RIGHT_ALT   SDLK_RALT
#define KEY_ALT_GR      SDLK_MODE
#define KEY_LGUI        SDLK_LGUI
#define KEY_RGUI        SDLK_RGUI
#define KEY_MENU        SDLK_MENU
#define KEY_TAB         SDLK_TAB
#define KEY_BS          SDLK_BACKSPACE
#define KEY_RET         SDLK_RETURN
#define KEY_PAUSE       SDLK_PAUSE
#define KEY_SCR_LOCK    SDLK_SCROLLOCK
#define KEY_ESC         SDLK_ESCAPE

#define QUIT            SDL_QUIT

// graphics modes. Expanded from the original GRAPHICS.H

enum {
  DETECT = -1,
  grOk = 0, SDL = 0,
  // all modes @ 320x200 
  SDL_320x200 = 1, SDL_CGALO = 1, CGA = 1, CGAC0 = 1, CGAC1 = 1,
  CGAC2 = 1, CGAC3 = 1, MCGAC0 = 1, MCGAC1 = 1, MCGAC2 = 1,
  MCGAC3 = 1, ATT400C0 = 1, ATT400C1 = 1, ATT400C2 = 1, ATT400C3 = 1,
  // all modes @ 640x200
  SDL_640x200 = 2, SDL_CGAHI = 2, CGAHI = 2, MCGAMED = 2,
  EGALO = 2, EGA64LO = 2,
  // all modes @ 640x350
  SDL_640x350 = 3, SDL_EGA = 3, EGA = 3, EGAHI = 3,
  EGA64HI = 3, EGAMONOHI = 3,
  // all modes @ 640x480
  SDL_640x480 = 4, SDL_VGA = 4, VGA = 4, MCGAHI = 4, VGAHI = 4,
  IBM8514LO = 4,
  // all modes @ 720x348
  SDL_720x348 = 5, SDL_HERC = 5,
  // all modes @ 720x350
  SDL_720x350 = 6, SDL_PC3270 = 6, HERCMONOHI = 6,
  // all modes @ 800x600
  SDL_800x600 = 7, SDL_SVGALO = 7, SVGA = 7,
  // all modes @ 1024x768
  SDL_1024x768 = 8, SDL_SVGAMED1 = 8,
  // all modes @ 1152x900
  SDL_1152x900 = 9, SDL_SVGAMED2 = 9,
  // all modes @ 1280x1024
  SDL_1280x1024 = 10, SDL_SVGAHI = 10,
  // all modes @ 1366x768
  SDL_1366x768 = 11, SDL_WXGA = 11,
  // other
  SDL_USER = 12, SDL_FULLSCREEN = 13
};

// libXbgi compatibility

#define X11_CGALO       SDL_CGALO
#define X11_CGAHI       SDL_CGAHI
#define X11_EGA         SDL_EGA
#define X11             SDL
#define X11_VGA         SDL_VGA
#define X11_640x480     SDL_640x480
#define X11_HERC        SDL_HERC
#define X11_PC3270      SDL_PC3270
#define X11_SVGALO      SDL_SVGALO
#define X11_800x600     SDL_800x600
#define X11_SVGAMED1    SDL_SVGAMED1
#define X11_1024x768    SDL_1024x768
#define X11_SVGAMED2    SDL_SVGAMED2
#define X11_1152x900    SDL_1152x900
#define X11_SVGAHI      SDL_SVGAHI
#define X11_1280x1024   SDL_1280x1024
#define X11_WXGA        SDL_WXGA
#define X11_1366x768    SDL_1366x768
#define X11_USER        SDL_USER
#define X11_FULLSCREEN  SDL_FULLSCREEN

// structs

struct arccoordstype {
  int x;
  int y;
  int xstart;
  int ystart;
  int xend;
  int yend;
};

struct date {
  int da_year;
  int da_day;
  int da_mon;
};

struct fillsettingstype {
  int pattern;
  int color;
};

struct linesettingstype {
  int linestyle;
  unsigned int upattern;
  int thickness;
};

struct palettetype {
  unsigned char size;
  signed char colors[MAXCOLORS + 1];
};

struct textsettingstype {
  int font;
  int direction;
  int charsize;
  int horiz;
  int vert;
};

struct viewporttype {
  int left;
  int top;
  int right;
  int bottom;
  int clip;
};

// prototypes - standard BGI
// make them C++ compatible

#ifdef __cplusplus
extern "C" {
#endif

void arc (int, int, int, int, int);
void bar3d (int, int, int, int, int, int);
void bar (int, int, int, int);
void circle (int, int, int);
void cleardevice ();
void clearviewport ();
void closegraph (void);
void delay (int msec);
void detectgraph (int *, int *);
void drawpoly (int, int *);
void ellipse (int, int, int, int, int, int);
void fillellipse (int, int, int, int);
void fillpoly (int, int *);
void floodfill (int, int, int);
int  getactivepage (void);
void getarccoords (struct arccoordstype *);
void getaspectratio (int *, int *);
int  getbkcolor (void);
int  bgi_getch (void);
// circumvents Mingw bug
#define getch bgi_getch
int  getcolor (void);
struct palettetype *getdefaultpalette (void);
char *getdrivername (void);
void getfillpattern (char *);
void getfillsettings (struct fillsettingstype *);
int  getgraphmode (void);
void getimage (int, int, int, int, void *);
void getlinesettings (struct linesettingstype *);
int  getmaxcolor (void);
int  getmaxmode (void);
int  getmaxx (void);
int  getmaxy (void);
char *getmodename (int);
void getmoderange (int, int *, int *);
void getpalette (struct palettetype *);
int  getpalettesize (struct palettetype *);
unsigned int getpixel (int, int);
void gettextsettings (struct textsettingstype *);
void getviewsettings (struct viewporttype *);
int  getvisualpage (void);
int  getx (void);
int  gety (void);
void graphdefaults ();
char *grapherrormsg (int);
int  graphresult (void);
unsigned imagesize (int, int, int, int);
void initgraph (int *, int *, char *);
int  installuserdriver (char *name, int (*detect)(void));
int  installuserfont (char *);
int  kbhit (void);
void line (int, int, int, int);
void linerel (int dx, int dy);
void lineto (int x, int y);
void moverel (int, int);
void moveto (int, int);
void outtext (char *);
void outtextxy (int, int, char *);
void pieslice (int, int, int, int, int);
void putimage (int, int, void *, int);
void putpixel (int, int, int);
#define random(range) (rand() % (range))
void readimagefile (char *, int, int, int, int);
void rectangle (int, int, int, int);
int  registerbgidriver (void (*driver)(void));
int  registerbgifont (void (*font)(void));
void restorecrtmode (void);
void sector (int, int, int, int, int, int);
void setactivepage (int);
void setallpalette (struct palettetype *);
void setaspectratio (int, int);
void setbkcolor (int);
void setcolor (int);
void setfillpattern (char *, int);
void setfillstyle (int, int);
unsigned setgraphbufsize (unsigned);
void setgraphmode (int);
void setlinestyle (int, unsigned, int);
void setpalette (int, int);
void settextjustify (int, int);
void settextstyle (int, int, int);
void setusercharsize (int, int, int, int);
void setviewport (int, int, int, int, int);
void setvisualpage (int);
void setwritemode (int);
int  textheight (char *);
int  textwidth (char *);
void writeimagefile (char *, int, int, int, int);

// SDL_bgi extensions

int  ALPHA_VALUE (int);
int  BLUE_VALUE (int);
void closewindow (int);
int  COLOR (int, int, int);
int  event (void);
int eventtype (void);
void freeimage (void *);
int  getcurrentwindow (void);
int  getevent (void);
void getmouseclick (int, int *, int *);
int  GREEN_VALUE (int);
void initwindow (int, int);
int  IS_BGI_COLOR (int color);
int  ismouseclick (int);
int  IS_RGB_COLOR (int color);
int  mouseclick (void);
int  mousex (void);
int  mousey (void);
void _putpixel (int, int);
int  RED_VALUE (int );
void refresh (void);
void sdlbgiauto (void);
void sdlbgifast (void);
void sdlbgislow (void);
void setalpha (int, Uint8);
void setbkrgbcolor (int);
void setblendmode (int);
void setcurrentwindow (int);
void setrgbcolor (int);
void setrgbpalette (int, int, int, int);
void setwinoptions (char *, int, int, Uint32);
void showerrorbox (const char *);
void swapbuffers (void);
int  xkbhit (void);

#ifdef __cplusplus
}
#endif

// #ifndef _SDL_BGI_H

#endif // _SDL_BGI_H

// --- End of file SDL_bgi.h
