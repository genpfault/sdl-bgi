// plasma.c
// used to produce the plasma.bmp file
// gcc plasma.c -lSDL_bgi -lSDL2 -lm

#include <math.h>
#include <graphics.h>

#define PI 3.141593

int main (void)
{

  int i, x, y, r, g, b;
  int cols[3][255];
  double c;
  
  initwindow (600, 600);
  
  for (i = 0; i < 255; i++) {

    cols[0][i] = (int) fabs( (128.0 - 127.0 * sin ((double) i * PI / 32.0)));
    cols[1][i] = (int) fabs( (128. - 127. * sin((double) i * PI / 64.)));
    cols[2][i] = (int) fabs( (128. - 127. * sin((double) i * PI / 128.)));
	  
  }
  
  for (y = 0; y < getmaxy (); y++) {
    for (x = 0; x < getmaxx (); x++) {
      c = (sin(x/35.)*128.+sin(y/28.)*32. + sin((x+y)/16.)*64.);
      if (c > 255)
	c = c - 256;
      if (c < 0)
	c = 256 + c;
      r = cols[0][(int)c];
      if (r > 255)
	r = r - 256;
      if (r < 0)
	r = 256 + c;
      g = cols[1][(int)c];
      if (g > 255)
	g = g - 256;
      if (g < 0) 
	g = 256 + c;
      b = cols[2][(int)c];
      if (b > 255)
	b = b - 256;
      if (b < 0)
	b = 256 + c;
      setcolor (COLOR(r, g, b));
      _putpixel(x, y);
    }
  }
  refresh ();
  writeimagefile ("plasma.bmp", 0, 0, 599, 599);
  getch ();
  closegraph ();
  return 0;

}

// end of file plasma.c
