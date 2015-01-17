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
 * PM-dependent scene code
 */

#include "itable.h"
#include <InterViews/bitmap.h>
#include <InterViews/canvas.h>
#include <InterViews/pm/canvasrep.h>
#include <InterViews/cursor.h>
#include <InterViews/interactor.h>
#include <InterViews/scene.h>
#include <InterViews/shape.h>
#include <InterViews/world.h>
#include <InterViews/PM/worldrep.h>
#include <InterViews/PM/PM_lib.h>
#include <string.h>

/*
 * Adjust window dimensions to avoid 0 width or height error.
 */

static void DefaultShape (long w, long h, long& nw, long& nh) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) DefaultShape");
  }
#endif

    nw = (w == 0) ? round(2*inch) : w;
    nh = (h == 0) ? round(2*inch) : h;
}

/*
 * Place an interactor according to user preferences.
 */

void Scene::UserPlace (Interactor* i, long w, long h) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Scene::UserPlace");
  }
#endif

    long width, height;
    DefaultShape(w, h, width, height);
    MakeWindow(i, 0, 0, width, height);
    SetWindowProperties(i, 0, 0, width, height, false);
    if (i->GetInteractorType() == IconInteractor) {
	Assign(i, 0, 0, width, height);
    } else {
	DoMap(i, width, height);
    }
}

/*
 * Place an interactor at a particular position.
 */

void Scene::Place (
    Interactor* i, Coord l, Coord b, Coord r, Coord t, boolean map
) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Scene::Place");
  }
#endif
  Coord newtop = ymax - t;
  long width, height;
  DefaultShape(r - l + 1, t - b + 1, width, height);
  MakeWindow(i, l, newtop, width, height);
  SetWindowProperties(i, l, newtop, width, height, true);
  Assign(i, l, b, width, height);
  if (map && i->GetInteractorType() != IconInteractor) {
    Map(i);
  }
}

/*
 * Simulate the X do_not_propagate_mask here.
 */
static USHORT PickWindowAttributes (Interactor* i)
{
#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) PickWindowAttributes");
  }
#endif

  if (i->GetInteractorType() != InteriorInteractor) {
    
    USHORT mask =  (USHORT)(KeyPressMask | KeyReleaseMask | ButtonPressMask | 
	          ButtonReleaseMask | PointerMotionMask );
    return mask;
  } else {
    return 0;
  }
}

/*
 * Pick the right class for the Interactor's window.
 */

static int PickWindowClass (Interactor*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("PickWindowClass");
  }
#endif

  return 0;
}

/*
 * Pick the right set of changes to the geometry of the interactor's window.
 */

static unsigned int PickWindowChanges ()
{

#ifdef DEBUG
  if (debugP(3)) {
    debug("PickWindowChanges");
  }
#endif

  return 0;
}

/*
 * Create a window for an interactor.  If a window already exists,
 * update its geometry, attributes, and event mask.
 */

