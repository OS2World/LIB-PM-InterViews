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
 * Support for window managers.
 */

#include <InterViews/sensor.h>
#include <InterViews/shape.h>
#include <InterViews/world.h>
#include <InterViews/worldview.h>

WorldView* _worldview;

WorldView::WorldView (World* w) {
    world = w;
    curfocus = nil;
    Init(w);
    if (input == nil) {
	input = new Sensor;
	input->Catch(DownEvent);
	input->Catch(UpEvent);
	input->CatchRemote();
    }
    w->Listen(input);
    _worldview = this;
}

WorldView::~WorldView () {
    /* nothing to do currently */
}

void WorldView::InsertRemote (RemoteInteractor i) {
    Map(i);
}

void WorldView::ChangeRemote (
    RemoteInteractor i, Coord left, Coord top, unsigned long w, unsigned long h
) {
    Change(i, left, top, w, h);
}

RemoteInteractor WorldView::Choose (Cursor* c, boolean waitforup) {
    Event e;

    GrabMouse(c);
    do {
	Read(e);
    } while (e.eventType != DownEvent);
    if (waitforup) {
	do {
	    Read(e);
	} while (e.eventType != UpEvent);
    }
    UngrabMouse();
    return Find(e.x, e.y);
}

void WorldView::FreeList (RemoteInteractor* i) {
    delete i;
}

class Blanket : public Interactor {
public:
    Blanket(long x, long y);
private:
    long _right;
    long _top;

    void Reconfig();
};

Blanket::Blanket (long x, long y) {
    _right = x;
    _top = y;
}

void Blanket::Reconfig () {
    shape->Rect(_right + 1, _top + 1);
}

void WorldView::RedrawAll () {
    Blanket* b = new Blanket(xmax, ymax);
    world->Insert(b, 0, 0);
    world->Remove(b);
    delete b;
    Sync();
}
