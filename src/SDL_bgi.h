// SDL_bgi.h	-*- C -*-

// A BGI (Borland Graphics Library) implementation based on SDL2.
// Easy to use, pretty fast, and useful for porting old programs.
// Guido Gonzato, PhD
// September 10, 2018

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


// SDL2 stuff
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>

// for fprintf()
#include <stdio.h>
// for exit(), calloc()
#include <stdlib.h>
// for sin(), cos()
#include <math.h>
// for strlen(), memcpy()
#include <string.h>

#ifndef _SDL_BGI_H
#define _SDL_BGI_H

#define SDL_BGI_VERSION 2.2.2

#define NOPE 0
#define YEAH 1
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

#define DEFAULT_FONT     0 // 8x8
#define TRIPLEX_FONT     1 // all other fonts are not implemented
#define SMALL_FONT       2
#define SANSSERIF_FONT   3
#define GOTHIC_FONT      4
#define BIG_FONT         5
#define SCRIPT_FONT      6
#define SIMPLEX_FONT     7
#define TRIPLEX_SCR_FONT 8
#define COMPLEX_FONT     9
#define EUROPEAN_FONT    10
#define BOLD_FONT        11

#define HORIZ_DIR      0
#define VERT_DIR       1

#define USER_CHAR_SIZE 0

#define LEFT_TEXT      0
#define CENTER_TEXT    1
#define RIGHT_TEXT     2
#define BOTTOM_TEXT    0
#define TOP_TEXT       2

// BGI colours

#define MAXCOLORS      15
#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define LIGHTGRAY       7
#define DARKGRAY        8
#define LIGHTBLUE       9
#define LIGHTGREEN     10
#define LIGHTCYAN      11
#define LIGHTRED       12
#define LIGHTMAGENTA   13
#define YELLOW         14
#define WHITE          15

// temporary colours

#define TMP_FG_COL     16
#define TMP_BG_COL     17
#define TMP_FILL_COL   18

// line style, thickness, and drawing mode

#define NORM_WIDTH      1
#define THICK_WIDTH     3

#define SOLID_LINE      0
#define DOTTED_LINE     1
#define CENTER_LINE     2
#define DASHED_LINE     3
#define USERBIT_LINE    4

#define COPY_PUT        0
#define XOR_PUT         1
#define OR_PUT          2
#define AND_PUT         3
#define NOT_PUT         4

// fill styles

#define EMPTY_FILL      0
#define SOLID_FILL      1
#define LINE_FILL       2
#define LTSLASH_FILL    3
#define SLASH_FILL      4
#define BKSLASH_FILL    5
#define LTBKSLASH_FILL  6
#define HATCH_FILL      7
#define XHATCH_FILL     8
#define INTERLEAVE_FILL 9
#define WIDE_DOT_FILL   10
#define CLOSE_DOT_FILL  11
#define USER_FILL       12

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
#define KEY_LEFT_CTRL   SDLK_LCTRL
#define KEY_RIGHT_CTRL  SDLK_RCTRL
#define KEY_LEFT_SHIFT  SDLK_LSHIFT
#define KEY_RIGHT_SHIFT SDLK_RSHIFT
#define KEY_LEFT_ALT    SDLK_LALT
#define KEY_ALT_GR      SDLK_MODE
#define KEY_TAB         SDLK_TAB
#define KEY_BS          SDLK_BACKSPACE
#define KEY_RET         SDLK_RETURN
#define KEY_PAUSE       SDLK_PAUSE
#define KEY_SCR_LOCK    SDLK_SCROLLOCK
#define KEY_ESC         SDLK_ESCAPE

#define QUIT            SDL_QUIT

// graphics modes

#define DETECT          -1
#define grOk            0
#define SDL             0
#define SDL_320x200     1
#define SDL_CGALO       1
#define CGA             1
#define SDL_640x200     2
#define SDL_CGAHI       2
#define SDL_640x350     3
#define SDL_EGA         3
#define EGA             3
#define EGALO           3
#define SDL_640x480     4
#define SDL_VGA         4
#define VGA             4
#define SDL_HERC        5
#define SDL_PC3270      6
#define SDL_800x600     7
#define SDL_SVGALO      7
#define SVGA            7
#define SDL_1024x768    8
#define SDL_SVGAMED1    8
#define SDL_1152x900    9
#define SDL_SVGAMED2    9
#define SDL_1280x1024   10
#define SDL_SVGAHI      10
#define SDL_1366x768    11
#define SDL_WXGA        11
#define SDL_USER        12
#define SDL_FULLSCREEN  13

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
void sdlbgifast (void);
void sdlbgislow (void);
void setalpha (int, Uint8);
void setbkrgbcolor (int);
void setcurrentwindow (int);
void setrgbcolor (int);
void setrgbpalette (int, int, int, int);
void setwinoptions (char *, int, int, Uint32);
void swapbuffers (void);
int  xkbhit (void);

/* ---- 8x8 font definition ---- */

/*  ZLIB (c) A. Schiffler 2012 */

#define GFX_FONTDATAMAX (8*256)

