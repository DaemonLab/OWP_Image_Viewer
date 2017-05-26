#include <FL/Fl.H>          // main FLTK header
#include <FL/Fl_Window.H>   // To create a window
#include <FL/Fl_Box.H>      // To create a box

int main (int argc, char ** argv)
{
  Fl_Window *window;        // The outer window with minimize, maximize, close buttons
  Fl_Box *box;          // Just a box

  window = new Fl_Window (300, 180);        // Crete a window 300 X 180 in pixels size
  // Crete the box whose top left corner will be at (20,40) and bottom right corner at (260,100)
  // with "Hello World!" written on top of it. Top left corner of window is (0,0) and
  // bottom right corner is (300, 180).
  // This will automatically be added to window by FLTK as window is the
  // most current "grouping widget" created in the code.
  box = new Fl_Box (20, 40, 260, 100, "Hello World!");

  box->box (FL_UP_BOX);     // The box should have a 3D feel that it's UP
  box->labelsize (36);      // Font size of the text on the box
  box->labelfont (FL_BOLD+FL_ITALIC);   // Style of the text
  box->labeltype (FL_SHADOW_LABEL);     // Label has a shadow
  window->end ();              // Do NOT add any more widgets to this window (even if they are created in code)
  window->show (argc, argv);            // Show the window

  return(Fl::run());                    // Run till all the windows created are exited and then return
}
