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
 * Basic composite object for interaction.
 */

#ifndef scene_h
#define scene_h

#include <InterViews/interactor.h>

class Scene : public Interactor {
public:
    void Insert(Interactor*);
    void Insert(Interactor*, Coord x, Coord y, Alignment = BottomLeft);
    void Change(Interactor* = nil);
    void Remove(Interactor*);

    void Move(Interactor*, Coord x, Coord y, Alignment = BottomLeft);
    void Raise(Interactor*);
    void Lower(Interactor*);

    void Propagate(boolean);

    virtual void Highlight(boolean);
protected:
    boolean propagate;

    Scene();
    Scene(Sensor*, Painter*);

    void PrepareToInsert(Interactor*);
    virtual Interactor* Wrap(Interactor*);
    virtual void DoInsert(Interactor*, boolean, Coord& x, Coord& y);
    virtual void DoChange(Interactor*);
    virtual void DoMove(Interactor*, Coord& x, Coord& y);
    virtual void DoRemove(Interactor*);
    virtual void DoRaise(Interactor*);
    virtual void DoLower(Interactor*);

    void Place(Interactor*, Coord, Coord, Coord, Coord, boolean map = true);
    void UserPlace(Interactor*, long w, long h);

    void Map(Interactor*, boolean raised = true);
    void Unmap(Interactor*);
private:
    void Assign(Interactor*, Coord x, Coord y, long w, long h);
    void DoAlign(Interactor*, Alignment, Coord& x, Coord& y);
    void DoMap(Interactor*, long w, long h);
    void InitCanvas(Interactor*);
    void MakeWindow(Interactor*, Coord x, Coord y, long w, long h);
    void SetWindowProperties(
	Interactor*, Coord x, Coord y, long w, long h, boolean placed
    );
    void Orphan(Interactor*);
};

/* Scene with a single component */
class MonoScene : public Scene {
public:
    MonoScene();
    MonoScene(Sensor*, Painter*);
    ~MonoScene();

    virtual void Draw();
    virtual void GetComponents(Interactor**, long, Interactor**&, long&);
    virtual void Resize();
protected:
    Interactor* component;

    virtual void DoInsert(Interactor*, boolean, Coord&, Coord&);
    virtual void DoChange(Interactor*);
    virtual void DoRemove(Interactor*);

    virtual void Reconfig();
};

#endif
