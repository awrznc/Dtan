#include <time.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "dtan.h"

#define FONT_TYPE "-*-fixed-medium-r-normal--24-*-*-*"

static void wait(double seconds) {

    #define MILLI_SECONDS_RATE 1000000
    #define SECONDS_RATE 1000

    struct timespec time = { 0, (long long)(seconds * SECONDS_RATE * MILLI_SECONDS_RATE) };
    nanosleep( &time, 0 );
}

/**
  * @brief Main loop.
  * @param [in] dtanObject Dtan Object.
  * @param [in] display Connection to the X server.
  * @param [in] window Window.
  * @param [in] gc GC.
  * @param [in] fontSet Font.
  **/
static int run(DtanObject* dtanObject, Display* display, Window window, GC gc, XFontSet fontSet) {
    XEvent event;

    // Update
    wait(0.5);
    while (1) {

        XNextEvent(display, &event);
        if (event.type == Expose) {
            XmbDrawImageString(
                display,
                window,
                fontSet,
                gc,
                DisplayWidth(display, 0)/2,
                DisplayHeight(display, 0)/2,
                dtanObject->display_string,
                strlen(dtanObject->display_string)
            );
        } else if (event.type == ButtonPress) {
            break;
        }
    }
}

int DtanRun (DtanObject* dtanObject){
    Display *display = XOpenDisplay (NULL);
    unsigned int rootWidth = DisplayWidth(display, 0);
    unsigned int rootHeight = DisplayHeight(display, 0);

    Window window = XCreateSimpleWindow(
        display,
        DefaultRootWindow(display),
        0, 0,                       // Point (X, Y)
        DisplayWidth(display, 0),   // Width
        DisplayHeight(display, 0),  // Height
        0, 0,                       // Border Size
        0xffffff                    // Background Color
    );

    // Remove Border
    XSelectInput(display, window, ExposureMask);
    long int data = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", False);
    long int property = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
    XChangeProperty(display, window, property, XA_ATOM, 32, PropModeReplace, (unsigned char *) &data, 1);

    // Background
    XSetWindowBackgroundPixmap( display, window, None );

    // Draw Window
    XMapWindow(display, window);
    XFlush(display);

    // Text
    GC gc = XCreateGC(display, window, 0, 0);
    XSetBackground(display, gc, BlackPixel(display, 0));
    XSetForeground(display, gc, WhitePixel(display, 0));
    char** string;
    int char_count;
    char* char_list;
    XFontSet font_set = XCreateFontSet( display, FONT_TYPE, &string, &char_count, &char_list );
    XFreeStringList(string);

    run(dtanObject, display, window, gc, font_set);

    XDestroyWindow( display, window );
    XCloseDisplay( display );
    return 0;
}
