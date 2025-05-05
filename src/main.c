#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include "x11_window.h"
#include "renderer.h"
#include "input.h"

int main(int argc, char *argv[]) {
    if (!XInitThreads()) {
        fprintf(stderr, "Failed to initialize X11 thread support.\n");
        return 1;
    }

    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }

    Window window = create_main_window(display);
    if (!window) {
        fprintf(stderr, "Failed to create window\n");
        return 1;
    }

    XMapWindow(display, window);
    XFlush(display);

    init_input(display, window);
    init_renderer(display, window);

    int running = 1;
    while (running) {
        XEvent event;
        while (XPending(display)) {
            XNextEvent(display, &event);
            if (handle_input_event(&event) == 0) {
                running = 0;
            }
        }

        render_frame();
    }

    XCloseDisplay(display);
    return 0;
}
