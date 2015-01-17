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
 * PM-dependent worldview code
 */

#include <InterViews/canvas.h>
#include <InterViews/cursor.h>
#include <InterViews/painter.h>
#include <InterViews/shape.h>
#include <InterViews/world.h>
#include <InterViews/worldview.h>
#include <InterViews/PM/worldrep.h>
#include <InterViews/PM/PM_lib.h>

void WorldView::Init (World* world) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) WorldView::Init");
  }
#endif

  WorldRep* rep = world->Rep();
  canvas = new Canvas((void*)HWND_DESKTOP);
 
  if ((canvas->width = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN)) == 0) {
    ReportError(rep->hab(), "(43) ");
  }
  if ((canvas->height = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN)) == 0) {
    ReportError(rep->hab(), "(44) ");
  }
  canvas->status = CanvasMapped;
  xmax = canvas->width - 1;
  ymax = canvas->height - 1;
  output->SetOverwrite(true);
}

/*
static const Mask bmask =
    ButtonPressMask|ButtonReleaseMask|OwnerGrabButtonMask|
    PointerMotionMask|PointerMotionHintMask;
*/

void WorldView::GrabMouse (Cursor*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::GrabMouse");
  }
#endif

}

void WorldView::UngrabMouse () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::UngrabMouse");
  }
#endif

}

boolean WorldView::GrabButton (unsigned, unsigned, Cursor*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::GrabButton");
  }
#endif

  return false;
}

void WorldView::UngrabButton (unsigned, unsigned) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::UngrabButton");
  }
#endif

}

//Done
void WorldView::Lock () {
/*
 * Bad idea to grab the server
 */
}

//Done
void WorldView::Unlock () {
/*
 * See Lock()
 */
}

void WorldView::ClearInput () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::ClearInput");
  }
#endif

}

void WorldView::MoveMouse (Coord, Coord) {

//WinSetPointerPos is something like we want.  What makes this less
//than trivial is when we have to move relative to a window.

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::MoveMouse");
  }
#endif

}

void WorldView::Map (RemoteInteractor i) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) WorldView::Map");
  }
#endif

  //X11
  //XMapWindow(world->Rep()->display(), (Window)i);

  if (WinSetWindowPos((HWND)i, 0, 0, 0, 0, 0, SWP_SHOW) == FALSE) {
    ReportError(world->Rep()->hab(), "(45) ");
  }
}

void WorldView::MapRaised (RemoteInteractor i) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) WorldView::MapRaised");
  }
#endif

  //X11
  //XMapRaised(world->Rep()->display(), (Window)i);

  if (WinSetWindowPos((HWND)i, HWND_TOP, 0, 0, 0, 0, SWP_ZORDER | SWP_SHOW) 
      == FALSE) {
    ReportError(world->Rep()->hab(), "(46) ");
  }
}

void WorldView::Unmap (RemoteInteractor i) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) WorldView::Unmap");
  }
#endif

  //X11
  //XUnmapWindow(world->Rep()->display(), (Window)i);

  if (WinSetWindowPos((HWND)i, 0, 0, 0, 0, 0, SWP_HIDE) 
      == FALSE) {
    ReportError(world->Rep()->hab(), "(46) ");
  }
}

RemoteInteractor WorldView::Find (Coord, Coord) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::Find");
  }
#endif

  return 0;
}

void WorldView::Move (RemoteInteractor i, Coord left, Coord top) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) WorldView::Move");
  }
#endif

  //X11
  //XMoveWindow(world->Rep()->display(), (Window)i, left, ymax - top);

  if (WinSetWindowPos((HWND)i, 0, (SHORT)left, (SHORT)top, 0, 0, SWP_MOVE) 
      == FALSE) {
    ReportError(world->Rep()->hab(), "(47) ");
  }
}

void WorldView::Change (
    RemoteInteractor i, Coord left, Coord top, unsigned long w, unsigned long h
) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) WorldView::Change");
  }
#endif

  //X11
  //XMoveResizeWindow(
  //    world->Rep()->display(), (Window)i, left, ymax - top, w, h
  // );

  if (WinSetWindowPos((HWND)i, 0, (SHORT)left, (SHORT)top, (SHORT)w, (SHORT)h, SWP_MOVE | SWP_SIZE) 
      == FALSE) {
    ReportError(world->Rep()->hab(), "(48) ");
  }
}

void WorldView::Raise (RemoteInteractor i) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) WorldView::Raise");
  }
#endif

  //X11
  //XRaiseWindow(world->Rep()->display(), (Window)i);

  if (WinSetWindowPos((HWND)i, HWND_TOP, 0, 0, 0, 0, SWP_ZORDER)
      == FALSE) {
    ReportError(world->Rep()->hab(), "(49) ");
  }
}    

void WorldView::Lower (RemoteInteractor i) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) WorldView::Lower");
  }
#endif

  //X11
  //XLowerWindow(world->Rep()->display(), (Window)i);

  if (WinSetWindowPos((HWND)i, HWND_BOTTOM, 0, 0, 0, 0, SWP_ZORDER)
      == FALSE) {
    ReportError(world->Rep()->hab(), "(50) ");
  }
}

void WorldView::Focus (RemoteInteractor) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::Focus");
  }
#endif

}

void WorldView::GetList (RemoteInteractor*&, unsigned long&) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::GetList");
  }
#endif

}

void WorldView::GetInfo (
    RemoteInteractor, Coord&, Coord&, Coord&, Coord&
) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::GetInfo");
  }
#endif

}

boolean WorldView::GetHints (
    RemoteInteractor , Coord&, Coord&, Shape&
) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::GetHints");
  }
#endif

  return false;
}

void WorldView::SetHints (RemoteInteractor, Coord, Coord, Shape&) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::SetHints");
  }
#endif

}

RemoteInteractor WorldView::GetIcon (RemoteInteractor) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::GetIcon");
  }
#endif

  return 0;
}

/* obsolete - window mgrs should set WM_STATE on clients' toplevel windows */

void WorldView::AssignIcon (RemoteInteractor, RemoteInteractor) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::AssignIcon");
  }
#endif

}

void WorldView::UnassignIcon (RemoteInteractor) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::UnassignIcon");
  }
#endif

}

RemoteInteractor WorldView::TransientOwner (RemoteInteractor) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::TransientOwner");
  }
#endif

  return 0;
}

char* WorldView::GetName (RemoteInteractor) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("WorldView::GetName");
  }
#endif

  return 0;
}
