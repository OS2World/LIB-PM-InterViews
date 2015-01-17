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
 * PM-dependent cursors
 */

#include <InterViews/bitmap.h>
#include <InterViews/color.h>
#include <InterViews/cursor.h>
#include <InterViews/font.h>
#include <InterViews/PM/worldrep.h>
#include <InterViews/PM/PM_lib.h>
#include <malloc.h>
#include <process.h>

//New
Pixmap PMCreateBitmapFromData(char *data, const int width, const int height)
{
  Pixmap hbm;
  BITMAPINFO *pbmi;
  BITMAPINFOHEADER bmp;

  bmp.cbFix = sizeof bmp;
  bmp.cx = width;
  bmp.cy = height;
  bmp.cPlanes = 1;
  bmp.cBitCount = 1;

  hbm = GpiCreateBitmap(_world->hpsMemory(), &bmp, 0L, (PBYTE)NULL, (PBITMAPINFO)NULL);
  if ((int)hbm == GPI_ERROR) {
    ReportError(_world->hab(), "(1) ");
  }

  if (GpiSetBitmap(_world->hpsMemory(), hbm) == HBM_ERROR) {
    ReportError(_world->hab(), "(2) ");
  }

  pbmi = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + sizeof(RGB));
  if (pbmi == 0) {
    PMMessageBox("[1]  Out of memory");
    WinTerminate(_world->hab());
    exit(1);
  }  
  pbmi->cbFix = sizeof bmp;
  pbmi->cx = width;
  pbmi->cy = height;
  pbmi->cPlanes = 1;
  pbmi->cBitCount = 1;

  pbmi->argbColor[0].bBlue = 0;
  pbmi->argbColor[0].bGreen = 0;
  pbmi->argbColor[0].bRed = 0;
  pbmi->argbColor[1].bBlue = 0xFF;
  pbmi->argbColor[1].bGreen = 0xFF;
  pbmi->argbColor[1].bRed = 0xFF;

  if (GpiSetBitmapBits(_world->hpsMemory(),0L,16L,(PBYTE)data,pbmi) == GPI_ALTERROR) {
    ReportError(_world->hab(), "(3) ");
  }
  free(pbmi);
  
  return(hbm);
}

/*
 * Create the pixmap for a cursor.  These are always 16x16, unlike
 * fill patterns, which are 32x32.
 *
 * PM expects the bitmap array to be padded to a multiple of 32 bits. 
 * The bitmap data itself can be smaller and padded with zeros to the end.
 *
 * PM will use the ifdef'ed VAX version of this routine because it
 * looks simpler.  
 */

static Pixmap MakeCursorPixmap (int* scanline) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) MakeCursorPixmap");
  }
#endif

    /*
     * Cursor bitmaps work fine on the VAX; I suspect because
     * XPutImage doesn't have to do (misbehaving) swapping on a VAX.
     * The #else code fills the cursor pixmap explicitly because
     * cursor bitmaps don't seem to work right on other machines.
     */
    char data[2*cursorHeight];
    char* p;
    register int i, j;
    register unsigned int s1, s2;
    register unsigned int src, dst;
    union {
	char c[sizeof(short)];
	short s;
    } u;

    p = data;
    for (i = 0; i < cursorHeight; i++) {
	dst = 0;
	src = scanline[i];
	s1 = 1;
	s2 = 1 << (cursorWidth - 1);
	for (j = 0; j < cursorWidth; j++) {
	    if ((s1 & src) != 0) {
		dst |= s2;
	    }
	    s1 <<= 1;
	    s2 >>= 1;
	}
	u.s = dst;
	*p++ = u.c[0];
	*p++ = u.c[1];
    }
    //return XCreateBitmapFromData(
    //	_world->display(), _world->root(), data, cursorWidth, cursorHeight
    //);

    return PMCreateBitmapFromData(data, 
				  cursorWidth, 
				  cursorHeight);
  }

/*
 * Convert a Color object to the X representation.
 */

static void MakeColor (Color* c, XColor& xc) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) MakeColor");
  }
#endif

    long r, g, b;

    xc.rgbIndex = c->PixelValue();
    c->Intensities(r, g, b);
    xc.u.rgb.bRed = (unsigned char)r;
    xc.u.rgb.bGreen = (unsigned char)g;
    xc.u.rgb.bBlue = (unsigned char)b;
}

Cursor::Cursor (Bitmap*, Bitmap*, Color*, Color*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Cursor::Cursor");
  }
#endif

}

Cursor::Cursor (Font*, int, int, Color*, Color*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Cursor::Cursor (Font*");
  }
#endif

}

Cursor::Cursor (int, Color*, Color*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Cursor::Cursor (int ");
  }
#endif

}

void* Cursor::Id () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Cursor::Id");
  }
#endif

  return 0;
}

Cursor::~Cursor () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Cursor::~Cursor");
  }
#endif

}
