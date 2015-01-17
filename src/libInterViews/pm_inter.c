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
 * PM-dependent code
 */

#include "itable.h"
#include <InterViews/bitmap.h>
#include <InterViews/canvas.h>
#include <InterViews/cursor.h>
#include <InterViews/interactor.h>
#include <InterViews/sensor.h>
#include <InterViews/shape.h>
#include <InterViews/world.h>
#include <InterViews/worldview.h>
#include <InterViews/PM/eventrep.h>
#include <InterViews/PM/worldrep.h>
#include <InterViews/PM/PM_lib.h>
#include <InterViews/PM/canvasrep.h>
#include <process.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#define ID_TIMER 1

#if defined (__cplusplus)
extern "C" {
void EnterLeave (HWND hwnd, MPARAM mp1, MPARAM mp2);
boolean eventPropagation (PIVMSG ivmsg);
MRESULT EXPENTRY PMGetEvent( HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2 );
}
#endif

extern Mask motionmask, keymask, entermask, leavemask, focusmask, upmask, 
  downmask, initmask;

/*
 * The X version of this routine selected the event types to be sent 
 * to this window.  
 */
void Interactor::Listen (Sensor* s) {

#ifdef DEBUG
  if (debugP(4)) debug("(Not Done) Interactor::Listen");
#endif

  Mask m;

  cursensor = s;
  if (canvas == nil) {
    /* can't set input interest without window */
    return;
  }
  m = (s == nil) ? 0 : s->mask;

  /*
    X11
    if (parent != nil) {
      m |= ExposureMask;
      if (parent->parent == nil) {
        m |= StructureNotifyMask;
      }
    }
    XSelectInput(_world->display(), (Window)canvas->id, m);
   */

  if (canvas->id != (HWND)NULL) {
    WinSetWindowUShort((HWND)canvas->id, IV_EVENT_MASK, (unsigned)m);
  } else {
#ifdef DEBUG
    debug("*** Can't find canvas->id in Interactor::Listen ***\n");
#endif
  }
}

int Interactor::Fileno () {

#ifdef DEBUG
  if (debugP(4)) debug("(Not Done) Interactor::Fileno");
#endif

    return 0;
}

/*
 * Read a single event from the event stream.  If it is an input event,
 * is associated with an interactor, and the interactor is interested in it,
 * then return true.  In all other cases, return false.
 */