void Scene::MakeWindow (
    Interactor* i, Coord x, Coord y, long width, long height
) {
    Coord ty;
    SWP swp;
    HWND hwnd, hwndFrame;
    //XSetWindowAttributes a;
    InteractorType t = i->GetInteractorType();
    if (parent == nil) {
	if (t == InteriorInteractor) {
	    /* backward compatibility for world->Insert(interactor) */
	    i->SetInteractorType(ToplevelInteractor);
	}
    } else if (t != InteriorInteractor) {
	/* parent != nil, should be interior now */
	i->SetInteractorType(InteriorInteractor);
    }
    /* 
     * The x and y coordinates passed to this routine are in the fourth
     * quandrant.  PM expects them in the first quadrant.  The coordinates
     * are translated to the first quadrant by getting the coordinates
     * of the parent window and doing the calculations shown in the
     * code.
     */
    if (WinQueryWindowPos((HWND)canvas->id, &swp) == FALSE) {
      ReportError(_world->hab(), "(77) ");
    }
    ty = swp.cy - y - height;

    if (i->canvas == nil) {

        //X11
	//unsigned int amask = PickWindowAttributes(i, a);
	//Window w = XCreateWindow(
	//    _world->display(), (Window)canvas->id, x, y, width, height, 0, 
	//    /* CopyFromParent */ 0, PickWindowClass(i),
	//    /* cast below is workaround for cfront 2.0 bug */
	//    (Visual*)CopyFromParent, amask, &a
	//);
	//i->canvas = new Canvas((void*)w);
	//_world->itable()->Insert((void*)w, i);

	int id = _world->_windowID++;
	USHORT amask = PickWindowAttributes(i);
	InteractorType itype = i->GetInteractorType();
	if (itype == InteriorInteractor || itype == PopupInteractor) {
	  /*
           * Create a child window
	   */
	  hwnd = WinCreateWindow(
	     (HWND)canvas->id,               // Parent window handle
             (PSZ)_world->_szClientClass,  // Client window class name
             (PSZ)NULL,                      // Window text
	     0L,                             // Window Style
             (SHORT)x,                       // Window position
             (SHORT)ty,
             (SHORT)width,                   // Window size
             (SHORT)height,
             (HWND)canvas->id,               // Owner window handle
             HWND_TOP,                       // Placement window handle        
	     (USHORT)id,                     // Window identifier
	     (PVOID)NULL,                           // Control data
	     (PVOID)NULL);                          // Presentation parameters
	   if (hwnd == (HWND)NULL) {
 	    ReportError(_world->hab(), "(19) ");
	   }
	  if (_world->_hwndClient == (HWND)NULL) {
	    _world->_hwndClient = hwnd;
	  }
	} else {
	  /* 
           * Create a top-level window 
	   */
	  hwndFrame = WinCreateStdWindow(
	    (HWND)_world->_root,            // Parent window handle
            0L,                             // Style of frame window
            &(_world->_flFrameFlags),       // Pointer to control data
            (PSZ)_world->_szClientClass,  // Client window class name
            (PSZ)NULL,                           // Title bar text
            0L,                             // Style of client window
            (HMODULE) NULL,                 // Module handle for resources
            1,                              // ID of resources
            &hwnd);                         // Pointer to client window handle

	  if (hwndFrame == (HWND)NULL) {
	    ReportError(_world->hab(), "(18) ");
	  }
	  if (_world->_hwndFrame == (HWND)NULL) {
	    _world->_hwndFrame = hwndFrame;
	    _world->_hwndClient = hwnd;
	  }

	  /*
	   * The input width and height passed to this routine refers to 
           * the size of the window without the window manager wrappings
           * (i.e. frame window).  This is an attempt to set the frame
	   * window size to a size that will make the client window size
	   * equal to the input width and height.
           */
	  long sv_cxborder = WinQuerySysValue(HWND_DESKTOP, SV_CXBORDER);
	  long sv_cyborder = WinQuerySysValue(HWND_DESKTOP, SV_CYBORDER);
	  long sv_cxsizeborder = WinQuerySysValue(HWND_DESKTOP, 
						  SV_CXSIZEBORDER);
	  long sv_cysizeborder = WinQuerySysValue(HWND_DESKTOP, 
						  SV_CYSIZEBORDER);
	  long sv_cytitlebar = WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);

	  WinSetWindowPos
	    (hwndFrame, HWND_TOP, 0, 0, 
	     (SHORT)(width + 2*sv_cxborder + sv_cxsizeborder + 2), 
	     (SHORT)(height + 2*sv_cyborder + sv_cytitlebar + sv_cysizeborder + 2),
	     SWP_SIZE | SWP_SHOW | SWP_ZORDER);
	
	  /* Set default icon */
	  WinSendMsg(hwndFrame, WM_SETICON,
		     WinQuerySysPointer(HWND_DESKTOP, SPTR_APPICON, FALSE),
		     (MPARAM)NULL);
	}
        /*
         * WinCreateStdWindow does not seem to allow us to assign a 
         * unique window id to its window handle.  To get around this,
         * we are storing the window id in the application area of the 
         * window handle.  Also, to be consistent, we are storing this
         * value for windows created using WinCreateWindow (even though
         * this value is already stored in the window handle).
         */
	WinSetWindowUShort(hwnd, IV_ID, id);

        /*
         * Store the do_not_propagate mask into the application area of
         * the window handle.
         */
	WinSetWindowUShort(hwnd, IV_DO_NOT_PROPAGATE_MASK, amask);

	/*
         * Just to be safe store the event mask.
         */
	WinSetWindowUShort(hwnd, IV_EVENT_MASK, 0);

	/*
	 * If this window is a client window of the frame window, then
	 * store the frame window handle into the application area of
	 * the window handle, otherwise store the value zero.
	 */
	if (itype != InteriorInteractor && itype != PopupInteractor) {
	  WinSetWindowULong(hwnd, IV_HWND_FRAME, (ULONG)hwndFrame);
	} else {
	  WinSetWindowULong(hwnd, IV_HWND_FRAME, 0L);
	}

	/*
	 * It's not done now, but the interactor could be stored in 
	 * the application area of the window handle.  This would
	 * probably improve the performance of retrieving interactors
	 * from window id's.
         */
	i->canvas = new Canvas((void*)hwnd);
	_world->itable()->Insert((void*)hwnd, i);

#ifdef DEBUG
	if (debugP(10)) {
	  char buf[80];
	  DoPrintMsg("Scene::MakeWindow", hwnd, 1002);
	  sprintf(buf, "Requested window size:  x = %ld, y = %ld, width = %ld, height = %ld", 
		  x, ty, width, height);
	  debug(buf);
	  SWP debugSwp;
	  WinQueryWindowPos(hwnd, (PSWP)&debugSwp);
	  sprintf(buf, "Actual window size:  x = %d, y = %d, width = %d, height = %d", debugSwp.x, debugSwp.y, debugSwp.cx, debugSwp.cy); 
	  debug(buf);
	  DoPrintMsg("Parent window", (HWND)canvas->id, 1002);
	  sprintf(buf, "Parent window size:  x = %d, y = %d, width = %d, height = %d", swp.x, swp.y, swp.cx, swp.cy); 
	  debug(buf);
	}
#endif

    } else {

        //X11
  	//XWindowChanges c;
	//unsigned int cmask = PickWindowChanges(
	//    i->canvas, x, y, width, height, c
	//);
	//XConfigureWindow(_world->display(), (Window)i->canvas->id, cmask, &c); 
        if (WinSetWindowPos((HWND)i->canvas->id,
			    HWND_BOTTOM,         // Relative window placement
			    (SHORT)x,
			    (SHORT)ty,
			    (SHORT)width,
			    (SHORT)height,
			    SWP_SIZE |           // Window positioning options
			    SWP_MOVE |
			    SWP_ZORDER)
	    == FALSE) {
	  ReportError(_world->hab(), "(20) ");
	}

#ifdef DEBUG
	if (debugP(10)) {
	  char buf[80];
	  HWND hwndParent;
	  DoPrintMsg("Scene::MakeWindow (position)", hwnd, 1002);
	  sprintf(buf, "Requested window size:  x = %ld, y = %ld, width = %ld, height = %ld", 
		  x, ty, width, height);
	  debug(buf);
	  SWP debugSwp;
	  WinQueryWindowPos((HWND)i->canvas->id, (PSWP)&debugSwp);
	  sprintf(buf, "Actual window size:  x = %d, y = %d, width = %d, height = %d", debugSwp.x, debugSwp.y, debugSwp.cx, debugSwp.cy); 
	  debug(buf);
	  hwndParent = WinQueryWindow((HWND)i->canvas->id, QW_PARENT, FALSE);
	  DoPrintMsg("Parent Window", hwndParent, 1002);
	  WinQueryWindowPos(hwndParent, (PSWP)&debugSwp);
	  sprintf(buf, "Parent window size:  x = %d, y = %d, width = %d, height = %d", debugSwp.x, debugSwp.y, debugSwp.cx, debugSwp.cy); 
	  debug(buf);
	}
#endif

    }
    i->Listen(i->cursensor == nil ? i->input : i->cursensor);
}

