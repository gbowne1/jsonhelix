#include "renderer.h"
#include <stdio.h>

static Display *g_display;
static Window g_window;

void init_renderer(Display *display, Window window) {
    g_display = display;
    g_window = window;
}

void render_frame() {
    XClearWindow(g_display, g_window);

    GC gc = XCreateGC(g_display, g_window, 0, NULL);
    XSetForeground(g_display, gc, BlackPixel(g_display, DefaultScreen(g_display)));

    XDrawString(g_display, g_window, gc, 20, 40, "jsonhelix running...", 21);

    XFreeGC(g_display, gc);
    XFlush(g_display);
}
