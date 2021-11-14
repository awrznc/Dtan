#include <time.h>

#include <X11/Xlib.h>               // libx11-dev
#include <X11/Xatom.h>              // libx11-dev
#include <X11/extensions/Xrender.h> // libxrender-dev
#include <X11/Xft/Xft.h>            // libxft-dev libfreetype6-dev

#include "dtan.h"

#define FONT_TYPE "serif"


/**
  * @brief Main loop.
  * @param [in] dtanObject Dtan Object.
  * @param [in] display Connection to the X server.
  * @param [in] window Window.
  * @param [in] xftColor Xft Color.
  * @param [in] font XftFont.
  **/
static int run(
    DtanObject* dtanObject,
    Display* display,
    Window window,
    XftColor xftColor,
    XftFont* font
) {

    // Draw Contents
    {
        XGlyphInfo extents = { 0 };
        XftTextExtents8(display, font, dtanObject->display_string, strlen(dtanObject->display_string), &extents);

        int screen = DefaultScreen (display);
        XftDraw* draw = XftDrawCreate(display, window, DefaultVisual(display, screen), DefaultColormap(display, screen));

        unsigned int width = DisplayWidth(display, 0);
        unsigned int height = DisplayHeight(display, 0);
        XftDrawString8(draw, &xftColor, font, (width/2)-(extents.width/2), height/2, dtanObject->display_string, strlen(dtanObject->display_string));
        XftDrawRect(draw, &xftColor, (width/2)-(extents.width/2), (height/2)+4, extents.width+8, 4);

        XftDrawDestroy(draw);
    }

    // Timer
    XEvent event = { 0 };
    fd_set in_fds = { 0 };
    struct timeval timer = { 0 };
    int x11_fd = ConnectionNumber(display);

    // Update
    while (1) {

        FD_ZERO(&in_fds);
        FD_SET(x11_fd, &in_fds);
        timer.tv_sec = 0;
        timer.tv_usec = UPDATE_PERIOD * 1000;
        select(x11_fd + 1, &in_fds, NULL, NULL, &timer);

        while(XPending(display)) XNextEvent(display, &event);

        if (!DtanUpdateStatus(dtanObject)) return 0;

        XClearWindow(display, window);
    }
}


int DtanRun (DtanObject* dtanObject){

    // Window
    Display *display = XOpenDisplay (NULL);
    int screen = DefaultScreen(display);
    unsigned int width = DisplayWidth(display, 0);
    unsigned int height = DisplayHeight(display, 0);
    Window window = XCreateWindow(
        display,
        DefaultRootWindow(display),
        0, 0, width, height, 0,
        DefaultDepth(display, screen),
        InputOutput,
        DefaultVisual(display, screen),
        0, NULL
    );

    // Remove Frame
    {
        XSelectInput(display, window, ExposureMask);
        long int data = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", False);
        long int property = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
        XChangeProperty(display, window, property, XA_ATOM, 32, PropModeReplace, (unsigned char *) &data, 1);
    }

    // Draw Window
    XMapWindow(display, window);
    XFlush(display);

    // Background
    {
        XSetWindowBackgroundPixmap( display, window, ParentRelative );
        XRenderPictureAttributes attributes = { .poly_edge=PolyEdgeSmooth, .poly_mode=PolyModeImprecise };
        XRenderPictFormat *format = XRenderFindStandardFormat(display, PictStandardRGB24);
        Picture picture = XRenderCreatePicture(display, window, format, CPPolyEdge | CPPolyMode, &attributes);
        XRenderColor color = { .red=0x0000, .green=0x0000, .blue=0x0000, .alpha=0x0000 };
        XRenderFillRectangle(display, PictOpOver, picture, &color, 0, 0, width, height);
    }

    // Color
    XftColor xft_color = { 0 };
    {
        // XRenderColor render_color = { .red=0x4fff, .green=0x4fff, .blue=0x4fff, .alpha=0x0000 };
        XRenderColor render_color = { .red=0xffff, .green=0xffff, .blue=0xffff, .alpha=0x0000 };
        int screen = DefaultScreen (display);
        Colormap colormap = DefaultColormap(display, screen);
        Visual* virtual = DefaultVisual(display, screen);
        XftColorAllocValue( display, virtual, colormap, &render_color, &xft_color);
        XftColorFree(display, virtual, colormap, &xft_color);
    }

    // Font
    XftFont* font = XftFontOpen (
        display, screen,
        XFT_FAMILY, XftTypeString, FONT_TYPE,
        XFT_SIZE, XftTypeInteger, FONT_SIZE,
        NULL
    );

    run(dtanObject, display, window, xft_color, font);

    XftFontClose(display, font);
    XDestroyWindow( display, window );
    XCloseDisplay( display );

    return 0;
}
