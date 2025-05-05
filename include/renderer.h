#ifndef RENDERER_H
#define RENDERER_H

#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xfixes.h>
#include <X11/Xcms.h>

void init_renderer(Display *display, Window window);
void render_frame();

#endif