static unsigned char gfxPrimitivesFontdata[GFX_FONTDATAMAX] = {

	/*
	* 0 0x00 '^@'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 1 0x01 '^A'
	*/
	0x7e,			/* 01111110 */
	0x81,			/* 10000001 */
	0xa5,			/* 10100101 */
	0x81,			/* 10000001 */
	0xbd,			/* 10111101 */
	0x99,			/* 10011001 */
	0x81,			/* 10000001 */
	0x7e,			/* 01111110 */

	/*
	* 2 0x02 '^B'
	*/
	0x7e,			/* 01111110 */
	0xff,			/* 11111111 */
	0xdb,			/* 11011011 */
	0xff,			/* 11111111 */
	0xc3,			/* 11000011 */
	0xe7,			/* 11100111 */
	0xff,			/* 11111111 */
	0x7e,			/* 01111110 */

	/*
	* 3 0x03 '^C'
	*/
	0x6c,			/* 01101100 */
	0xfe,			/* 11111110 */
	0xfe,			/* 11111110 */
	0xfe,			/* 11111110 */
	0x7c,			/* 01111100 */
	0x38,			/* 00111000 */
	0x10,			/* 00010000 */
	0x00,			/* 00000000 */

	/*
	* 4 0x04 '^D'
	*/
	0x10,			/* 00010000 */
	0x38,			/* 00111000 */
	0x7c,			/* 01111100 */
	0xfe,			/* 11111110 */
	0x7c,			/* 01111100 */
	0x38,			/* 00111000 */
	0x10,			/* 00010000 */
	0x00,			/* 00000000 */

	/*
	* 5 0x05 '^E'
	*/
	0x38,			/* 00111000 */
	0x7c,			/* 01111100 */
	0x38,			/* 00111000 */
	0xfe,			/* 11111110 */
	0xfe,			/* 11111110 */
	0xd6,			/* 11010110 */
	0x10,			/* 00010000 */
	0x38,			/* 00111000 */

	/*
	* 6 0x06 '^F'
	*/
	0x10,			/* 00010000 */
	0x38,			/* 00111000 */
	0x7c,			/* 01111100 */
	0xfe,			/* 11111110 */
	0xfe,			/* 11111110 */
	0x7c,			/* 01111100 */
	0x10,			/* 00010000 */
	0x38,			/* 00111000 */

	/*
	* 7 0x07 '^G'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x3c,			/* 00111100 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 8 0x08 '^H'
	*/
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0xe7,			/* 11100111 */
	0xc3,			/* 11000011 */
	0xc3,			/* 11000011 */
	0xe7,			/* 11100111 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */

	/*
	* 9 0x09 '^I'
	*/
	0x00,			/* 00000000 */
	0x3c,			/* 00111100 */
	0x66,			/* 01100110 */
	0x42,			/* 01000010 */
	0x42,			/* 01000010 */
	0x66,			/* 01100110 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 10 0x0a '^J'
	*/
	0xff,			/* 11111111 */
	0xc3,			/* 11000011 */
	0x99,			/* 10011001 */
	0xbd,			/* 10111101 */
	0xbd,			/* 10111101 */
	0x99,			/* 10011001 */
	0xc3,			/* 11000011 */
	0xff,			/* 11111111 */

	/*
	* 11 0x0b '^K'
	*/
	0x0f,			/* 00001111 */
	0x07,			/* 00000111 */
	0x0f,			/* 00001111 */
	0x7d,			/* 01111101 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0x78,			/* 01111000 */

	/*
	* 12 0x0c '^L'
	*/
	0x3c,			/* 00111100 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x3c,			/* 00111100 */
	0x18,			/* 00011000 */
	0x7e,			/* 01111110 */
	0x18,			/* 00011000 */

	/*
	* 13 0x0d '^M'
	*/
	0x3f,			/* 00111111 */
	0x33,			/* 00110011 */
	0x3f,			/* 00111111 */
	0x30,			/* 00110000 */
	0x30,			/* 00110000 */
	0x70,			/* 01110000 */
	0xf0,			/* 11110000 */
	0xe0,			/* 11100000 */

	/*
	* 14 0x0e '^N'
	*/
	0x7f,			/* 01111111 */
	0x63,			/* 01100011 */
	0x7f,			/* 01111111 */
	0x63,			/* 01100011 */
	0x63,			/* 01100011 */
	0x67,			/* 01100111 */
	0xe6,			/* 11100110 */
	0xc0,			/* 11000000 */

	/*
	* 15 0x0f '^O'
	*/
	0x18,			/* 00011000 */
	0xdb,			/* 11011011 */
	0x3c,			/* 00111100 */
	0xe7,			/* 11100111 */
	0xe7,			/* 11100111 */
	0x3c,			/* 00111100 */
	0xdb,			/* 11011011 */
	0x18,			/* 00011000 */

	/*
	* 16 0x10 '^P'
	*/
	0x80,			/* 10000000 */
	0xe0,			/* 11100000 */
	0xf8,			/* 11111000 */
	0xfe,			/* 11111110 */
	0xf8,			/* 11111000 */
	0xe0,			/* 11100000 */
	0x80,			/* 10000000 */
	0x00,			/* 00000000 */

	/*
	* 17 0x11 '^Q'
	*/
	0x02,			/* 00000010 */
	0x0e,			/* 00001110 */
	0x3e,			/* 00111110 */
	0xfe,			/* 11111110 */
	0x3e,			/* 00111110 */
	0x0e,			/* 00001110 */
	0x02,			/* 00000010 */
	0x00,			/* 00000000 */

	/*
	* 18 0x12 '^R'
	*/
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x7e,			/* 01111110 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x7e,			/* 01111110 */
	0x3c,			/* 00111100 */
	0x18,			/* 00011000 */

	/*
	* 19 0x13 '^S'
	*/
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x00,			/* 00000000 */
	0x66,			/* 01100110 */
	0x00,			/* 00000000 */

	/*
	* 20 0x14 '^T'
	*/
	0x7f,			/* 01111111 */
	0xdb,			/* 11011011 */
	0xdb,			/* 11011011 */
	0x7b,			/* 01111011 */
	0x1b,			/* 00011011 */
	0x1b,			/* 00011011 */
	0x1b,			/* 00011011 */
	0x00,			/* 00000000 */

	/*
	* 21 0x15 '^U'
	*/
	0x3e,			/* 00111110 */
	0x61,			/* 01100001 */
	0x3c,			/* 00111100 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x3c,			/* 00111100 */
	0x86,			/* 10000110 */
	0x7c,			/* 01111100 */

	/*
	* 22 0x16 '^V'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0x7e,			/* 01111110 */
	0x7e,			/* 01111110 */
	0x00,			/* 00000000 */

	/*
	* 23 0x17 '^W'
	*/
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x7e,			/* 01111110 */
	0x18,			/* 00011000 */
	0x7e,			/* 01111110 */
	0x3c,			/* 00111100 */
	0x18,			/* 00011000 */
	0xff,			/* 11111111 */

	/*
	* 24 0x18 '^X'
	*/
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x7e,			/* 01111110 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */

	/*
	* 25 0x19 '^Y'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x7e,			/* 01111110 */
	0x3c,			/* 00111100 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */

	/*
	* 26 0x1a '^Z'
	*/
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x0c,			/* 00001100 */
	0xfe,			/* 11111110 */
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 27 0x1b '^['
	*/
	0x00,			/* 00000000 */
	0x30,			/* 00110000 */
	0x60,			/* 01100000 */
	0xfe,			/* 11111110 */
	0x60,			/* 01100000 */
	0x30,			/* 00110000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 28 0x1c '^\'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0xfe,			/* 11111110 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 29 0x1d '^]'
	*/
	0x00,			/* 00000000 */
	0x24,			/* 00100100 */
	0x66,			/* 01100110 */
	0xff,			/* 11111111 */
	0x66,			/* 01100110 */
	0x24,			/* 00100100 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 30 0x1e '^^'
	*/
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x7e,			/* 01111110 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 31 0x1f '^_'
	*/
	0x00,			/* 00000000 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0x7e,			/* 01111110 */
	0x3c,			/* 00111100 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 32 0x20 ' '
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 33 0x21 '!'
	*/
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x3c,			/* 00111100 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */

	/*
	* 34 0x22 '"'
	*/
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x24,			/* 00100100 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 35 0x23 '#'
	*/
	0x6c,			/* 01101100 */
	0x6c,			/* 01101100 */
	0xfe,			/* 11111110 */
	0x6c,			/* 01101100 */
	0xfe,			/* 11111110 */
	0x6c,			/* 01101100 */
	0x6c,			/* 01101100 */
	0x00,			/* 00000000 */

	/*
	* 36 0x24 '$'
	*/
	0x18,			/* 00011000 */
	0x3e,			/* 00111110 */
	0x60,			/* 01100000 */
	0x3c,			/* 00111100 */
	0x06,			/* 00000110 */
	0x7c,			/* 01111100 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */

	/*
	* 37 0x25 '%'
	*/
	0x00,			/* 00000000 */
	0xc6,			/* 11000110 */
	0xcc,			/* 11001100 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0x66,			/* 01100110 */
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */

	/*
	* 38 0x26 '&'
	*/
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0x38,			/* 00111000 */
	0x76,			/* 01110110 */
	0xdc,			/* 11011100 */
	0xcc,			/* 11001100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 39 0x27 '''
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 40 0x28 '('
	*/
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0x30,			/* 00110000 */
	0x30,			/* 00110000 */
	0x18,			/* 00011000 */
	0x0c,			/* 00001100 */
	0x00,			/* 00000000 */

	/*
	* 41 0x29 ')'
	*/
	0x30,			/* 00110000 */
	0x18,			/* 00011000 */
	0x0c,			/* 00001100 */
	0x0c,			/* 00001100 */
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0x00,			/* 00000000 */

	/*
	* 42 0x2a '*'
	*/
	0x00,			/* 00000000 */
	0x66,			/* 01100110 */
	0x3c,			/* 00111100 */
	0xff,			/* 11111111 */
	0x3c,			/* 00111100 */
	0x66,			/* 01100110 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 43 0x2b '+'
	*/
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x7e,			/* 01111110 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 44 0x2c ','
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */

	/*
	* 45 0x2d '-'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 46 0x2e '.'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */

	/*
	* 47 0x2f '/'
	*/
	0x06,			/* 00000110 */
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0x60,			/* 01100000 */
	0xc0,			/* 11000000 */
	0x80,			/* 10000000 */
	0x00,			/* 00000000 */

	/*
	* 48 0x30 '0'
	*/
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0xc6,			/* 11000110 */
	0xd6,			/* 11010110 */
	0xc6,			/* 11000110 */
	0x6c,			/* 01101100 */
	0x38,			/* 00111000 */
	0x00,			/* 00000000 */

	/*
	* 49 0x31 '1'
	*/
	0x18,			/* 00011000 */
	0x38,			/* 00111000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x7e,			/* 01111110 */
	0x00,			/* 00000000 */

	/*
	* 50 0x32 '2'
	*/
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0x06,			/* 00000110 */
	0x1c,			/* 00011100 */
	0x30,			/* 00110000 */
	0x66,			/* 01100110 */
	0xfe,			/* 11111110 */
	0x00,			/* 00000000 */

	/*
	* 51 0x33 '3'
	*/
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0x06,			/* 00000110 */
	0x3c,			/* 00111100 */
	0x06,			/* 00000110 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 52 0x34 '4'
	*/
	0x1c,			/* 00011100 */
	0x3c,			/* 00111100 */
	0x6c,			/* 01101100 */
	0xcc,			/* 11001100 */
	0xfe,			/* 11111110 */
	0x0c,			/* 00001100 */
	0x1e,			/* 00011110 */
	0x00,			/* 00000000 */

	/*
	* 53 0x35 '5'
	*/
	0xfe,			/* 11111110 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0xfc,			/* 11111100 */
	0x06,			/* 00000110 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 54 0x36 '6'
	*/
	0x38,			/* 00111000 */
	0x60,			/* 01100000 */
	0xc0,			/* 11000000 */
	0xfc,			/* 11111100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 55 0x37 '7'
	*/
	0xfe,			/* 11111110 */
	0xc6,			/* 11000110 */
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0x30,			/* 00110000 */
	0x30,			/* 00110000 */
	0x00,			/* 00000000 */

	/*
	* 56 0x38 '8'
	*/
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 57 0x39 '9'
	*/
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7e,			/* 01111110 */
	0x06,			/* 00000110 */
	0x0c,			/* 00001100 */
	0x78,			/* 01111000 */
	0x00,			/* 00000000 */

	/*
	* 58 0x3a ':'
	*/
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */

	/*
	* 59 0x3b ';'
	*/
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */

	/*
	* 60 0x3c '<'
	*/
	0x06,			/* 00000110 */
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0x18,			/* 00011000 */
	0x0c,			/* 00001100 */
	0x06,			/* 00000110 */
	0x00,			/* 00000000 */

	/*
	* 61 0x3d '='
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 62 0x3e '>'
	*/
	0x60,			/* 01100000 */
	0x30,			/* 00110000 */
	0x18,			/* 00011000 */
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0x60,			/* 01100000 */
	0x00,			/* 00000000 */

	/*
	* 63 0x3f '?'
	*/
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */

	/*
	* 64 0x40 '@'
	*/
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xde,			/* 11011110 */
	0xde,			/* 11011110 */
	0xde,			/* 11011110 */
	0xc0,			/* 11000000 */
	0x78,			/* 01111000 */
	0x00,			/* 00000000 */

	/*
	* 65 0x41 'A'
	*/
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0xc6,			/* 11000110 */
	0xfe,			/* 11111110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */

	/*
	* 66 0x42 'B'
	*/
	0xfc,			/* 11111100 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x7c,			/* 01111100 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0xfc,			/* 11111100 */
	0x00,			/* 00000000 */

	/*
	* 67 0x43 'C'
	*/
	0x3c,			/* 00111100 */
	0x66,			/* 01100110 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0x66,			/* 01100110 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 68 0x44 'D'
	*/
	0xf8,			/* 11111000 */
	0x6c,			/* 01101100 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x6c,			/* 01101100 */
	0xf8,			/* 11111000 */
	0x00,			/* 00000000 */

	/*
	* 69 0x45 'E'
	*/
	0xfe,			/* 11111110 */
	0x62,			/* 01100010 */
	0x68,			/* 01101000 */
	0x78,			/* 01111000 */
	0x68,			/* 01101000 */
	0x62,			/* 01100010 */
	0xfe,			/* 11111110 */
	0x00,			/* 00000000 */

	/*
	* 70 0x46 'F'
	*/
	0xfe,			/* 11111110 */
	0x62,			/* 01100010 */
	0x68,			/* 01101000 */
	0x78,			/* 01111000 */
	0x68,			/* 01101000 */
	0x60,			/* 01100000 */
	0xf0,			/* 11110000 */
	0x00,			/* 00000000 */

	/*
	* 71 0x47 'G'
	*/
	0x3c,			/* 00111100 */
	0x66,			/* 01100110 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0xce,			/* 11001110 */
	0x66,			/* 01100110 */
	0x3a,			/* 00111010 */
	0x00,			/* 00000000 */

	/*
	* 72 0x48 'H'
	*/
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xfe,			/* 11111110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */

	/*
	* 73 0x49 'I'
	*/
	0x3c,			/* 00111100 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 74 0x4a 'J'
	*/
	0x1e,			/* 00011110 */
	0x0c,			/* 00001100 */
	0x0c,			/* 00001100 */
	0x0c,			/* 00001100 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0x78,			/* 01111000 */
	0x00,			/* 00000000 */

	/*
	* 75 0x4b 'K'
	*/
	0xe6,			/* 11100110 */
	0x66,			/* 01100110 */
	0x6c,			/* 01101100 */
	0x78,			/* 01111000 */
	0x6c,			/* 01101100 */
	0x66,			/* 01100110 */
	0xe6,			/* 11100110 */
	0x00,			/* 00000000 */

	/*
	* 76 0x4c 'L'
	*/
	0xf0,			/* 11110000 */
	0x60,			/* 01100000 */
	0x60,			/* 01100000 */
	0x60,			/* 01100000 */
	0x62,			/* 01100010 */
	0x66,			/* 01100110 */
	0xfe,			/* 11111110 */
	0x00,			/* 00000000 */

	/*
	* 77 0x4d 'M'
	*/
	0xc6,			/* 11000110 */
	0xee,			/* 11101110 */
	0xfe,			/* 11111110 */
	0xfe,			/* 11111110 */
	0xd6,			/* 11010110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */

	/*
	* 78 0x4e 'N'
	*/
	0xc6,			/* 11000110 */
	0xe6,			/* 11100110 */
	0xf6,			/* 11110110 */
	0xde,			/* 11011110 */
	0xce,			/* 11001110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */

	/*
	* 79 0x4f 'O'
	*/
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 80 0x50 'P'
	*/
	0xfc,			/* 11111100 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x7c,			/* 01111100 */
	0x60,			/* 01100000 */
	0x60,			/* 01100000 */
	0xf0,			/* 11110000 */
	0x00,			/* 00000000 */

	/*
	* 81 0x51 'Q'
	*/
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xce,			/* 11001110 */
	0x7c,			/* 01111100 */
	0x0e,			/* 00001110 */

	/*
	* 82 0x52 'R'
	*/
	0xfc,			/* 11111100 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x7c,			/* 01111100 */
	0x6c,			/* 01101100 */
	0x66,			/* 01100110 */
	0xe6,			/* 11100110 */
	0x00,			/* 00000000 */

	/*
	* 83 0x53 'S'
	*/
	0x3c,			/* 00111100 */
	0x66,			/* 01100110 */
	0x30,			/* 00110000 */
	0x18,			/* 00011000 */
	0x0c,			/* 00001100 */
	0x66,			/* 01100110 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 84 0x54 'T'
	*/
	0x7e,			/* 01111110 */
	0x7e,			/* 01111110 */
	0x5a,			/* 01011010 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 85 0x55 'U'
	*/
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 86 0x56 'V'
	*/
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x6c,			/* 01101100 */
	0x38,			/* 00111000 */
	0x00,			/* 00000000 */

	/*
	* 87 0x57 'W'
	*/
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xd6,			/* 11010110 */
	0xd6,			/* 11010110 */
	0xfe,			/* 11111110 */
	0x6c,			/* 01101100 */
	0x00,			/* 00000000 */

	/*
	* 88 0x58 'X'
	*/
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x6c,			/* 01101100 */
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */

	/*
	* 89 0x59 'Y'
	*/
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x3c,			/* 00111100 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 90 0x5a 'Z'
	*/
	0xfe,			/* 11111110 */
	0xc6,			/* 11000110 */
	0x8c,			/* 10001100 */
	0x18,			/* 00011000 */
	0x32,			/* 00110010 */
	0x66,			/* 01100110 */
	0xfe,			/* 11111110 */
	0x00,			/* 00000000 */

	/*
	* 91 0x5b '['
	*/
	0x3c,			/* 00111100 */
	0x30,			/* 00110000 */
	0x30,			/* 00110000 */
	0x30,			/* 00110000 */
	0x30,			/* 00110000 */
	0x30,			/* 00110000 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 92 0x5c '\'
	*/
	0xc0,			/* 11000000 */
	0x60,			/* 01100000 */
	0x30,			/* 00110000 */
	0x18,			/* 00011000 */
	0x0c,			/* 00001100 */
	0x06,			/* 00000110 */
	0x02,			/* 00000010 */
	0x00,			/* 00000000 */

	/*
	* 93 0x5d ']'
	*/
	0x3c,			/* 00111100 */
	0x0c,			/* 00001100 */
	0x0c,			/* 00001100 */
	0x0c,			/* 00001100 */
	0x0c,			/* 00001100 */
	0x0c,			/* 00001100 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 94 0x5e '^'
	*/
	0x10,			/* 00010000 */
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 95 0x5f '_'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xff,			/* 11111111 */

	/*
	* 96 0x60 '`'
	*/
	0x30,			/* 00110000 */
	0x18,			/* 00011000 */
	0x0c,			/* 00001100 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 97 0x61 'a'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x78,			/* 01111000 */
	0x0c,			/* 00001100 */
	0x7c,			/* 01111100 */
	0xcc,			/* 11001100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 98 0x62 'b'
	*/
	0xe0,			/* 11100000 */
	0x60,			/* 01100000 */
	0x7c,			/* 01111100 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0xdc,			/* 11011100 */
	0x00,			/* 00000000 */

	/*
	* 99 0x63 'c'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xc0,			/* 11000000 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 100 0x64 'd'
	*/
	0x1c,			/* 00011100 */
	0x0c,			/* 00001100 */
	0x7c,			/* 01111100 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 101 0x65 'e'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xfe,			/* 11111110 */
	0xc0,			/* 11000000 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 102 0x66 'f'
	*/
	0x3c,			/* 00111100 */
	0x66,			/* 01100110 */
	0x60,			/* 01100000 */
	0xf8,			/* 11111000 */
	0x60,			/* 01100000 */
	0x60,			/* 01100000 */
	0xf0,			/* 11110000 */
	0x00,			/* 00000000 */

	/*
	* 103 0x67 'g'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x76,			/* 01110110 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0x7c,			/* 01111100 */
	0x0c,			/* 00001100 */
	0xf8,			/* 11111000 */

	/*
	* 104 0x68 'h'
	*/
	0xe0,			/* 11100000 */
	0x60,			/* 01100000 */
	0x6c,			/* 01101100 */
	0x76,			/* 01110110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0xe6,			/* 11100110 */
	0x00,			/* 00000000 */

	/*
	* 105 0x69 'i'
	*/
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x38,			/* 00111000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 106 0x6a 'j'
	*/
	0x06,			/* 00000110 */
	0x00,			/* 00000000 */
	0x06,			/* 00000110 */
	0x06,			/* 00000110 */
	0x06,			/* 00000110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x3c,			/* 00111100 */

	/*
	* 107 0x6b 'k'
	*/
	0xe0,			/* 11100000 */
	0x60,			/* 01100000 */
	0x66,			/* 01100110 */
	0x6c,			/* 01101100 */
	0x78,			/* 01111000 */
	0x6c,			/* 01101100 */
	0xe6,			/* 11100110 */
	0x00,			/* 00000000 */

	/*
	* 108 0x6c 'l'
	*/
	0x38,			/* 00111000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 109 0x6d 'm'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xec,			/* 11101100 */
	0xfe,			/* 11111110 */
	0xd6,			/* 11010110 */
	0xd6,			/* 11010110 */
	0xd6,			/* 11010110 */
	0x00,			/* 00000000 */

	/*
	* 110 0x6e 'n'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xdc,			/* 11011100 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x00,			/* 00000000 */

	/*
	* 111 0x6f 'o'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 112 0x70 'p'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xdc,			/* 11011100 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x7c,			/* 01111100 */
	0x60,			/* 01100000 */
	0xf0,			/* 11110000 */

	/*
	* 113 0x71 'q'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x76,			/* 01110110 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0x7c,			/* 01111100 */
	0x0c,			/* 00001100 */
	0x1e,			/* 00011110 */

	/*
	* 114 0x72 'r'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xdc,			/* 11011100 */
	0x76,			/* 01110110 */
	0x60,			/* 01100000 */
	0x60,			/* 01100000 */
	0xf0,			/* 11110000 */
	0x00,			/* 00000000 */

	/*
	* 115 0x73 's'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0xc0,			/* 11000000 */
	0x7c,			/* 01111100 */
	0x06,			/* 00000110 */
	0xfc,			/* 11111100 */
	0x00,			/* 00000000 */

	/*
	* 116 0x74 't'
	*/
	0x30,			/* 00110000 */
	0x30,			/* 00110000 */
	0xfc,			/* 11111100 */
	0x30,			/* 00110000 */
	0x30,			/* 00110000 */
	0x36,			/* 00110110 */
	0x1c,			/* 00011100 */
	0x00,			/* 00000000 */

	/*
	* 117 0x75 'u'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 118 0x76 'v'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x6c,			/* 01101100 */
	0x38,			/* 00111000 */
	0x00,			/* 00000000 */

	/*
	* 119 0x77 'w'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xc6,			/* 11000110 */
	0xd6,			/* 11010110 */
	0xd6,			/* 11010110 */
	0xfe,			/* 11111110 */
	0x6c,			/* 01101100 */
	0x00,			/* 00000000 */

	/*
	* 120 0x78 'x'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xc6,			/* 11000110 */
	0x6c,			/* 01101100 */
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */

	/*
	* 121 0x79 'y'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7e,			/* 01111110 */
	0x06,			/* 00000110 */
	0xfc,			/* 11111100 */

	/*
	* 122 0x7a 'z'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0x4c,			/* 01001100 */
	0x18,			/* 00011000 */
	0x32,			/* 00110010 */
	0x7e,			/* 01111110 */
	0x00,			/* 00000000 */

	/*
	* 123 0x7b '{'
	*/
	0x0e,			/* 00001110 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x70,			/* 01110000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x0e,			/* 00001110 */
	0x00,			/* 00000000 */

	/*
	* 124 0x7c '|'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */

	/*
	* 125 0x7d '}'
	*/
	0x70,			/* 01110000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x0e,			/* 00001110 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x70,			/* 01110000 */
	0x00,			/* 00000000 */

	/*
	* 126 0x7e '~'
	*/
	0x76,			/* 01110110 */
	0xdc,			/* 11011100 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 127 0x7f ''
	*/
	0x00,			/* 00000000 */
	0x10,			/* 00010000 */
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xfe,			/* 11111110 */
	0x00,			/* 00000000 */

	/*
	* 128 0x80 ''
	*/
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x0c,			/* 00001100 */
	0x78,			/* 01111000 */

	/*
	* 129 0x81 ''
	*/
	0xcc,			/* 11001100 */
	0x00,			/* 00000000 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 130 0x82 ''
	*/
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xfe,			/* 11111110 */
	0xc0,			/* 11000000 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 131 0x83 ''
	*/
	0x7c,			/* 01111100 */
	0x82,			/* 10000010 */
	0x78,			/* 01111000 */
	0x0c,			/* 00001100 */
	0x7c,			/* 01111100 */
	0xcc,			/* 11001100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 132 0x84 ''
	*/
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */
	0x78,			/* 01111000 */
	0x0c,			/* 00001100 */
	0x7c,			/* 01111100 */
	0xcc,			/* 11001100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 133 0x85 ''
	*/
	0x30,			/* 00110000 */
	0x18,			/* 00011000 */
	0x78,			/* 01111000 */
	0x0c,			/* 00001100 */
	0x7c,			/* 01111100 */
	0xcc,			/* 11001100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 134 0x86 ''
	*/
	0x30,			/* 00110000 */
	0x30,			/* 00110000 */
	0x78,			/* 01111000 */
	0x0c,			/* 00001100 */
	0x7c,			/* 01111100 */
	0xcc,			/* 11001100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 135 0x87 ''
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0x7e,			/* 01111110 */
	0x0c,			/* 00001100 */
	0x38,			/* 00111000 */

	/*
	* 136 0x88 ''
	*/
	0x7c,			/* 01111100 */
	0x82,			/* 10000010 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xfe,			/* 11111110 */
	0xc0,			/* 11000000 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 137 0x89 ''
	*/
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xfe,			/* 11111110 */
	0xc0,			/* 11000000 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 138 0x8a ''
	*/
	0x30,			/* 00110000 */
	0x18,			/* 00011000 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xfe,			/* 11111110 */
	0xc0,			/* 11000000 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 139 0x8b ''
	*/
	0x66,			/* 01100110 */
	0x00,			/* 00000000 */
	0x38,			/* 00111000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 140 0x8c ''
	*/
	0x7c,			/* 01111100 */
	0x82,			/* 10000010 */
	0x38,			/* 00111000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 141 0x8d ''
	*/
	0x30,			/* 00110000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x38,			/* 00111000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 142 0x8e ''
	*/
	0xc6,			/* 11000110 */
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0xc6,			/* 11000110 */
	0xfe,			/* 11111110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */

	/*
	* 143 0x8f ''
	*/
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xfe,			/* 11111110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */

	/*
	* 144 0x90 ''
	*/
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0xfe,			/* 11111110 */
	0xc0,			/* 11000000 */
	0xf8,			/* 11111000 */
	0xc0,			/* 11000000 */
	0xfe,			/* 11111110 */
	0x00,			/* 00000000 */

	/*
	* 145 0x91 ''
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0x18,			/* 00011000 */
	0x7e,			/* 01111110 */
	0xd8,			/* 11011000 */
	0x7e,			/* 01111110 */
	0x00,			/* 00000000 */

	/*
	* 146 0x92 ''
	*/
	0x3e,			/* 00111110 */
	0x6c,			/* 01101100 */
	0xcc,			/* 11001100 */
	0xfe,			/* 11111110 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xce,			/* 11001110 */
	0x00,			/* 00000000 */

	/*
	* 147 0x93 ''
	*/
	0x7c,			/* 01111100 */
	0x82,			/* 10000010 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 148 0x94 ''
	*/
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 149 0x95 ''
	*/
	0x30,			/* 00110000 */
	0x18,			/* 00011000 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 150 0x96 ''
	*/
	0x78,			/* 01111000 */
	0x84,			/* 10000100 */
	0x00,			/* 00000000 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 151 0x97 ''
	*/
	0x60,			/* 01100000 */
	0x30,			/* 00110000 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 152 0x98 ''
	*/
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7e,			/* 01111110 */
	0x06,			/* 00000110 */
	0xfc,			/* 11111100 */

	/*
	* 153 0x99 ''
	*/
	0xc6,			/* 11000110 */
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x6c,			/* 01101100 */
	0x38,			/* 00111000 */
	0x00,			/* 00000000 */

	/*
	* 154 0x9a ''
	*/
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 155 0x9b ''
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x7e,			/* 01111110 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0x7e,			/* 01111110 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 156 0x9c ''
	*/
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0x64,			/* 01100100 */
	0xf0,			/* 11110000 */
	0x60,			/* 01100000 */
	0x66,			/* 01100110 */
	0xfc,			/* 11111100 */
	0x00,			/* 00000000 */

	/*
	* 157 0x9d ''
	*/
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x3c,			/* 00111100 */
	0x7e,			/* 01111110 */
	0x18,			/* 00011000 */
	0x7e,			/* 01111110 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 158 0x9e ''
	*/
	0xf8,			/* 11111000 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xfa,			/* 11111010 */
	0xc6,			/* 11000110 */
	0xcf,			/* 11001111 */
	0xc6,			/* 11000110 */
	0xc7,			/* 11000111 */

	/*
	* 159 0x9f ''
	*/
	0x0e,			/* 00001110 */
	0x1b,			/* 00011011 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x18,			/* 00011000 */
	0xd8,			/* 11011000 */
	0x70,			/* 01110000 */
	0x00,			/* 00000000 */

	/*
	* 160 0xa0 ' '
	*/
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0x78,			/* 01111000 */
	0x0c,			/* 00001100 */
	0x7c,			/* 01111100 */
	0xcc,			/* 11001100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 161 0xa1 '¡'
	*/
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x38,			/* 00111000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 162 0xa2 '¢'
	*/
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */

	/*
	* 163 0xa3 '£'
	*/
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 164 0xa4 '¤'
	*/
	0x76,			/* 01110110 */
	0xdc,			/* 11011100 */
	0x00,			/* 00000000 */
	0xdc,			/* 11011100 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x00,			/* 00000000 */

	/*
	* 165 0xa5 '¥'
	*/
	0x76,			/* 01110110 */
	0xdc,			/* 11011100 */
	0x00,			/* 00000000 */
	0xe6,			/* 11100110 */
	0xf6,			/* 11110110 */
	0xde,			/* 11011110 */
	0xce,			/* 11001110 */
	0x00,			/* 00000000 */

	/*
	* 166 0xa6 '¦'
	*/
	0x3c,			/* 00111100 */
	0x6c,			/* 01101100 */
	0x6c,			/* 01101100 */
	0x3e,			/* 00111110 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 167 0xa7 '§'
	*/
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0x6c,			/* 01101100 */
	0x38,			/* 00111000 */
	0x00,			/* 00000000 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 168 0xa8 '¨'
	*/
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0x63,			/* 01100011 */
	0x3e,			/* 00111110 */
	0x00,			/* 00000000 */

	/*
	* 169 0xa9 '©'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xfe,			/* 11111110 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 170 0xaa 'ª'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xfe,			/* 11111110 */
	0x06,			/* 00000110 */
	0x06,			/* 00000110 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 171 0xab '«'
	*/
	0x63,			/* 01100011 */
	0xe6,			/* 11100110 */
	0x6c,			/* 01101100 */
	0x7e,			/* 01111110 */
	0x33,			/* 00110011 */
	0x66,			/* 01100110 */
	0xcc,			/* 11001100 */
	0x0f,			/* 00001111 */

	/*
	* 172 0xac '¬'
	*/
	0x63,			/* 01100011 */
	0xe6,			/* 11100110 */
	0x6c,			/* 01101100 */
	0x7a,			/* 01111010 */
	0x36,			/* 00110110 */
	0x6a,			/* 01101010 */
	0xdf,			/* 11011111 */
	0x06,			/* 00000110 */

	/*
	* 173 0xad '­'
	*/
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x3c,			/* 00111100 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */

	/*
	* 174 0xae '®'
	*/
	0x00,			/* 00000000 */
	0x33,			/* 00110011 */
	0x66,			/* 01100110 */
	0xcc,			/* 11001100 */
	0x66,			/* 01100110 */
	0x33,			/* 00110011 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 175 0xaf '¯'
	*/
	0x00,			/* 00000000 */
	0xcc,			/* 11001100 */
	0x66,			/* 01100110 */
	0x33,			/* 00110011 */
	0x66,			/* 01100110 */
	0xcc,			/* 11001100 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 176 0xb0 '°'
	*/
	0x22,			/* 00100010 */
	0x88,			/* 10001000 */
	0x22,			/* 00100010 */
	0x88,			/* 10001000 */
	0x22,			/* 00100010 */
	0x88,			/* 10001000 */
	0x22,			/* 00100010 */
	0x88,			/* 10001000 */

	/*
	* 177 0xb1 '±'
	*/
	0x55,			/* 01010101 */
	0xaa,			/* 10101010 */
	0x55,			/* 01010101 */
	0xaa,			/* 10101010 */
	0x55,			/* 01010101 */
	0xaa,			/* 10101010 */
	0x55,			/* 01010101 */
	0xaa,			/* 10101010 */

	/*
	* 178 0xb2 '²'
	*/
	0x77,			/* 01110111 */
	0xdd,			/* 11011101 */
	0x77,			/* 01110111 */
	0xdd,			/* 11011101 */
	0x77,			/* 01110111 */
	0xdd,			/* 11011101 */
	0x77,			/* 01110111 */
	0xdd,			/* 11011101 */

	/*
	* 179 0xb3 '³'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 180 0xb4 '´'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0xf8,			/* 11111000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 181 0xb5 'µ'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0xf8,			/* 11111000 */
	0x18,			/* 00011000 */
	0xf8,			/* 11111000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 182 0xb6 '¶'
	*/
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0xf6,			/* 11110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */

	/*
	* 183 0xb7 '·'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xfe,			/* 11111110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */

	/*
	* 184 0xb8 '¸'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xf8,			/* 11111000 */
	0x18,			/* 00011000 */
	0xf8,			/* 11111000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 185 0xb9 '¹'
	*/
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0xf6,			/* 11110110 */
	0x06,			/* 00000110 */
	0xf6,			/* 11110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */

	/*
	* 186 0xba 'º'
	*/
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */

	/*
	* 187 0xbb '»'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xfe,			/* 11111110 */
	0x06,			/* 00000110 */
	0xf6,			/* 11110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */

	/*
	* 188 0xbc '¼'
	*/
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0xf6,			/* 11110110 */
	0x06,			/* 00000110 */
	0xfe,			/* 11111110 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 189 0xbd '½'
	*/
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0xfe,			/* 11111110 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 190 0xbe '¾'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0xf8,			/* 11111000 */
	0x18,			/* 00011000 */
	0xf8,			/* 11111000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 191 0xbf '¿'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xf8,			/* 11111000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 192 0xc0 'À'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x1f,			/* 00011111 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 193 0xc1 'Á'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0xff,			/* 11111111 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 194 0xc2 'Â'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xff,			/* 11111111 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 195 0xc3 'Ã'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x1f,			/* 00011111 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 196 0xc4 'Ä'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xff,			/* 11111111 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 197 0xc5 'Å'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0xff,			/* 11111111 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 198 0xc6 'Æ'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x1f,			/* 00011111 */
	0x18,			/* 00011000 */
	0x1f,			/* 00011111 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 199 0xc7 'Ç'
	*/
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x37,			/* 00110111 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */

	/*
	* 200 0xc8 'È'
	*/
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x37,			/* 00110111 */
	0x30,			/* 00110000 */
	0x3f,			/* 00111111 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 201 0xc9 'É'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x3f,			/* 00111111 */
	0x30,			/* 00110000 */
	0x37,			/* 00110111 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */

	/*
	* 202 0xca 'Ê'
	*/
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0xf7,			/* 11110111 */
	0x00,			/* 00000000 */
	0xff,			/* 11111111 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 203 0xcb 'Ë'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xff,			/* 11111111 */
	0x00,			/* 00000000 */
	0xf7,			/* 11110111 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */

	/*
	* 204 0xcc 'Ì'
	*/
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x37,			/* 00110111 */
	0x30,			/* 00110000 */
	0x37,			/* 00110111 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */

	/*
	* 205 0xcd 'Í'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xff,			/* 11111111 */
	0x00,			/* 00000000 */
	0xff,			/* 11111111 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 206 0xce 'Î'
	*/
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0xf7,			/* 11110111 */
	0x00,			/* 00000000 */
	0xf7,			/* 11110111 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */

	/*
	* 207 0xcf 'Ï'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0xff,			/* 11111111 */
	0x00,			/* 00000000 */
	0xff,			/* 11111111 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 208 0xd0 'Ð'
	*/
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0xff,			/* 11111111 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 209 0xd1 'Ñ'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xff,			/* 11111111 */
	0x00,			/* 00000000 */
	0xff,			/* 11111111 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 210 0xd2 'Ò'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xff,			/* 11111111 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */

	/*
	* 211 0xd3 'Ó'
	*/
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x3f,			/* 00111111 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 212 0xd4 'Ô'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x1f,			/* 00011111 */
	0x18,			/* 00011000 */
	0x1f,			/* 00011111 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 213 0xd5 'Õ'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x1f,			/* 00011111 */
	0x18,			/* 00011000 */
	0x1f,			/* 00011111 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 214 0xd6 'Ö'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x3f,			/* 00111111 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */

	/*
	* 215 0xd7 '×'
	*/
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0xff,			/* 11111111 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */

	/*
	* 216 0xd8 'Ø'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0xff,			/* 11111111 */
	0x18,			/* 00011000 */
	0xff,			/* 11111111 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 217 0xd9 'Ù'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0xf8,			/* 11111000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 218 0xda 'Ú'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x1f,			/* 00011111 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 219 0xdb 'Û'
	*/
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */

	/*
	* 220 0xdc 'Ü'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */

	/*
	* 221 0xdd 'Ý'
	*/
	0xf0,			/* 11110000 */
	0xf0,			/* 11110000 */
	0xf0,			/* 11110000 */
	0xf0,			/* 11110000 */
	0xf0,			/* 11110000 */
	0xf0,			/* 11110000 */
	0xf0,			/* 11110000 */
	0xf0,			/* 11110000 */

	/*
	* 222 0xde 'Þ'
	*/
	0x0f,			/* 00001111 */
	0x0f,			/* 00001111 */
	0x0f,			/* 00001111 */
	0x0f,			/* 00001111 */
	0x0f,			/* 00001111 */
	0x0f,			/* 00001111 */
	0x0f,			/* 00001111 */
	0x0f,			/* 00001111 */

	/*
	* 223 0xdf 'ß'
	*/
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0xff,			/* 11111111 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 224 0xe0 'à'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x76,			/* 01110110 */
	0xdc,			/* 11011100 */
	0xc8,			/* 11001000 */
	0xdc,			/* 11011100 */
	0x76,			/* 01110110 */
	0x00,			/* 00000000 */

	/*
	* 225 0xe1 'á'
	*/
	0x78,			/* 01111000 */
	0xcc,			/* 11001100 */
	0xcc,			/* 11001100 */
	0xd8,			/* 11011000 */
	0xcc,			/* 11001100 */
	0xc6,			/* 11000110 */
	0xcc,			/* 11001100 */
	0x00,			/* 00000000 */

	/*
	* 226 0xe2 'â'
	*/
	0xfe,			/* 11111110 */
	0xc6,			/* 11000110 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0xc0,			/* 11000000 */
	0x00,			/* 00000000 */

	/*
	* 227 0xe3 'ã'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0xfe,			/* 11111110 */
	0x6c,			/* 01101100 */
	0x6c,			/* 01101100 */
	0x6c,			/* 01101100 */
	0x6c,			/* 01101100 */
	0x00,			/* 00000000 */

	/*
	* 228 0xe4 'ä'
	*/
	0xfe,			/* 11111110 */
	0xc6,			/* 11000110 */
	0x60,			/* 01100000 */
	0x30,			/* 00110000 */
	0x60,			/* 01100000 */
	0xc6,			/* 11000110 */
	0xfe,			/* 11111110 */
	0x00,			/* 00000000 */

	/*
	* 229 0xe5 'å'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0xd8,			/* 11011000 */
	0xd8,			/* 11011000 */
	0xd8,			/* 11011000 */
	0x70,			/* 01110000 */
	0x00,			/* 00000000 */

	/*
	* 230 0xe6 'æ'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x7c,			/* 01111100 */
	0xc0,			/* 11000000 */

	/*
	* 231 0xe7 'ç'
	*/
	0x00,			/* 00000000 */
	0x76,			/* 01110110 */
	0xdc,			/* 11011100 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */

	/*
	* 232 0xe8 'è'
	*/
	0x7e,			/* 01111110 */
	0x18,			/* 00011000 */
	0x3c,			/* 00111100 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x3c,			/* 00111100 */
	0x18,			/* 00011000 */
	0x7e,			/* 01111110 */

	/*
	* 233 0xe9 'é'
	*/
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0xc6,			/* 11000110 */
	0xfe,			/* 11111110 */
	0xc6,			/* 11000110 */
	0x6c,			/* 01101100 */
	0x38,			/* 00111000 */
	0x00,			/* 00000000 */

	/*
	* 234 0xea 'ê'
	*/
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x6c,			/* 01101100 */
	0x6c,			/* 01101100 */
	0xee,			/* 11101110 */
	0x00,			/* 00000000 */

	/*
	* 235 0xeb 'ë'
	*/
	0x0e,			/* 00001110 */
	0x18,			/* 00011000 */
	0x0c,			/* 00001100 */
	0x3e,			/* 00111110 */
	0x66,			/* 01100110 */
	0x66,			/* 01100110 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */

	/*
	* 236 0xec 'ì'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0xdb,			/* 11011011 */
	0xdb,			/* 11011011 */
	0x7e,			/* 01111110 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 237 0xed 'í'
	*/
	0x06,			/* 00000110 */
	0x0c,			/* 00001100 */
	0x7e,			/* 01111110 */
	0xdb,			/* 11011011 */
	0xdb,			/* 11011011 */
	0x7e,			/* 01111110 */
	0x60,			/* 01100000 */
	0xc0,			/* 11000000 */

	/*
	* 238 0xee 'î'
	*/
	0x1e,			/* 00011110 */
	0x30,			/* 00110000 */
	0x60,			/* 01100000 */
	0x7e,			/* 01111110 */
	0x60,			/* 01100000 */
	0x30,			/* 00110000 */
	0x1e,			/* 00011110 */
	0x00,			/* 00000000 */

	/*
	* 239 0xef 'ï'
	*/
	0x00,			/* 00000000 */
	0x7c,			/* 01111100 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0xc6,			/* 11000110 */
	0x00,			/* 00000000 */

	/*
	* 240 0xf0 'ð'
	*/
	0x00,			/* 00000000 */
	0xfe,			/* 11111110 */
	0x00,			/* 00000000 */
	0xfe,			/* 11111110 */
	0x00,			/* 00000000 */
	0xfe,			/* 11111110 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 241 0xf1 'ñ'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x7e,			/* 01111110 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0x00,			/* 00000000 */

	/*
	* 242 0xf2 'ò'
	*/
	0x30,			/* 00110000 */
	0x18,			/* 00011000 */
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0x00,			/* 00000000 */

	/*
	* 243 0xf3 'ó'
	*/
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0x18,			/* 00011000 */
	0x0c,			/* 00001100 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0x00,			/* 00000000 */

	/*
	* 244 0xf4 'ô'
	*/
	0x0e,			/* 00001110 */
	0x1b,			/* 00011011 */
	0x1b,			/* 00011011 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */

	/*
	* 245 0xf5 'õ'
	*/
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0xd8,			/* 11011000 */
	0xd8,			/* 11011000 */
	0x70,			/* 01110000 */

	/*
	* 246 0xf6 'ö'
	*/
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x7e,			/* 01111110 */
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 247 0xf7 '÷'
	*/
	0x00,			/* 00000000 */
	0x76,			/* 01110110 */
	0xdc,			/* 11011100 */
	0x00,			/* 00000000 */
	0x76,			/* 01110110 */
	0xdc,			/* 11011100 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 248 0xf8 'ø'
	*/
	0x38,			/* 00111000 */
	0x6c,			/* 01101100 */
	0x6c,			/* 01101100 */
	0x38,			/* 00111000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 249 0xf9 'ù'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 250 0xfa 'ú'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x18,			/* 00011000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 251 0xfb 'û'
	*/
	0x0f,			/* 00001111 */
	0x0c,			/* 00001100 */
	0x0c,			/* 00001100 */
	0x0c,			/* 00001100 */
	0xec,			/* 11101100 */
	0x6c,			/* 01101100 */
	0x3c,			/* 00111100 */
	0x1c,			/* 00011100 */

	/*
	* 252 0xfc 'ü'
	*/
	0x6c,			/* 01101100 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x36,			/* 00110110 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 253 0xfd 'ý'
	*/
	0x78,			/* 01111000 */
	0x0c,			/* 00001100 */
	0x18,			/* 00011000 */
	0x30,			/* 00110000 */
	0x7c,			/* 01111100 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 254 0xfe 'þ'
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x3c,			/* 00111100 */
	0x3c,			/* 00111100 */
	0x3c,			/* 00111100 */
	0x3c,			/* 00111100 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

	/*
	* 255 0xff ' '
	*/
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */
	0x00,			/* 00000000 */

};

#endif

// --- End of file SDL_bgi.h
