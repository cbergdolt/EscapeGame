// testing5.c
// program to test new features in gfx5, including partial clear
//   and cursor selection
// five events are now detected: key press, key release,
//   mouse click, mouse release, mouse move
// printf statements provide feedback at the terminal window
// left mouse click-n-hold creates a "pen" for drawing

#include <stdio.h>
#include <unistd.h>
#include "gfx5.h"

int main()
{
  char c;
  int event;
  int ispressed = 0, isclicked = 0;  // flags for key and mouse events
  int thick = 1;                     // pen thickness 

  gfx_open(800,600, "My window"); 
  gfx_color(255,255,255); 

  while(1) { 
    if(event = gfx_event_waiting()) { 
      c = gfx_wait();
      if (c == 'q')  break;
      if (c == 27) gfx_clear();      // clear the window when ESC is hit
      if (c >= 32 && c <= 126)       // range of most keyboard keys
        if(event == 1) {             // key press event
          printf("you pressed key: %c\n", c);
          ispressed = 1;
        } else if (event == 2) {     // key release event
          if(ispressed) printf("you released key: %c\n", c);
          ispressed = 0;
        }
      if (c == 1 || c == 3) 
        if(event == 3) {             // mouse click event
          printf("you clicked mouse button %d\n", c);
          isclicked = 1;
        } else if (event == 4) {     // mouse release event
          if(isclicked) printf("you unclicked mouse button %d\n", c);
          isclicked = 0;
        }
      if (event == 5) {              // mouse move event
        printf("mouse is moving; it's at: (%d,%d)\n", gfx_xpos(),gfx_ypos());
        if(isclicked) {
          if (thick == 1) gfx_point(gfx_xpos(), gfx_ypos());
          else gfx_fill_circle(gfx_xpos(), gfx_ypos(), thick);
        }
      }
      // change the pen's thickness
      if (c >= '1' && c <= '9') thick = c - '0';
      // change the pen's color
      if (c == 'r') gfx_color(255,0,0);     // red
      if (c == 'g') gfx_color(0,255,0);     // green
      if (c == 'b') gfx_color(0,0,255);     // blue
      if (c == 'w') gfx_color(255,255,255); // white
      if (c == 'y') gfx_color(255,255,0);   // yellow
      if (c == 'c') gfx_color(0,255,255);   // cyan
      if (c == 'm') gfx_color(255,0,255);   // magenta
      // clear a small area at top left when spacebar is hit
      if (c == ' ') gfx_cleararea(50,50,200,200);
      // try a few cursors
      if (c == '=') gfx_changecursor(XC_crosshair);
      if (c == 'w') gfx_changecursor(XC_watch);
      if (c == 'a') gfx_changecursor(XC_arrow);
    }
    usleep(1000);
    gfx_flush();
  }
  return 0;
}