boolean Interactor::GetEvent (Event& e, boolean remove) 
{
  IVMSG& ivmsg = e.Rep()->event();
  Interactor* i;
  USHORT id;
  static timerStarted = FALSE;

  e.eventType = MotionEvent;
  /*
   * Timer events are turned on and off here instead of Interactor::Select.
   * This will generate WM_TIMER messages with an id value of ID_TIMER.  All
   * other WM_TIMER messages will be ignored.   
   */
  if (timerStarted) {
    if (!(cursensor != nil && cursensor->timer)) {
      if (WinStopTimer(_world->hab(), _world->hwndClient(), ID_TIMER)
	  == FALSE) {
	ReportError(_world->hab(), "(41) ");
      }
      timerStarted = FALSE;
    }
  } else {
    if (cursensor != nil && cursensor->timer) {
      ULONG timeout;
      Sensor* s;
    
      s = cursensor;
      timeout = s->sec * 1000;
      timeout = timeout + (s->usec / 100);
      if (WinStartTimer(_world->hab(), _world->hwndClient(),ID_TIMER, (USHORT)timeout) 
	  == 0) {
	ReportError(_world->hab(), "(42) ");
      }
      timerStarted = TRUE;
    }
  }
      
  _world->NextMessage(&ivmsg);

#ifdef DEBUG
  if (debugP(1)) DoPrintMsg("GetEvent", ivmsg.hwnd, ivmsg.msg);
#endif

  id = WinQueryWindowUShort(ivmsg.hwnd, IV_ID);
 
  /* Propagate events */
  if (!eventPropagation(&ivmsg)) {
    return false;
  }

  switch (ivmsg.msg) {

  /*
   * Same as MapNotify/UnmapNotify in X.
   */
  case WM_SHOW:
    /* 
     * If window is visible.
     */
    if (ivmsg.mp1) {
      if (_world->itable()->Find(i, (void*)ivmsg.hwnd)) {
	i->SendActivate();

	/*
	 * If the pointer is in this window then send an ENTERNOTIFY event.
         */
	POINTL ptl;
	HWND showHWND;
	if (WinQueryPointerPos(_world->root(), &ptl) == FALSE) {
	  ReportError(_world->hab(), "(74) ");
	}
	if ((showHWND = WinWindowFromPoint(_world->root(), &ptl, TRUE, FALSE))
	    == (HWND)NULL) {
	  ReportError(_world->hab(), "(75) ");
	}
	if (showHWND == ivmsg.hwnd) {
	  EnterLeave(ivmsg.hwnd, ivmsg.mp1, ivmsg.mp2);
	}
      }
    /* 
     * Window is not visible.
     */
    } else { 
      if (_world->itable()->Find(i, (void*)ivmsg.hwnd)) {
	i->SendDeactivate();
      }
    }
    return false;

  /*
   * Same as Expose in X.
   */
  case WM_PAINT:
    if (_world->itable()->Find(i, (void*)ivmsg.hwnd)) {

      /*
       * PM does not automatically fill the background color of a window
       * when a window is shown.  In order to insure that the entire window
       * always has a background color, every window paint message will
       * fill the window with the background color before any redrawing
       * is done.  The "squares" demo program demonstrated this problem
       * when bringing up the "view setup" window.
       */
      HPS hps;
      
      hps = WinGetPS(ivmsg.hwnd);
      WinFillRect(hps, &ivmsg.rcl, canvas->rep->windowBackgroundColor);
      WinReleasePS(hps);

      i->SendRedraw(SHORT1FROMMP(ivmsg.mp1), SHORT2FROMMP(ivmsg.mp1), 
		    SHORT1FROMMP(ivmsg.mp2), SHORT2FROMMP(ivmsg.mp2), 
		    0
		    );
#ifdef DEBUG
          if (debugP(9)) {
            char buf[80];
            DoPrintMsg("WM_PAINT id = ", (HWND)ivmsg.hwnd, 1002);
            sprintf(buf, "WM_PAINT:  left = %d, bottom = %d, right = %d, top = %d\n",
		    SHORT1FROMMP(ivmsg.mp1), SHORT2FROMMP(ivmsg.mp1), 
		    SHORT1FROMMP(ivmsg.mp1) + SHORT1FROMMP(ivmsg.mp2), 
		    SHORT2FROMMP(ivmsg.mp1) + SHORT2FROMMP(ivmsg.mp2));
            debug(buf);
          }
#endif

    }
    return false;

  /* Same as ConfigureNotify in X. */
  case WM_SIZE:
    if (_world->itable()->Find(i, (void*)ivmsg.hwnd)) {
      i->SendResize(0, 0, SHORT1FROMMP(ivmsg.mp2), SHORT2FROMMP(ivmsg.mp2));
    }
    return false;

  /* Same as MotionNotify in X */
  case WM_MOUSEMOVE:
    EnterLeave(ivmsg.hwnd, ivmsg.mp1, ivmsg.mp2);
    break;

  /*
   * Same as KeyPress in X.
   */
  case WM_CHAR:
    break;

  /* Same as ButtonPress/ButtonRelease in X */
  case WM_BUTTON1DOWN:
  case WM_BUTTON2DOWN:
  case WM_BUTTON1UP:
  case WM_BUTTON2UP:
    break;

  /* Same as EnterNotify and LeaveNotify in X */
  case WM_ENTERNOTIFY:
  case WM_LEAVENOTIFY:
    break;

  case WM_TIMER:
    switch (SHORT1FROMMP(ivmsg.mp1)) {
    case ID_TIMER:
      e.target = this;
      e.eventType = TimerEvent;
      break;
    default:
      return false;
    }
    break;

  case WM_CLOSE:
    WinDestroyWindow( _world->hwndFrame() );
    WinDestroyMsgQueue( _world->hmq() );    
    WinTerminate( _world->hab() );          
    exit(1);

  default:
    /* ignore */
    return false;
  }

  /* only input events should get here */
  if (e.eventType != TimerEvent) {
    if (!_world->itable()->Find(i, (void*)ivmsg.hwnd)) {
      return false;
    }
    if (i->cursensor == nil) {
      return false;
    }
    if (!i->cursensor->Interesting(e)) {
      return false;
    }
    e.target = i;
    e.y = i->ymax - e.y + 1;
  }
  
  if (!remove) {
    _world->putBackMessage(&ivmsg);
  }
  return true;
}

/*
 * Check to see if any input events of interest are pending.
 * This routine will return true even if the event is for another interactor.
 */

boolean Interactor::Check () {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::Check");
#endif

    return false;
}

long Interactor::CheckQueue () {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::CheckQueue");
#endif

    return 0;
}

void Interactor::SendRedraw (Coord x, Coord y, long w, long h, long) {

#ifdef DEBUG
  if (debugP(4)) debug("(Not Done) Interactor::SendRedraw");
#endif

  Coord top = ymax - y;
  Redraw(x, top - h + 1, x + w - 1, top);
}

void Interactor::SendResize (Coord x, Coord y, long w, long h) {

#ifdef DEBUG
  if (debugP(4)) debug("(Not Done) Interactor::SendResize");
#endif

  left = x;
  bottom = parent->ymax - y - h + 1;
  if (canvas->width != w || canvas->height != h) {
    canvas->width = w;
    canvas->height = h;
    xmax = w - 1;
    ymax = h - 1;
    Resize();
  }
}

