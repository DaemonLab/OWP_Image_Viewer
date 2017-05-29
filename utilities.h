#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <FL/Fl_Image.H>

#include <vector>
#include <string>
using namespace std;

// Load the image from file
Fl_Image* loadImage(const char* path);

// Fill the vector with names of all image files from the dirname
void populate_image_filenames(char* dirname, vector<string> &v);

#endif // __UTILITIES_H__
