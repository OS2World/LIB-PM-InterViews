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
 * Tray - composes interactors into (possibly) overlapping layouts.
 */

#ifndef tray_h
#define tray_h

#include <InterViews/scene.h>
#include <InterViews/shape.h>

class TrayElement;
class TSolver;

class TGlue {
public:
    TGlue(long w = 0, long h = 0, long hstretch = hfil, long vstretch = vfil);
    TGlue(long w, long h, long hshr, long hstr, long vshr, long vstr);
    ~TGlue();

    Shape* GetShape();
private:
    Shape* shape;
};

class Tray : public Scene {
public:
    Tray(Interactor* background = nil);
    Tray(const char*, Interactor* background = nil);
    Tray(Sensor*, Painter*, Interactor* background = nil);
    ~Tray();
    
    virtual void Draw();
    virtual void Reshape(Shape&);
    virtual void GetComponents(Interactor**, long, Interactor**&, long&);

    void Align(Alignment, Interactor*, TGlue* = nil);
    void Align(Alignment, Interactor*, Alignment, Interactor*, TGlue* = nil);
    void Align(
	Alignment, Interactor*, Interactor*,
	Interactor* = nil, Interactor* = nil, Interactor* = nil,
	Interactor* = nil, Interactor* = nil
    );
    void HBox(
	Interactor*, Interactor*,
	Interactor* = nil, Interactor* = nil, Interactor* = nil,
	Interactor* = nil, Interactor* = nil
    );
    void VBox(
	Interactor*, Interactor*,
	Interactor* = nil, Interactor* = nil, Interactor* = nil,
	Interactor* = nil, Interactor* = nil
    );
protected:
    virtual void DoInsert(Interactor*, boolean, Coord& x, Coord& y);
    virtual void DoChange(Interactor*);
    virtual void DoRemove(Interactor*);
    virtual void Reconfig();
    virtual void Resize();
private:
    long nelements;
    TrayElement* head;
    TrayElement* tail;
    Interactor* bg;
    TSolver* tsolver;

    void Init(Interactor*);
    void ComponentBounds(long&, long&);
    boolean AlreadyInserted(Interactor*);
    void CalcShape();
    void PlaceElement(TrayElement*);
    boolean TrayOrBg(Interactor*);
};

inline Shape* TGlue::GetShape () { return shape; }

#endif