/*
 * Set toplevel windows' properties for ICCCM compatibility.
 */

void Scene::SetWindowProperties (
    Interactor* , Coord, Coord, long, long, boolean
) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Scene::SetWindowProperties");
  }
#endif

}

/*
 * Map an interactor and wait for confirmation of its position.
 * The interactor is either being placed by the user or inserted
 * in the world (in which case a window manager might choose to allocate
 * it a different size and placement).
 */

//struct EventInfo {
//    Window w;
//    Interactor* i;
//};

static boolean MapOrRedraw ()
{
#ifdef DEBUG
  if (debugP(3)) {
    debug("MapOrRedraw");
  }
#endif


  return false;
}

void Scene::DoMap (Interactor* i, long, long) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Scene::DoMap");
  }
#endif

  SWP swp;

  if (WinSetWindowPos((HWND)i->canvas->id, HWND_TOP, 0,0,0,0,
		      SWP_ZORDER | SWP_SHOW)
      == FALSE) {
    ReportError(_world->hab(), "(21) ");
  }
  if (WinQueryWindowPos((HWND)i->canvas->id, &swp) == FALSE) {
    ReportError(_world->hab(), "(80) ");
  }
  i->SendResize(0, 0, swp.cx, swp.cy);
//  Assign(i, 0, 0, swp.cx, swp.cy);

  i->canvas->status = CanvasMapped;
}