void Interactor::SendActivate () {

#ifdef DEBUG
  if (debugP(5)) debug("(Done) Interactor::SendActivate");
#endif

    canvas->status = CanvasMapped;
    Activate();
}

void Interactor::SendDeactivate () {

#ifdef DEBUG
  if (debugP(5)) debug("(Done) Interactor::SendDeactivate");
#endif

    canvas->status = CanvasUnmapped;
    Deactivate();
}

void Interactor::Poll (Event& e) {

#ifdef DEBUG
  if (debugP(4)) debug("(Not Done) Interactor::Poll");
#endif

  POINTL ptl, ptlw;
  Interactor* i;

  //X11
  //XQueryPointer(
  //	_world->display(), (Window)canvas->id, &root, &child,
  //	&root_x, &root_y, &x, &y, &state
  //);

  if (WinQueryPointerPos(_world->root(), &ptl) == FALSE) {
    ReportError(_world->hab(), "(63) ");
  }
  if (WinMapWindowPoints(_world->root(), (HWND)canvas->id, &ptlw, 1) 
      == FALSE) {
    ReportError(_world->hab(), "(82) ");
  }
    
  if (_world->itable()->Find(i, (void*)_world->root())) {
    e.w = (World*)i;
  } else {
    e.w = nil;
  }
  e.x = ptlw.x;
  e.y = ptlw.y;
  e.wx = ptl.x;
  e.wy = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN) - ptl.y;

  /* Warning:  Don't have these fields. */
  e.control = false;
  e.meta = false;
  e.shift = false;
  e.shiftlock = false;
  e.leftmouse = false;
  e.middlemouse = false;
  e.rightmouse = false;
}

void Interactor::Flush () {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::Flush");
#endif

}

void Interactor::Sync () {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::Sync");
#endif

}

void Interactor::GetRelative (Coord&, Coord&, Interactor*) {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::GetRelative");
#endif

}

void Interactor::DoSetCursor (Cursor*) {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::DoSetCursor");
#endif

}

void Interactor::DoSetName (const char*) {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::DoSetName");
#endif

}

void Interactor::DoSetGroupLeader (Interactor*) {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::DoSetGroupLeader");
#endif

}

void Interactor::DoSetTransientFor (Interactor*) {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::DoSetTransientFor");
#endif

}

void Interactor::DoSetIconName (const char*) {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::DoSetIconName");
#endif

}

void Interactor::DoSetIconBitmap (Bitmap*) {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::DoSetIconBitmap");
#endif

}

void Interactor::DoSetIconMask (Bitmap*) {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::DoSetIconMask");
#endif

}

void Interactor::DoSetIconInteractor (Interactor*) {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::DoSetIconInteractor");
#endif

}

void Interactor::DoSetIconGeometry (const char*) {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::DoSetIconGeometry");
#endif

}

void Interactor::Iconify () {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::Iconify");
#endif

}

void Interactor::DeIconify () {

#ifdef DEBUG
  if (debugP(3)) debug("Interactor::DeIconify");
#endif

}

/* New functions */

/*
 * This function is called by PM's WinDispatchMsg routine and also 
 * directly by PM.  All messages are added to the IV queue.
 */

#ifdef __cplusplus
extern "C" {
#endif
MRESULT EXPENTRY PMGetEvent( HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2 )
{

#ifdef DEBUG
  if (debugP(2)) DoPrintMsg("PMGetEvent:", hwnd, msg);
#endif

  POINTL ptl;
  boolean messageOK;
  MPARAM tmp1, tmp2;
  SWP swp;
  RECTL rcl;
  HPS hps;

  /* 
   * Messages that aren't in this switch statement will be thrown away. 
   */
  messageOK = TRUE;
  tmp1 = mp1; tmp2 = mp2;
  rcl.xLeft = rcl.yBottom = rcl.xRight = rcl.yTop = 0;
  switch (msg) {
  case WM_MOUSEMOVE:
  case WM_BUTTON1DOWN:
  case WM_BUTTON2DOWN:
  case WM_BUTTON1UP:
  case WM_BUTTON2UP:
    /* Translate the mouse position to the fourth quadrant. */
    if (WinQueryWindowPos(hwnd, &swp) == FALSE) {
      ReportError(_world->hab(), "(78) ");
    }
    tmp1 = MPFROM2SHORT(SHORT1FROMMP(mp1), swp.cy - SHORT2FROMMP(mp1));
    break;
  case WM_PAINT:
    /* 
     * Determine the damaged paint region and translate this region to 
     * the fourth quadrant.
     */
    if (WinQueryWindowPos(hwnd, &swp) == FALSE) {
      ReportError(_world->hab(), "(79) ");
    }
    hps = WinBeginPaint(hwnd, (HPS)NULL, (PRECTL)&rcl);
    WinEndPaint(hps);
    tmp1 = MPFROM2SHORT(rcl.xLeft, swp.cy - rcl.yTop);
    tmp2 = MPFROM2SHORT(rcl.xRight - rcl.xLeft, rcl.yTop - rcl.yBottom);
    break;
  case WM_SHOW:
  case WM_SIZE:
  case WM_CHAR:
  case WM_ENTERNOTIFY:
  case WM_LEAVENOTIFY:
  case WM_TIMER:
  case WM_CLOSE:
    break;
  default:
    messageOK = FALSE;
  }

  if (messageOK) {
    /* Return the pointer position in screen coordinates */
    if (WinQueryMsgPos(_world->hab(), &ptl) == FALSE) {
      ReportError(_world->hab(), "(62) ");
    }
    /* and map it from quadrant 1 to quadrant 4. */
    ptl.y = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN) - ptl.y;

    /* Add the PM message to the IV message queue. */
    _world->addMessage(hwnd, msg, tmp1, tmp2, ptl, rcl);
  }
  return (WinDefWindowProc( hwnd, msg, mp1, mp2));
}
#ifdef __cplusplus
}   /* end of extern c */
#endif

