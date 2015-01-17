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
 * PM-dependent bitmap code
 */
#include <InterViews/bitmap.h>
#include <InterViews/font.h>
#include <InterViews/transformer.h>
#include <InterViews/PM/painterrep.h>
#include <InterViews/PM/worldrep.h>
#include <InterViews/PM/PM_lib.h>
#include <bstring.h>
#include <string.h>
#include <math.h>

/*
 * class Bitmap
 */

static int BitmapImageSize (unsigned int w, unsigned int h) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) BitmapImageSize");
  }
#endif

    return (w+7)/8 * h;
}

BitmapRep::BitmapRep (const char*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("BitmapRep::BitmapRep");
  }
#endif

}

BitmapRep::BitmapRep (void*, unsigned int, unsigned int, int, int) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("BitmapRep::BitmapRep (void*");
  }
#endif

}    

BitmapRep::BitmapRep (Font*, int) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("BitmapRep::BitmapRep (Font*");
  }
#endif

}    

BitmapRep::BitmapRep (BitmapRep*, BitTx) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("BitmapRep::BitmapRep (BitmapRep*");
  }
#endif

}

static void DrawSourceTransformedImage (
) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("DrawSourceTransformedImage");
  }
#endif

}

static void DrawDestinationTransformedImage (
) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("DrawDestinationTransformedImage");
  }
#endif

}

void DrawTransformedImage (
) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("DrawTransformedImage");
  }
#endif

}

BitmapRep::BitmapRep (BitmapRep*, Transformer*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("BitmapRep::BitmapRep (BitmapRep* b, Transformer*");
  }
#endif

}

BitmapRep::~BitmapRep () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("BitmapRep::~BitmapRep");
  }
#endif

}

void BitmapRep::Touch () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("BitmapRep::Touch");
  }
#endif

}

void BitmapRep::PutBit (int, int, boolean) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("BitmapRep::PutBit");
  }
#endif

}

boolean BitmapRep::GetBit (int, int) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("BitmapRep::GetBit");
  }
#endif

  return false;
}

void* BitmapRep::GetData () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("BitmapRep::GetData");
  }
#endif

  return 0;
}        

void* BitmapRep::GetMap () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("BitmapRep::GetMap");
  }
#endif

  return 0;
}