/*
 * Warning:  I'm not sure if I handled XMapRaised and XMapWindow correctly.
 */
void Scene::Map (Interactor* i, boolean raised) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Scene::Map");
  }
#endif

  HWND w = (HWND)i->canvas->id;
  if (raised) {

    //X11
    //XMapRaised(d, w);

    if (WinSetWindowPos(w, HWND_TOP, 0, 0, 0, 0, SWP_ZORDER | SWP_SHOW)
	== FALSE) {
      ReportError(_world->hab(), "(67) ");
    }
  } else {

    //X11
    //XMapWindow(d, w);

    if (WinSetWindowPos(w, HWND_TOP, 0, 0, 0, 0, SWP_SHOW)
	== FALSE) {
      ReportError(_world->hab(), "(68) ");
    }
  }
  i->canvas->status = CanvasMapped;
}

/*
 * We must send a synthetic UnMapNotify for ICCCM compatibility
 */

void Scene::Unmap (Interactor* i) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Scene::Unmap");
  }
#endif

  WorldRep* rep = GetWorld()->Rep();
  HWND w = (HWND)i->canvas->id;
  HWND hwndFrame;

#ifdef DEBUG
      if (debugP(8)) {
	DoPrintMsg("Scene::Unmap ", w, (int)1002);
      }
#endif

  /*
   *X11
    XUnmapWindow(rep->display(), w);
   */
  hwndFrame = (HWND)WinQueryWindowULong((HWND)w, IV_HWND_FRAME);

  /*
   * Hiding the client window does not hide the frame window.
   */
  if (hwndFrame != (HWND)0) {
    if (WinSetWindowPos(hwndFrame, 0, 0, 0, 0, 0, SWP_HIDE) 
	== FALSE) {
      ReportError(_world->hab(), "(73) ");
    }
  } else {
    if (WinSetWindowPos((HWND)w, 0, 0, 0, 0, 0, SWP_HIDE) 
	== FALSE) {
      ReportError(_world->hab(), "(97) ");
    }
  }
  i->canvas->status = CanvasUnmapped;
/*
 *X11
  if (parent == nil && i->GetInteractorType() != PopupInteractor) {
    Bool propagate = False;
    Mask eventmask = (SubstructureRedirectMask|SubstructureNotifyMask);
    XEvent xe;

    xe.type = UnmapNotify;
    xe.xunmap.type = UnmapNotify;
    xe.xunmap.display = rep->display();
    xe.xunmap.event = rep->root();
    xe.xunmap.window = w;
    xe.xunmap.from_configure = False;

    XSendEvent(rep->display(), rep->root(), propagate, eventmask, &xe);
  }
 */
}

void Scene::Raise (Interactor*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Scene::Raise");
  }
#endif

}

void Scene::Lower (Interactor*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Scene::Lower");
  }
#endif

}

void Scene::Move (Interactor*, Coord, Coord, Alignment) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Scene::Move");
  }
#endif

}
