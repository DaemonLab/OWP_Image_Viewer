#include <FL/Fl.H>          // main FLTK header
#include <FL/Fl_Window.H>   // To create a window
#include <FL/Fl_Box.H>      // To create a box
#include <FL/Fl_Button.H>   // For Buttons
#include <FL/Fl_Menu_Bar.H> // For Menu Bar
#include <FL/Fl_Menu_Item.H> // Menu items
#include <FL/Fl_File_Chooser.H> // For open file dialog box

#include <FL/fl_ask.H>     // For the info and error dialog boxes

#include "utilities.h"  // Our own utility toolkit

#include <vector>
#include <string>
using namespace std;

// Function declarations
void create_all_widgets();
void create_menu_bar();
void update_current_image();

// Callback declarations
void open_cb(Fl_Widget*, void*);        // Called for open file menu
void open_dir_cb(Fl_Widget*, void*);    // Called for open directory  menu
void quit_cb(Fl_Widget*, void*);    // Called for quit menu
void about_cb(Fl_Widget*, void*);    // Called for about menu
void prev_cb(Fl_Widget*, void*);    // Called for prev button
void next_cb(Fl_Widget*, void*);    // Called for next button
void del_cb(Fl_Widget*, void*);     // Called for delete button
void zoom_cb(Fl_Widget*, void*);    // Called for zoom button

// Global variable
Ex_Fl_Box *image_box;          // Just a box to display the required images
vector<string> all_files;   // List of files in opened directory
int curfile = 0;            // Current photo number in the list that is displayed

Ex_Fl_Window *window;   // Our extended window which will do resize for us

int main (int argc, char ** argv)
{
  window = new Ex_Fl_Window (400, 460);        // Crete a window 400 X 460 (width*height) in pixels size

  create_all_widgets();
  create_menu_bar();

  window->resizable(image_box); // Make the window resizable with image_box being the one which is resized most
  window->set_cb(update_current_image); // Set the callback when redrawing window
  window->set_imgbox(image_box);
  window->end ();              // Do NOT add any more widgets to this window (even if they are created in code)
  window->show (argc, argv);            // Show the window

  return(Fl::run());                    // Run till all the windows created are exited and then return
}

void create_all_widgets()
{
    // Create a box which will be used to display the image. First first two numbers are
    // coordinates of the top left corner of the box. The later two numbers are width and height of the box
    image_box = new Ex_Fl_Box (40, 40, 320, 340, "Image goes here");

    // Create some buttons, first two numbers are coordinates of the top left
    // corner of the button. The later two numbers are width and height of the button
    // Aligning image on a button is necessary because a button tries to save space for text
    // and we need to tell it that there will not be any text on the button
    Fl_Button* prevButton = new Fl_Button(0, 160, 40, 100, "");
    prevButton->image(resize_image(loadImage("images/prev.png"), prevButton));  // Set the image
    prevButton->align(FL_ALIGN_IMAGE_BACKDROP);     // Align the image on the button
    prevButton->callback(prev_cb);
    prevButton->shortcut('a');  // Set the shortcut for this button
    Fl_Button* nextButton = new Fl_Button(360, 160, 40, 100, "");
    nextButton->image(resize_image(loadImage("images/next.png"), nextButton));
    nextButton->align(FL_ALIGN_IMAGE_BACKDROP);
    nextButton->callback(next_cb);
    nextButton->shortcut('d');

    Fl_Button* zoomButton = new Fl_Button(130, 400, 50, 50, "");
    zoomButton->image(resize_image(loadImage("images/zoom.png"), zoomButton));
    zoomButton->align(FL_ALIGN_IMAGE_BACKDROP);
    zoomButton->callback(zoom_cb);
    Fl_Button* delButton = new Fl_Button(200, 400, 50, 50, "");
    delButton->image(resize_image(loadImage("images/delete.png"), delButton));
    delButton->align(FL_ALIGN_IMAGE_BACKDROP);
    delButton->callback(del_cb);
}

void create_menu_bar()
{
    /** The constructor f a menu item takes following arguments
            A string to display for the menu
            A shortcut that will do the same thing as the menu
            A callback functor to call when this is invoked
            Any extra data that this menu can store
            Any extra flags like FL_MENU_DIVIDER or FL_SUBMENU
        A 0 for any argument means that argument is not used.
        The last two arguments are not required as their default value is 0.
    **/

    // Create menu items, each element will be a menu item.
    // every menu item categorized with FL_SUBMENU  will be shown on menu bar
    // The ampersand (&) before a letter means that that letter will be used to mark that
    // menu item. Example ALT+F will open File menu.
    // Also every menu item after a FL_SUBMENU will automatically be added into that
    // A null menu item {0} demarcates the end of a sub-menu item list and also the end
    // of this menu items list.
    // FL_COMMAND means Ctrl button should be pressed. So Ctrl+O will do the same thing as Open File menu
    Fl_Menu_Item menuitems[] =
    {
        { "&File", 0, 0, 0, FL_SUBMENU },
            { "&Open File...",    FL_COMMAND + 'o', (Fl_Callback *)open_cb, 0,  FL_MENU_DIVIDER },
            { "&Open Directory...",    FL_COMMAND + 'd', (Fl_Callback *)open_dir_cb, 0,  FL_MENU_DIVIDER },
            { "E&xit", FL_COMMAND + 'q', (Fl_Callback *)quit_cb, 0 },
            { 0 },
        { "&Help", 0, 0, 0, FL_SUBMENU },
            { "&About",       FL_COMMAND + 'a', (Fl_Callback *)about_cb },
            { 0 },
        { 0 }
    };
    // Add the menu items to the menu bar
    Fl_Menu_Bar *m = new Fl_Menu_Bar(0, 0, 400, 30);    // top left corner and w,h of menubar
    m->copy(menuitems);
}

