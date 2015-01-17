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
 * General scrolling interface.
 */

#ifndef scroller_h
#define scroller_h

#include <InterViews/interactor.h>

class Scroller : public Interactor {
protected:
    Interactor* interactor;
    long size;
    Perspective* view;
    Perspective* shown;
    double scale;
    Sensor* tracking;
    boolean syncScroll;

    Scroller(Interactor*, long);
    Scroller(const char*, Interactor*, long);
    Scroller(Interactor*, long, Painter* out);
    ~Scroller();

    void Background(Coord, Coord, Coord, Coord);
    void MakeBackground();
    virtual void Resize();
private:
    void Init();
};

class HScroller : public Scroller {
public:
    HScroller(Interactor*, long size = 0L);
    HScroller(const char*, Interactor*, long size = 0L);
    HScroller(Interactor*, long size, Sensor* in, Painter* out);

    virtual void Handle(Event&);
    virtual void Update();
private:
    void Bar(Coord, long);
    void Border(Coord);
    void GetBarInfo(Perspective*, Coord&, long&);
    void Init();
    void Outline(Coord, long);
    virtual void Reconfig();
    virtual void Redraw(Coord, Coord, Coord, Coord);
    void Sides(Coord, Coord);
    Coord Slide(Event&);
};

class VScroller : public Scroller {
public:
    VScroller(Interactor*, long size = 0L);
    VScroller(const char*, Interactor*, long size = 0L);
    VScroller(Interactor*, long size, Sensor* in, Painter* out);

    virtual void Handle(Event&);
    virtual void Update();
private:
    void Bar(Coord, long);
    void Border(Coord);
    void GetBarInfo(Perspective*, Coord&, long&);
    void Init();
    void Outline(Coord, long);
    virtual void Reconfig();
    virtual void Redraw(Coord, Coord, Coord, Coord);
    void Sides(Coord, Coord);
    Coord Slide(Event&);
};

#endif
