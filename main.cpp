#include <FL/Fl.H>          // main FLTK header
#include <FL/Fl_Window.H>   // To create a window
#include <FL/Fl_Box.H>      // To create a box
#include <FL/Fl_Button.H>   // For Buttons
#include <FL/Fl_Menu_Bar.H> // For Menu Bar
#include <FL/Fl_Menu_Item.H> // Menu items

#include <FL/fl_ask.H>

// Function declarations
void create_all_widgets();
void create_menu_bar();
void open_cb(Fl_Widget*, void*);    // Called for open menu
void quit_cb(Fl_Widget*, void*);    // Called for quit menu
void about_cb(Fl_Widget*, void*);    // Called for about menu

int main (int argc, char ** argv)
{
  Fl_Window *window;        // The outer window with minimize, maximize, close buttons
  window = new Fl_Window (400, 460);        // Crete a window 400 X 460 (width*height) in pixels size

  create_all_widgets();
  create_menu_bar();

  window->end ();              // Do NOT add any more widgets to this window (even if they are created in code)
  window->show (argc, argv);            // Show the window

  return(Fl::run());                    // Run till all the windows created are exited and then return
}

void create_all_widgets()
{
    Fl_Box *box;          // Just a box to display the required images
    // Create a box which will be used to display the image. First first two numbers are
    // coordinates of the top left corner of the box. The later two numbers are width and height of the box
    box = new Fl_Box (20, 40, 360, 340, "Image goes here");
    box->box (FL_UP_BOX);     // The box should have a 3D feel that it's UP

    // Create some buttons, first two numbers are coordinates of the top left
    // corner of the button. The later two numbers are width and height of the button
    Fl_Button* prevButton = new Fl_Button(60, 400, 50, 50, "Prev");
    Fl_Button* zoomButton = new Fl_Button(130, 400, 50, 50, "Zoom");
    Fl_Button* delButton = new Fl_Button(200, 400, 50, 50, "Delete");
    Fl_Button* nextButton = new Fl_Button(270, 400, 50, 50, "Next");
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
    // Open a new image
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