void open_cb(Fl_Widget*, void*)
{
    // Undo any directory loading stuff
    all_files.clear();
    curfile = 0;
    // Open a new image using the standard file chooser dialog box
    // First argument is title of the window. Third argument is default/initial filename
    // Fourth argument =0 means the returned filename is absolute path else it will be relative
    // The second argument is "Filter text(ext_filters)" where ext_filters are used to show the files
    // in this case, all the bmp, jpeg, jpg and png files will be shown
    char * file_selected = fl_file_chooser	("Select an Image file","Image files (*.{bmp,jpg,jpeg,png})","", 0);
    if(file_selected != NULL)   // If the used selected a file
    {
        all_files.push_back(string(file_selected)); // Add one file to the list
        curfile = 0;    // Set current index into the list
        update_current_image(); // Update the image
    }
}

void open_dir_cb(Fl_Widget* widget, void* v)
{
    // Open a directory with image files
    // the 3 arguments are like 1st, 3rd and 4th arguments or fl_file_chooser
    char* dirname = fl_dir_chooser("Select a directory with image files","", 0);
    if(dirname != NULL)
    {
        populate_image_filenames(dirname, all_files);
        if(all_files.size() == 0)
        {
            image_box->image(NULL);  // Remove the image
            image_box->label("The directory doesn't contain any image files!");   // Remove Info
        }
        else
        {
            curfile = 0;        // Set the index of current file into the list
            update_current_image();
        }
    }
}

void quit_cb(Fl_Widget* widget, void* v)
{
    // Close everything
    exit(0);
}

void about_cb(Fl_Widget* widget, void* v)
{
    // Show about dialog
    fl_message_title("About");  // Set the title of the box
    fl_message("This application is made under our series One Week Projects (OWP).\n"
               "It is meant to provide a basic tutorial for developing cross platform GUi in C++.\n"
               "This is series is aimed to promote learning via coding and provide students\n"
               "with enough material so that they can google around and learn more on their own.\n"
               "More more such projects check our official account on github:\n"
               "            https://github.com/DaemonLab");     // Display the message
}

void prev_cb(Fl_Widget* widget, void* v)
{
    if (all_files.size()==0 || curfile==0)
        return;
    --curfile;
    image_box->set_zoomed(false);
    update_current_image();
}

void next_cb(Fl_Widget* widget, void* v)
{
    if (all_files.size()==0 || curfile==(all_files.size()-1))
        return;
    ++curfile;
    image_box->set_zoomed(false);
    update_current_image();
}

void del_cb(Fl_Widget* widget, void* v)
{
    if(all_files.size() == 0)
        return;
    int choice = fl_choice("Do you really want to delete this file on disk?",
                           "Yes", "No", 0);         // Text for upto 3 buttons
    if (choice == 0)    //Yes
    {
        // Call the C++ stdio function 'remove' to delete the file on disk
        if (remove(all_files[curfile].c_str()) != 0)    // Error while deleting
        {
            // Show Error dialog
            fl_message_title("Error");  // Set the title of the box
            fl_alert("File could not be deleted!\n"
                       "Please check that you have proper permission for the operation@");
        }
        else
        {
            // Remove the file from the list
            all_files.erase(all_files.begin() + curfile);
            if (curfile >= all_files.size())
                curfile = all_files.size()-1;
            // Show info dialog
            fl_message_title("Info");  // Set the title of the box
            fl_message("File deleted successfully!");     // Display the message
            update_current_image();
        }
    }
}

void zoom_cb(Fl_Widget* widget, void* v)
{
    image_box->set_zoomed(!image_box->is_zoomed());
}

void update_current_image()
{
    if(all_files.size()==0)     // No image in the list
    {
        image_box->image(NULL);
        image_box->label("No image to display!");
        image_box->redraw();    // Force redraw to update the window
        return;
    }
    Fl_Image* img = loadImage(all_files[curfile].c_str());   // Use our utility to load the image
    if (!img)   // Image couldn't be loaded
    {
        // Display and error message and return
        image_box->image(NULL);
        image_box->label("The image could not be loaded!\n"
                         "Please verify that the file is a valid image file!");
        image_box->redraw();    // Force redraw to update the window
        return;
    }
    image_box->label("");   // Remove text
    image_box->image(img);  // Set the image -- Size will be decided by zoom
    // This function is already being called in draw function so need to force redraw
}
