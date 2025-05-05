#include "input.h"
#include <stdio.h>

void init_input(Display *display, Window window) {
    // Future expansion for XInput2 or Xkb
}

int handle_input_event(XEvent *event) {
    switch (event->type) {
        case KeyPress: {
            KeySym keysym = XLookupKeysym(&event->xkey, 0);
            if (keysym == XK_q || keysym == XK_Escape) {
                return 0;  // Quit
            }
            break;
        }
        default:
            break;
    }
    return 1;
}
