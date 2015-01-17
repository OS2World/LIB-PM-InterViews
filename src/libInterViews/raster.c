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

#include <InterViews/raster.h>
#include <InterViews/color.h>
#include <bstring.h>

Raster::Raster (Color** data, long width, long height) {
    rep = new RasterRep(width, height);
    raster = new Color*[width * height];
    if (data != nil) {
	bcopy(data, (char*)raster, (int)(width*height*sizeof(Color*)));
    } else {
	bzero((char*)raster, (int)(width*height*sizeof(Color*)));
    }
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            long index = Index(x, y);
            Color* c = raster[index];
            if (c == nil) {
                c = white;
                raster[index] = c;
            }
            c->Reference();
            rep->PutPixel(x, y, c->PixelValue());
        }
    }
}

Raster::Raster (Canvas* c, Coord x0, Coord y0, long width, long height) {
    rep = new RasterRep(c, x0, y0, width, height);
    raster = nil;
}

Raster::Raster (Raster* r) {
    rep = new RasterRep(r->rep);
    long width = Width();
    long height = Height();
    if (r->raster != nil) {
        raster = new Color*[width * height];
        bcopy((char*)r->raster, (char*)raster, (int)(width*height*sizeof(Color*)));
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                long index = Index(x, y);
                Color* c = raster[index];
                if (c == nil) {
                    c = white;
                    raster[index] = c;
                }
                c->Reference();
                rep->PutPixel(x, y, c->PixelValue());
            }
        }
    } else {
        raster = nil;
    }
}

Raster::~Raster () {
    if (raster != nil) {
	long width = Width();
	long height = Height();
	for (int x = 0; x < width; ++x) {
	    for (int y = 0; y < height; ++y) {
		long index = Index(x, y);
		Unref(raster[index]);
	    }
	}
	delete raster;
    }
    delete rep;
}

Color* Raster::Peek (long x, long y) {
    if (Contains(x, y)) {
        if (raster == nil) {
            long width = Width();
            long height = Height();
            raster = new Color*[width * height];
            bzero((char*)raster, (int)(width*height*sizeof(Color*)));
        }
        long index = Index(x, y);
        Color* c = raster[index];
        if (c == nil) {
            c = new Color(rep->GetPixel(x, y));
	    c->Reference();
            raster[index] = c;
        }
        return c;
    } else {
        return nil;
    }
}

void Raster::Poke (Color* c, long x, long y) {
    if (Contains(x, y)) {
        if (raster == nil) {
            long width = Width();
            long height = Height();
            raster = new Color*[width * height];
            bzero((char*)raster, (int)(width*height*sizeof(Color*)));
        }
        long index = Index(x, y);
        Unref(raster[index]);
        raster[index] = c;
        c->Reference();
        rep->PutPixel(x, y, c->PixelValue());
    }
}
