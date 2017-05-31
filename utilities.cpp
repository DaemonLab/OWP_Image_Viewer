#include "utilities.h"

#include <string.h>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/filename.H>

Fl_Image* loadImage(const char* path)
{
    // All the JPEG/BMP/PNG image classes are subclasses of
    // Fl_Image and hence we can return their pointer in the base class pointer
    int len = strlen(path);
    if (len < 5)
        return NULL;
    if (!strcmp(&path[len-4], ".bmp"))    // BMP Image
        return new Fl_BMP_Image(path);
    else if (!strcmp((&path[len-4]), ".png"))
        return new Fl_PNG_Image(path);
    else if ((!strcmp((&path[len-4]), ".jpg")) ||  (!strcmp((&path[len-5]), ".jpeg")))
        return new Fl_JPEG_Image(path);
    else
        return NULL;
}

void populate_image_filenames(char* dirname, vector<string> &v)
{
    v.clear();
    if (!fl_filename_isdir(dirname))  // If it isn't directory/folder
        return;
    dirent** file_list;
    // List all the files in the directory
    // returns the number of files found
    int num_files = fl_filename_list(dirname, &file_list);
    for(int i=0;i < num_files; ++i)
    {
        char* curfile_name = file_list[i]->d_name;

        int len = strlen(curfile_name);
        string full_path = string(dirname) + "/" + string(curfile_name);    // full_path
        if (!fl_filename_isdir(full_path.c_str()) && len > 4 &&        // If the file is not a directory
            (
              (!strcmp((&curfile_name[len-4]), ".jpg")) ||  (!strcmp((&curfile_name[len-5]), ".jpeg"))
               || (!strcmp((&curfile_name[len-4]), ".png")) || (!strcmp((&curfile_name[len-4]), ".bmp"))
            ))  // And it ends with the following
        {
            // If it is and image file add it to the list
            v.push_back(full_path);
        }
    }
}

Fl_Image* resize_image(Fl_Image* orig, Fl_Widget* widget)
{
    // Create a copy with required width and height
    Fl_Image* tmp_img = orig->copy(widget->w(), widget->h());
    delete orig;    // Clear memory
    return tmp_img; // Return new image
}

Ex_Fl_Box::Ex_Fl_Box(int x, int y, int w, int h, const char* lbl) : Fl_Box(x, y, w, h, lbl)
{
    zoomed = false;
}

void Ex_Fl_Box::set_zoomed(bool zoomed)
{
    if(!this->zoomed && zoomed)
    {
        img_orig = image();
        if (img_orig == NULL)   // return if there's no image
            return;
        // Set the zoomed image to be an image with twice the original size
        image(img_orig->copy(img_orig->w()*2, img_orig->h()*2));
        this->zoomed = true;
        // The zoom coordinates in coordinates of the picture
        this->cx = this->w()/2;
        this->cy = this->h()/2;
    }
    else if(this->zoomed && !zoomed)
    {
        // delete the zoomed image and restore the original one
        Fl_Image* zoomed_img = image();
        delete zoomed_img;
        image(img_orig);
        img_orig = NULL;
        this->zoomed = false;
    }
    this->redraw();
}

bool Ex_Fl_Box::is_zoomed()
{
    return zoomed;  // return zoomed state
}

int Ex_Fl_Box::handle(int event)
{
    switch(event)
    {
    case FL_KEYDOWN:
        if(zoomed)
        {
            int key = Fl::event_key();	// Get the key pressed during last event
            int imgw = image()->w(), imgh = image()->h();   // Width and height of current image
            // The zoom is centred at (cx,cy) so they have min and max values
            // cx and cy are on the image and are displayed at centre of the box
            int mincx = this->w()/2, mincy = this->h()/2;
            int maxcx = imgw - mincx, maxcy = imgh - mincy;
            switch(key)
            {
            case 'i':
                cy -= this->h()/10;     // Shift the centre y by 0.1 times the height of the box
                cy = cy < mincy ? mincy : cy;     // clip cy at zero
                break;
            case 'j':
                cx -= this->w()/10;     // Shift the centre x by 0.1 times the width of the box
                cx = cx < mincx ? mincx : cx;     // clip cx at zero
                break;
            case 'k':
                cy += this->h()/10;     // Shift the centre y by 0.1 times the height of the box
                cy = cy > maxcy ? maxcy : cy;     // clip cy at image height
                break;
            case 'l':
                cx += this->w()/10;     // Shift the centre x by 0.1 times the width of the box
                cx = cx > maxcx ? maxcx : cx;     // clip cx at image width
                break;
            default:
                break;
            }
            this->redraw(); // Force redraw for this event
        }
        break;
    default:
        break;
    }
    return Fl_Box::handle(event);       // return with default handler
}

void Ex_Fl_Box::draw()
{
    Fl_Image* img = Fl_Box::image();
    if(img != NULL)
    {
        // Set the drawing region. Everything drawn from now on will be clipped in this rectangle
        // Normally FLTK doesn't check if a widget is drawing all over the window for
        // performance reasons but we don't want the image to be drawn out of the box
        // The clip region doesn't allow any drawing outside this region
        fl_push_clip(this->x(), this->y(), this->w(), this->h());

        int thisw = this->w(), thish = this->h();

        if(!zoomed)
        {
            Fl_Image* img_cpy = img->copy(thisw, thish);
            img_cpy->draw(this->x(), this->y(), thisw, thish);     // If not zoomed then fit the image on the box
            delete img_cpy;
        }
        else
        {
            int imgw = img->w(), imgh = img->h();
            // Draw a small image in the centre
            if(imgw <= thisw && imgh <= thish)
                img->draw((thisw-imgw)/2, (thish-imgh)/2, imgw, imgh);
            else
            {
                // the coordinate of top left corner of the image
                int l = thisw/2 - cx;
                int t = thish/2 - cy;
                img->draw(l, t);
            }
        }
        // Remove the clipping region
        fl_pop_clip();
    }
    else
        Fl_Box::draw();     // No image, use default draw
}

Ex_Fl_Window::Ex_Fl_Window(int w, int h) : Fl_Window(w, h)
{
}

void Ex_Fl_Window::set_cb(Ex_Redraw_Callback cb)
{
    redraw_cb = cb;   // Setting the redraw callback
}

void Ex_Fl_Window::draw()
{
    // Override the draw function to call the redraw update function and then redraw the window
    redraw_cb();
    Fl_Window::draw();
}

int Ex_Fl_Window::handle(int event)
{
    if(event == FL_KEYDOWN) // Keyboard button was pressed
    {
        switch(Fl::event_key())		// Switch to the key pressed
        {
        case 'i':
        case 'j':
        case 'k':
        case 'l':
            // Pass these events to the image box
            return img_box->handle(event);
            break;
        }
    }
    // Do default action for the rest of events
    return Fl_Window::handle(event);
}

void Ex_Fl_Window::set_imgbox(Ex_Fl_Box* box)
{
    // Set the image box to which key press events will be sent
    img_box = box;
}
