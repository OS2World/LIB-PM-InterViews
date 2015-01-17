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
 * PM-dependent raster code
 */

#include <InterViews/canvas.h>
#include <InterViews/raster.h>
#include <InterViews/PM/worldrep.h>
#include <InterViews/PM/PM_lib.h>

static int RasterImageSize(long w, long h) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) RasterImageSize");
  }
#endif

    return w * h * sizeof(unsigned long);
}

RasterRep::RasterRep(long w, long h) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) RasterRep::RasterRep");
  }
#endif

    width = w;
    height = h;
    data = nil;
}

RasterRep::RasterRep(Canvas*, long, long, long, long) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("RasterRep::RasterRep");
  }
#endif

}

RasterRep::RasterRep(RasterRep*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("RasterRep::RasterRep (Canvas*");
  }
#endif

}

RasterRep::~RasterRep() {

#ifdef DEBUG
  if (debugP(3)) {
    debug("RasterRep::~RasterRep");
  }
#endif

}

void* RasterRep::GetData() {

#ifdef DEBUG
  if (debugP(3)) {
    debug("RasterRep::GetData");
  }
#endif

  return 0;
}

void RasterRep::PutPixel(long, long, long) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("RasterRep::PutPixel");
  }
#endif

}

long RasterRep::GetPixel(long, long) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("RasterRep::GetPixel");
  }
#endif

  return 0;
}
