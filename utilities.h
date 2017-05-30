#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <FL/Fl_Image.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>

#include <vector>
#include <string>
using namespace std;

// Load the image from file
Fl_Image* loadImage(const char* path);

// Fill the vector with names of all image files from the dirname
void populate_image_filenames(char* dirname, vector<string> &v);

// Resize the image to be of the widget's size
Fl_Image* resize_image(Fl_Image* orig, Fl_Widget* widget);

// Callback to be used for redraw update
typedef void (*Ex_Redraw_Callback)();

// A subclass of Fl_Window that properly handles the redraw/resize of window
class Ex_Fl_Window : public Fl_Window
{
    Ex_Redraw_Callback redraw_cb;   // An update callback
public:
    Ex_Fl_Window(int w, int h);     // Constructor

    void set_cb(Ex_Redraw_Callback cb);     // to set the callback
    void draw();    // To call the callback and then draw
};

#endif // __UTILITIES_H__
