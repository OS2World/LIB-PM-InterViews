/*
 * Copyright (c) 1987, 1988, 1989 Stanford University
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Stanford not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Stanford makes no representations about
 * the suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * STANFORD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
 * IN NO EVENT SHALL STANFORD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/****

 Copyright (C) 1990 Texas Instruments Incorporated.

 Permission is granted to any individual or institution to use, copy, modify,
 and distribute this software, provided that this complete copyright and
 permission notice is maintained, intact, in all copies and supporting
 documentation.

 Texas Instruments Incorporated provides this software "as is" without express
 or implied warranty.

****/

/*
 * PM-dependent canvas code
 */

#include "itable.h"
#include <InterViews/canvas.h>
#include <InterViews/pm/canvasrep.h>
#include <InterViews/color.h>
#include <InterViews/world.h>
#include <InterViews/PM/worldrep.h>
#include <InterViews/PM/PM_lib.h>

Canvas::Canvas (void* c) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Canvas::Canvas");
  }
#endif

  id = c;
  width = 0;
  height = 0;
  status = CanvasUnmapped;
  rep = new CanvasRep;
  rep->windowBackgroundColor = CLR_BACKGROUND;
}

Canvas::Canvas (long w, long h) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Canvas::Canvas");
  }
#endif

  //X11
  //id = (void*)XCreatePixmap(
  //	_world->display(), _world->root(),
  //    w, h, DefaultDepth(_world->display(), _world->screen())
  //);

  /* Warning:  not handling pixmap yet. */
  id = 0;
  width = w;
  height = h;
  status = CanvasOffscreen;
  rep = new CanvasRep;
}

Canvas::~Canvas () {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Canvas::~Canvas");
  }
#endif
  if (id != nil) {
    if (status == CanvasOffscreen) {
      /*
        X11
        XFreePixmap(_world->display(), (Pixmap)id);
       */
#ifdef DEBUG
	debug("*** Canvas::~Canvas ***\n");
#endif
    } else {
      /*
        X11
        XDestroyWindow(_world->display(), (Window)id);
       */
      WinDestroyWindow((HWND)id);
      _world->itable()->Remove(id);
    }
    id = nil;
  }
}

void Canvas::WaitForCopy () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Canvas::WaitForCopy");
  }
#endif

}

void Canvas::SetBackground (Color* c) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not done) Canvas::SetBackground");
  }
#endif

  /*
   * X11
  if (status != CanvasOffscreen) {
    XSetWindowBackground(_world->display(), (Window)id, c->PixelValue());
  }
   */
  rep->windowBackgroundColor = c->PixelValue();
}

//Done
void Canvas::Clip (Coord, Coord, Coord, Coord) {
    /* Canvas clipping unimplemented for X11. */
}

//Done
void Canvas::NoClip () {
    /* Canvas clipping unimplemented for X11. */
}

//Done
void Canvas::ClipOn () {
    /* Canvas clipping unimplemented for X11. */
}

//Done
void Canvas::ClipOff () {
    /* Canvas clipping unimplemented for X11. */
}

//Done
boolean Canvas::IsClipped () {
    return false;
}

//Done
void Canvas::Map (Coord& x, Coord& y) {
    /* nothing to do */
}
