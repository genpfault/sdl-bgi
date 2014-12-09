/* SDL_bgi.h	-*- C -*-
 * 
 * A BGI-like graphics library based on SDL and SDL_gfx.
 * Easy to use and useful for porting old programs.
 * Guido Gonzato, December 2014.
 *
 */

#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>

// BGI fonts

#define DEFAULT_FONT     0 // 8x8
#define TRIPLEX_FONT     1 // 7x13
#define SMALL_FONT       2 // 5x8
#define SANSSERIF_FONT   3 // 9x18
#define GOTHIC_FONT      4 // 10x20
#define BIG_FONT         5 // 16x26
#define SCRIPT_FONT      6 // 10x20
#define SIMPLEX_FONT     7 // 10x20
#define TRIPLEX_SCR_FONT 8 // 10x20
#define COMPLEX_FONT     9 // 10x20
#define EUROPEAN_FONT    10 // 10x20
#define BOLD_FONT        11 // 10x20

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
#define WM_WHEELUP      SDL_BUTTON_WHEELUP
#define WM_WHEELDOWN    SDL_BUTTON_WHEELDOWN
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
#define KEY_LEFT_WIN    SDLK_LSUPER
#define KEY_RIGHT_WIN   SDLK_RSUPER
#define KEY_ALT_GR      SDLK_MODE
#define KEY_TAB         SDLK_TAB
#define KEY_BS          SDLK_BACKSPACE
#define KEY_RET         SDLK_RETURN
#define KEY_PAUSE       SDLK_PAUSE
#define KEY_SCR_LOCK    SDLK_SCROLLOCK
#define KEY_ESC         SDLK_ESCAPE

// graphics modes

#define DETECT          0
#define grOk            0
#define X11_CGALO       0
#define X11_CGAHI       1
#define EGA             2
#define EGALO           2
#define X11_EGA         2
#define X11             3
#define VGA             3
#define X11_VGA         3
#define X11_640x480     3
#define X11_HERC        4
#define X11_PC3270      5
#define X11_SVGALO      6
#define X11_800x600     6
#define X11_SVGAMED1    7
#define X11_1024x768    7
#define X11_SVGAMED2    8
#define X11_1152x900    8
#define X11_SVGAHI      9
#define X11_1280x1024   9
#define X11_WXGA        10
#define X11_1366x768    10
#define X11_USER        11
#define X11_FULLSCREEN  12

// everything gets drawn here

extern SDL_Surface *BGI;
extern int button;

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

struct bgi_info {
  int colour_index;
  char *colour_name;
  unsigned long pixel_value;
};

// prototypes

void arc (int, int, int, int, int);
void bar3d (int, int, int, int, int, int);
void bar (int, int, int, int);
int  BLUE_VALUE (int);
void circle (int, int, int);
void cleardevice ();
void clearviewport ();
void closegraph (void);
int  COLOR (int, int, int);
#define delay(msec)   (usleep (msec*1000))
void detectgraph (int *, int *);
void drawpoly (int, int *);
void ellipse (int, int, int, int, int, int);
int  event (void);
void fillellipse (int, int, int, int);
void fillpoly (int, int *);
void floodfill (int, int, int);
void freeimage (void *);
void getarccoords (struct arccoordstype *);
void getaspectratio (int *, int *);
int  getbkcolor (void);
int  getch (void);
int  getcolor (void);
struct palettetype *getdefaultpalette (void);
char *getdrivername (void);
int  getevent (void);
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
void getmouseclick (int, int *, int *);
void getpalette (struct palettetype *);
int  getpalettesize (struct palettetype *);
unsigned int getpixel (int, int);
void gettextsettings (struct textsettingstype *);
void getviewsettings (struct viewporttype *);
int  getx (void);
int  gety (void);
void graphdefaults ();
char *grapherrormsg (int);
int  graphresult (void);
int  GREEN_VALUE (int);
unsigned imagesize (int, int, int, int);
void initgraph (int *, int *, char *);
void initwindow (int, int);
int  installuserdriver (char *, int *);
int  installuserfont (char *);
int  IS_BGI_COLOR (int color);
int  ismouseclick (int);
int  IS_RGB_COLOR (int color);
int  kbhit (void);
void line (int, int, int, int);
void linerel (int dx, int dy);
void lineto (int x, int y);
int  mouseclick (void);
int  mousex (void);
int  mousey (void);
void moverel (int, int);
void moveto (int, int);
void outtext (char *);
void outtextxy (int, int, char *);
void pieslice (int, int, int, int, int);
void _putpixel (int, int);
void putimage (int, int, void *, int);
void putpixel (int, int, int);
#define random(range) (rand() % (range))
void rectangle (int, int, int, int);
int  RED_VALUE (int );
void refresh (void);
int  registerbgidriver (void *);
int  registerbgifont (void *);
void restorecrtmode (void);
void sector (int, int, int, int, int, int);
void setactivepage (int);
void setallpalette (struct palettetype *);
void setaspectratio (int, int);
void setbkcolor (int);
void setbkrgbcolor (int);
void setcolor (int);
void setfillpattern (char *, int); 
void setfillstyle (int, int);
unsigned int setgraphbufsize (unsigned int);
void setgraphmode (int); 
void setlinestyle (int, unsigned, int);
void setallpalette (struct palettetype *); 
void setaspectratio (int, int);
void setpalette (int, int);
void setrgbcolor (int);
void setrgbpalette (int, int, int, int);
void settextjustify (int, int);
void settextstyle (int, int, int);
void setusercharsize (int, int, int, int);
void setviewport (int, int, int, int, int);
void setvisualpage (int); // TODO
void setwritemode (int);
int  textheight (char *);
int  textwidth (char *);

// --- End of file SDL_bgi.h
