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
 * PM-dependent event code
 */

#include "itable.h"
#include <InterViews/event.h>
#include <InterViews/PM/eventrep.h>
#include <InterViews/PM/worldrep.h>
#include <InterViews/PM/PM_lib.h>
#include <string.h>

Event::Event() {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Event::Event");
  }
#endif

    rep = new EventRep;
}

Event::~Event() {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Event::~Event");
  }
#endif

    delete rep;
}

void Event::GetMotionInfo() {

#ifdef DEBUG
  if (debugP(7)) {
    debug("(Not Done) Event::GetMotionInfo");
  }
#endif

  IVMSG& m = rep->event();
  eventType = MotionEvent;
  timestamp = m.time;

  x = SHORT1FROMMP(m.mp1);
  y = SHORT2FROMMP(m.mp1);
  wx = m.x_root;
  wy = m.y_root;

  GetKeyState((unsigned int)m.mp1);
  FindWorld((unsigned long)_world->root());
}

void Event::GetButtonInfo(EventType t) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Event::GetButtonInfo");
  }
#endif

  IVMSG& b = rep->event();
  eventType = t;
  timestamp = b.time;

  x = SHORT1FROMMP(b.mp1);
  y = SHORT2FROMMP(b.mp1);
  wx = b.x_root;
  wy = b.y_root;
  if (((INT)b.msg == WM_BUTTON1DOWN) || ((INT)b.msg == WM_BUTTON1UP)) {
    button = 0;  //Button 1
    leftmouse = 1;
  } else {
    button = 2;  //Button 2
    rightmouse = 1;
  }
  len = 0;
  GetKeyState((unsigned int)b.mp1);
  FindWorld((unsigned long)_world->root());
}

void Event::GetKeyInfo() {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Event::GetKeyInfo");
  }
#endif
  IVMSG& k = rep->event();

  eventType = KeyEvent;
  timestamp = k.time;

/*
 * Not setting this yet.
 *
  x = k.x;
  y = k.y;
  wx = k.x_root;
  wy = k.y_root;
*/
  x = y = wx = wy = 0;

  USHORT fs = SHORT1FROMMP(k.mp1);
  button = SHORT1FROMMP(k.mp2);
  GetKeyState((unsigned int)k.mp1);

  /* I'm not sure about what window handle to use at this point. */
  FindWorld((unsigned long)_world->root());
}

void Event::GetKeyState(register unsigned state) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Event::GetKeyState");
  }
#endif

    shift = (state & KC_SHIFT) != 0;
    control = (state & KC_CTRL) != 0;
    meta = (state & KC_ALT) != 0;
/*
    shiftlock = (state & LockMask) != 0;
    leftmouse = (state & Button1Mask) != 0;
    middlemouse = (state & Button2Mask) != 0;
    rightmouse = (state & Button3Mask) != 0;
*/
}

boolean Event::GetCrossingInfo(EventType t) {

#ifdef DEBUG
  if (debugP(4)) debug("(Not Done) Event::GetCrossingInfo");
#endif

  IVMSG& c = rep->event();

  /* X11
   *if (c.detail != NotifyInferior) {
   */

  eventType = t;
  timestamp = c.time;
  x = SHORT1FROMMP(c.mp1);
  y = SHORT2FROMMP(c.mp1);
  wx = c.x_root;
  wy = c.y_root;
  GetKeyState((unsigned int)c.mp1);
  FindWorld((unsigned long)_world->root());
  return true;
}

void Event::FindWorld(unsigned long root) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Event::FindWorld");
  }
#endif

    Interactor* i;

    if (_world->itable()->Find(i, (void*)root)) {
        w = (World*)i;
    } else {
	w = nil;
    }
}
