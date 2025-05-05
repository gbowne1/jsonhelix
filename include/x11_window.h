#ifndef X11_WINDOW_H
#define X11_WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xinerama.h>
#include <X11/Xutil.h>

Window create_main_window(Display *display);

#endif
