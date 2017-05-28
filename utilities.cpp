#include "utilities.h"

#include <string.h>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_PNG_Image.H>

Fl_Image* loadImage(char* path)
{
    // All the JPEG/BMP/PNG image classes are subclasses of
    // Fl_Image and hence we can return their pointer in the base class pointer
    int len = strlen(path);
    if (len < 5)
        return NULL;
    if (!strcmp(&path[len-3], "bmp"))    // BMP Image
        return new Fl_BMP_Image(path);
    else if (!strcmp((&path[len-3]), "png"))
        return new Fl_PNG_Image(path);
    else if ((!strcmp((&path[len-3]), "jpg")) ||  (!strcmp((&path[len-4]), "jpeg")))
        return new Fl_JPEG_Image(path);
    else
        return NULL;
}
