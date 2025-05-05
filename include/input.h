#ifndef INPUT_H
#define INPUT_H

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/XTest.h>
#include <X11/extensions/Xkbstr.h>

void init_input(Display *display, Window window);
int handle_input_event(XEvent *event);

#endif
