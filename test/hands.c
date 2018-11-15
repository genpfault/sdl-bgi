// hands.c

#include <graphics.h>
#include "turtle.h"

void hand (void)
{
  // position and heading are already set
  
  // wrist
  setlinestyle (0, 0, THICK_WIDTH);
  forwd (25);
  setlinestyle (0, 0, NORM_WIDTH);
  savestate ();
  
  // fingers
  turnleft (45);
  forwd (15);
  restorestate ();
  
  turnleft (15);
  forwd (30);
  restorestate ();
  
  turnright (5);
  forwd (35);
  restorestate ();
  
  turnright (20);
  forwd (30);
  restorestate ();
  
  turnright (35);
  forwd (25);
  restorestate ();
 
}

// -----

int main (int argc, char *argv[])
{
  int i, *pos;
  
  initwindow (800, 600);
  setbkcolor (BLACK);
  cleardevice ();
  home ();
  pos = position ();
  printf ("%d %d\n", pos[0], pos[1]);

  for (i = 0; i < 100; i++) {
  
    setcolor (1 + random(MAXCOLORS));
    setposition (random (800), random(600));
    setheading (random(360));
    hand ();
    delay (100);
    refresh ();
    
  }
  
  getch ();
  closegraph ();

}