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
 * A frame surrounds another interactor.
 */

#ifndef frame_h
#define frame_h

#include <InterViews/scene.h>

class Frame : public MonoScene {
public:
    Frame(Interactor* = nil, long width = 1);
    Frame(const char*, Interactor* = nil, long width = 1);
protected:
    unsigned left:8, bottom:8, right:8, top:8;
    
    Frame(Interactor*, long, long, long, long);
    Frame(const char*, Interactor*, long, long, long, long);

    virtual void Reconfig();
    virtual void Resize();
    virtual void Redraw(Coord, Coord, Coord, Coord);
private:
    void Init(Interactor*, long, long, long, long);
};

class ShowFrame : public Frame {
public:
    ShowFrame(Interactor* i = nil, long width = 1) : (i, width) { Init(); }
    ShowFrame(
	const char* name,
	Interactor* i = nil, long width = 1
    ) : (name, i, width) { Init(); }

    virtual void Handle(Event&);
    virtual void HandleInput(Event&);
    virtual void InsideFrame(boolean);
protected:
    ShowFrame(
	Interactor* i, long l, long b, long r, long h
    ) : (i, l, b, r, h) { Init(); }
    ShowFrame(
	const char* name,
	Interactor* i, long l, long b, long r, long h
    ) : (name, i, l, b, r, h) { Init(); }
private:
    void Init();
};

class Banner;

class TitleFrame : public ShowFrame {
public:
    TitleFrame(Banner*, Interactor*, long width = 1);
    TitleFrame(const char*, Banner*, Interactor*, long width = 1);

    virtual void InsideFrame(boolean);
protected:
    Banner* banner;

    virtual Interactor* Wrap(Interactor*);
private:
    void Init(Banner*, Interactor*);
};

class BorderFrame : public ShowFrame {
public:
    BorderFrame(Interactor* = nil, long width = 1);
    BorderFrame(const char*, Interactor* = nil, long width = 1);

    virtual void InsideFrame(boolean);
protected:
    virtual void Redraw(Coord, Coord, Coord, Coord);
private:
    boolean normal;

    void Init();
};

class ShadowFrame : public Frame {
public:
    ShadowFrame(Interactor* = nil, long h = 1, long v = 1);
    ShadowFrame(const char*, Interactor* = nil, long h = 1, long v = 1);
protected:
    virtual void Redraw(Coord, Coord, Coord, Coord);
private:
    void Init(Interactor*, long h, long v);
};

class MarginFrame : public Frame {
public:
    MarginFrame(Interactor* = nil, long margin = 0);
    MarginFrame(const char*, Interactor* = nil, long margin = 0);
    MarginFrame(Interactor*, long margin, long shrink, long stretch);
    MarginFrame(Interactor*, long hmargin, long vmargin);
    MarginFrame(Interactor*,
        long hmargin, long hshrink, long hstretch,
        long vmargin, long vshrink, long vstretch
    );
protected:
    virtual void Reconfig();
    virtual void Resize();
    virtual void Redraw(Coord, Coord, Coord, Coord);
protected:
    long hmargin, hshrink, hstretch;
    long vmargin, vshrink, vstretch;
private:
    void Init(long, long, long, long, long, long);
};

#endif
