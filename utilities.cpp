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
