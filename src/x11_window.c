#include "x11_window.h"
#include <stdio.h>

Window create_main_window(Display *display) {
    int screen = DefaultScreen(display);
    int width = 1024, height = 768;

    Window root = RootWindow(display, screen);
    Window win = XCreateSimpleWindow(display, root,
                                     0, 0, width, height, 1,
                                     BlackPixel(display, screen),
                                     WhitePixel(display, screen));

    XStoreName(display, win, "jsonhelix");

    XSelectInput(display, win, ExposureMask | KeyPressMask | KeyReleaseMask |
                                   ButtonPressMask | ButtonReleaseMask |
                                   PointerMotionMask | StructureNotifyMask);

    return win;
}