/* 
 * Here is where we try to simulate the X EnterNotify/LeaveNotify events.
 * To do this we have to determine whether the mouse has moved into (out
 * of) a window.  If so, then we will post a leave event for the previous 
 * window and an enter event for the current window.
 */
void EnterLeave (HWND hwnd, MPARAM mp1, MPARAM mp2)
{
  int id;

  if (_world->_savedID == -1) {
    if (WinPostMsg(hwnd, WM_ENTERNOTIFY, mp1, mp2) 
	== FALSE) {
      ReportError(_world->hab(), "(69) ");
    }
    _world->_savedID = WinQueryWindowUShort(hwnd, IV_ID);
    _world->_savedHWND = hwnd;
    _world->_savedMP1 = mp1;
    _world->_savedMP2 = mp2;
  } else {
    id = WinQueryWindowUShort(hwnd, IV_ID);

    if (id != _world->_savedID) {

      /*
       * I'm not going to check for errors on this posting because the
       * window may not exist anymore.
       */
      WinPostMsg(_world->_savedHWND, WM_LEAVENOTIFY, _world->_savedMP1,
		 _world->_savedMP2);
      
      if (WinPostMsg(hwnd, WM_ENTERNOTIFY, mp1, mp2)
	  == FALSE) {
	ReportError(_world->hab(), "(71) ");
      }
      _world->_savedID = id;
      _world->_savedHWND = hwnd;
      _world->_savedMP1 = mp1;
      _world->_savedMP2 = mp2;
    }
  }
}
  
/*
 * Simulate X event propagation.
 */
boolean eventPropagation (PIVMSG ivmsg)
{
  int mask = WinQueryWindowUShort(ivmsg->hwnd, IV_EVENT_MASK);
  int amask = WinQueryWindowUShort(ivmsg->hwnd, IV_DO_NOT_PROPAGATE_MASK);

  switch (ivmsg->msg) {

  /*
   * I'm not going to do propagation of motion movement events.
   */
  case WM_MOUSEMOVE:
    return true;

  /*
   * I'm not going to handle event propagation of character events
   * because I think PM already does this.  One day I will find out for sure.
   */
  case WM_CHAR:
    /*
    if (SHORT1FROMMP(ivmsg->mp1) & KC_KEYUP) {
      return false;
    }
    if (mask & keymask) {
      return true;
    }
    if (amask & keymask) {
      return false;
    }
    break;
    */
   
    return true;

  /*
   * I'm not going to handle event propagation of button events because 
   * PM already does this.
   */
  case WM_BUTTON1DOWN:
  case WM_BUTTON2DOWN:
  case WM_BUTTON1UP:
  case WM_BUTTON2UP:
    return true;

  case WM_ENTERNOTIFY:
    if (mask & entermask) {
      return true;
    }
    if (amask & entermask) {
      return false;
    }
    break;

  case WM_LEAVENOTIFY:
    if (mask & leavemask) {
      return true;
    }
    if (amask & leavemask) {
      return false;
    }
    break;

  default:
    return true;
  }
    
  HWND hwnd;
  hwnd = WinQueryWindow((HWND)ivmsg->hwnd, QW_PARENT, FALSE);
  if (hwnd == (HWND)NULL || hwnd == HWND_DESKTOP) {
    return false;
  }
  ivmsg->hwnd = hwnd;
  if (eventPropagation(ivmsg)) {
    return true;
  } else {
    return false;
  }
}
