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
 * Raster - rasterized image
 */

#ifndef raster_h
#define raster_h

#include <InterViews/defs.h>
#include <InterViews/resource.h>

class Color;
class Canvas;

class Raster : public Resource {
public:
    Raster(Color** data, long width, long height);
    Raster(Canvas*, Coord x0, Coord y0, long width, long height);
    Raster(Raster*);
    ~Raster();

    long Width();
    long Height();
    boolean Contains(long x, long y);
    long Index(long x, long y);

    Color* Peek(long x, long y);
    void Poke(Color*, long x, long y);
protected:
    friend class Painter;

    Color** raster;
    class RasterRep* rep;
};

class RasterRep {
private:
    friend class Raster;
    friend class Painter;

    RasterRep(long w, long h);
    RasterRep(Canvas*, Coord x0, Coord y0, long w, long h);
    RasterRep(RasterRep*);
    ~RasterRep();

    long GetPixel(long x, long y);
    void PutPixel(long x, long y, long);
    void* GetData();

    long width;
    long height;
    void* data;
};

inline long Raster::Width () { return rep->width; }
inline long Raster::Height () { return rep->height; }
inline boolean Raster::Contains (long x, long y) {
    return x >= 0 && x < Width() && y >= 0 && y < Height();
}
inline long Raster::Index (long x, long y) { return y * Width() + x; }

#endif
