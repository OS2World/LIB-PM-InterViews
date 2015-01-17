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

#ifndef shape_h
#define shape_h

#include <InterViews/defs.h>

/*
 * Constants for defining "infinite" stretchability or shrinkability.
 */

static const long hfil = 1000000;
static const long vfil = 1000000;

class Shape {
public:
    long width, height;		/* natural dimensions */
    long hstretch, vstretch;	/* stretchability */
    long hshrink, vshrink;	/* shrinkability */
    long aspect;		/* desired aspect ratio, 0 means don't care */
    long hunits, vunits;	/* allocate in multiples */

    Shape();
    ~Shape();

    void Square(long);
    void Rect(long w, long h);
    void Rigid(long hshr = 0, long hstr = 0, long vshr = 0, long vstr = 0);
    void SetUndefined();
    boolean Defined();
    boolean Undefined () { return !Defined(); }
};

#endif
