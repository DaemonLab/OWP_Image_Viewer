#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <Fl/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>

#include <FL/fl_draw.H>

#include <vector>
#include <string>
using namespace std;

// Load the image from file
Fl_Image* loadImage(const char* path);

// Fill the vector with names of all image files from the dirname
void populate_image_filenames(char* dirname, vector<string> &v);

// Resize the image to be of the widget's size
Fl_Image* resize_image(Fl_Image* orig, Fl_Widget* widget);

// A subclass of Fl_Box which will allow proper zooming
class Ex_Fl_Box : public Fl_Box
{
    bool zoomed;
    int cx, cy;
    Fl_Image* img_orig;
public:
    Ex_Fl_Box(int x, int y, int w, int h, const char* lbl);   // constructor

    void set_zoomed(bool zoomed);   // Set zoom
    bool is_zoomed();   // get if it is zoomed

    // Override functions to handle event and drawing
    int handle(int event);
    void draw();
};

// Callback to be used for redraw update
typedef void (*Ex_Redraw_Callback)();

// A subclass of Fl_Window that properly handles the redraw/resize of window
class Ex_Fl_Window : public Fl_Window
{
    Ex_Redraw_Callback redraw_cb;   // An update callback
    Ex_Fl_Box* img_box;
public:
    Ex_Fl_Window(int w, int h);     // Constructor

    void set_cb(Ex_Redraw_Callback cb);     // to set the callback
    void set_imgbox(Ex_Fl_Box* box);        // To set image box for keypress events

    void draw();    // To call the callback and then draw
    int handle(int event);  // To handle keypress and send them to image box
};

#endif // __UTILITIES_H__
