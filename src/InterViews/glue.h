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
 * Glue is useful for variable spacing between interactors.
 */

#ifndef glue_h
#define glue_h

#include <InterViews/interactor.h>
#include <InterViews/shape.h>

class Glue : public Interactor {
protected:
    Glue();
    Glue(const char*);
    Glue(Painter* bg);

    void Redraw(Coord, Coord, Coord, Coord);
private:
    void Init();
};

class HGlue : public Glue {
public:
    HGlue(long natural = 0, long stretch = hfil);
    HGlue(const char*, long natural = 0, long stretch = hfil);
    HGlue(long natural, long shrink, long stretch);
    HGlue(const char*, long natural, long shrink, long stretch);
    HGlue(Painter* bg, long natural = 0, long stretch = hfil);
    HGlue(Painter* bg, long natural, long shrink, long stretch);
private:
    void Init(long nat, long shrink, long stretch);
};

class VGlue : public Glue {
public:
    VGlue(long natural = 0, long stretch = vfil);
    VGlue(const char*, long natural = 0, long stretch = vfil);
    VGlue(long natural, long shrink, long stretch);
    VGlue(const char*, long natural, long shrink, long stretch);
    VGlue(Painter* bg, long natural = 0, long stretch = vfil);
    VGlue(Painter* bg, long natural, long shrink, long stretch);
private:
    void Init(long nat, long shrink, long stretch);
};

#endif
