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
 * PM-dependent sensor code
 */

#include "itable.h"
#include <InterViews/sensor.h>
#include <InterViews/PM/PM_lib.h>
#include <InterViews/PM/eventrep.h>
#include <InterViews/PM/worldrep.h>

Mask motionmask = PointerMotionMask;
Mask keymask = KeyPressMask;
Mask entermask = EnterWindowMask;
Mask leavemask = LeaveWindowMask;
Mask focusmask = FocusChangeMask;
Mask substructmask = SubstructureRedirectMask;
Mask upmask = ButtonPressMask|ButtonReleaseMask|OwnerGrabButtonMask;
Mask downmask = ButtonPressMask|ButtonReleaseMask|OwnerGrabButtonMask;
Mask initmask = PointerMotionHintMask;

static Sensor* grabber;

boolean Sensor::Interesting(Event& e) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Sensor::Interesting");
  }
#endif

  register IVMSG& x = e.Rep()->event();
  switch (x.msg) {

  /* Same as MotionNotify in X */
  case WM_MOUSEMOVE:
    e.GetMotionInfo();
    return true;

  /* Same as KeyPress in X */
  case WM_CHAR:
    USHORT fs = SHORT1FROMMP(x.mp1);
    if (!(fs & KC_KEYUP)) {
      e.GetKeyInfo();
      return ButtonIsSet(down, e.button);
    }

  /* Same as ButtonPress in X */
  case WM_BUTTON1DOWN:
  case WM_BUTTON2DOWN:
    e.GetButtonInfo(DownEvent);
    boolean b = ButtonIsSet(down, e.button);
    if (b && ButtonIsSet(up, e.button)) {
      grabber = this;
    } else {
      grabber = nil;
    }
    return b;

  /* Same as ButtonRelease in X */
  case WM_BUTTON1UP:
  case WM_BUTTON2UP:
    e.GetButtonInfo(UpEvent);
    return ButtonIsSet(up, e.button) || (grabber != nil);
    
  /* Same as EnterNotify in X */
  case WM_ENTERNOTIFY:
    return e.GetCrossingInfo(EnterEvent);

  /* Same as LeaveNotify in X */
  case WM_LEAVENOTIFY:
    return e.GetCrossingInfo(LeaveEvent);

  default:
    /* ignore */;
  }
  return false;
}
