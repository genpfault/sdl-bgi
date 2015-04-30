// do your test using this template

#include <SDL2/SDL_bgi.h>

int main (void)
{
  
  initgraph (NULL, NULL, "");
  
  setcolor (RED);
  setlinestyle (0, 0, THICK_WIDTH);
  circle (400, 300, 200);
  getch ();
  
  restorecrtmode ();
  puts ("Aspetta 2 secondi per tornare alla grafica: ");
  delay (2000);
  
  setgraphmode (0);
  refresh ();
  
  getch ();
  closegraph ();
  
}

// ----- end of test.c
