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
 * PM-dependent painter code
 */

#include "btable.h"
#include <InterViews/bitmap.h>
#include <InterViews/brush.h>
#include <InterViews/canvas.h>
#include <InterViews/color.h>
#include <InterViews/font.h>
#include <InterViews/painter.h>
#include <InterViews/pattern.h>
#include <InterViews/raster.h>
#include <InterViews/transformer.h>
#include <InterViews/PM/painterrep.h>
#include <InterViews/PM/worldrep.h>
#include <InterViews/PM/PM_lib.h>
#include <InterViews/PM/canvasrep.h>

#ifdef DEBUG
#include <stdio.h>
#endif

PainterRep::PainterRep() {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) PainterRep::PainterRep");
  }
#endif

  /*
   *X11
   *fillgc = XCreateGC(_world->display(), _world->root(), 0, nil);
   *dashgc = XCreateGC(_world->display(), _world->root(), 0, nil);
   */
    
  lineAttrsMask = 0;
  charAttrsMask = 0;
  markerAttrsMask = 0;
  areaAttrsMask = 0;
  imageAttrsMask = 0;

  fillbg = true;
  overwrite = false;
  xor = false;
  FillPattern = (HBITMAP)NULL;
}

/* Done */
PainterRep::~PainterRep() {
}

void PainterRep::PrepareFill(void* info) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) PainterRep::PrepareFill");
  }
#endif

/*
    if (info == nil) {
        XSetFillStyle(_world->display(), fillgc, FillSolid);
    } else if (fillbg) {
        XSetStipple(_world->display(), fillgc, (Pixmap)info);
        XSetFillStyle(_world->display(), fillgc, FillOpaqueStippled);
    } else {
        XSetStipple(_world->display(), fillgc, (Pixmap)info);
        XSetFillStyle(_world->display(), fillgc, FillStippled);
    }
*/

    if (info == nil) {
      /* Fill solid */
      SetFillStyle(FM_OVERPAINT, BM_OVERPAINT);
      SetFillPattern((HBITMAP)0);
    } else if (fillbg) {
      /* Fill with pattern */
      SetFillStyle(FM_OVERPAINT, BM_OVERPAINT);
      SetFillPattern((HBITMAP)info);
    } else {
      /* Fill with pattern, leave unset bits alone */
      SetFillStyle(FM_OVERPAINT, BM_LEAVEALONE);
      SetFillPattern((HBITMAP)info);
    }
}

void PainterRep::PrepareDash(int width, void* info, int count) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) PainterRep::PrepareDash");
  }
#endif

    if (count == 0) {

        /*
         *X11
         *XSetLineAttributes(
         *    _world->display(), dashgc, width,
         *    LineSolid, CapButt, JoinMiter
         *);
	 */
	
	lineAttrs.fxWidth = (width == 0) ? 1 : (short)width;
	lineAttrs.usType = (short)LINETYPE_SOLID;
	lineAttrs.usEnd = (short)LINEEND_FLAT;
	lineAttrs.usJoin = (short)LINEJOIN_MITRE;
	lineAttrsMask |= LBB_WIDTH | LBB_TYPE | LBB_END | LBB_JOIN;
/*
        if (info == nil) {
            XSetFillStyle(
                _world->display(), dashgc, FillSolid
            );
        } else if (fillbg) {
            XSetFillStyle(
                _world->display(), dashgc, FillOpaqueStippled
            );
            XSetStipple(_world->display(), dashgc, (Pixmap)info);
        } else {
            XSetFillStyle(
                _world->display(), dashgc, FillStippled
            );
            XSetStipple(_world->display(), dashgc, (Pixmap)info);
        }
*/
    } else {
/*
        XSetFillStyle(_world->display(), dashgc, FillSolid);
*/
        if (info == nil) {

	    /*
             *X11
             *XSetLineAttributes(
             *    _world->display(), dashgc, width,
             *    LineSolid, CapButt, JoinMiter
             *);
	     */

	    lineAttrs.fxWidth = (width == 0) ? 1 : (short)width;
	    lineAttrs.usType = (short)LINETYPE_SOLID;
	    lineAttrs.usEnd = (short)LINEEND_FLAT;
	    lineAttrs.usJoin = (short)LINEJOIN_MITRE;
	    lineAttrsMask |= LBB_WIDTH | LBB_TYPE | LBB_END | LBB_JOIN;

        } else if (fillbg) {

	    /*
             *X11
             *XSetLineAttributes(
             *    _world->display(), dashgc, width,
             *    LineDoubleDash, CapButt, JoinMiter
             *);
             */

	    /* Don't have an exact replacement for LineDoubleDash */
	    lineAttrs.fxWidth = (width == 0) ? 1 : width;
	    lineAttrs.usType = (short)LINETYPE_DASHDOUBLEDOT;
	    lineAttrs.usEnd = (short)LINEEND_FLAT;
	    lineAttrs.usJoin = (short)LINEJOIN_MITRE;
	    lineAttrsMask |= LBB_WIDTH | LBB_TYPE | LBB_END | LBB_JOIN;
/*
            XSetDashes(
                _world->display(), dashgc, 0, (char*)info, count
            );
*/
        } else {

            /*
	     *X11
             *XSetLineAttributes(
             *    _world->display(), dashgc, width,
             *    LineOnOffDash, CapButt, JoinMiter
             *);
	     */

	    lineAttrs.fxWidth = (width == 0) ? 1 : width;
	    lineAttrs.usType = (short)LINETYPE_SHORTDASH;
	    lineAttrs.usEnd = (short)LINEEND_FLAT;
	    lineAttrs.usJoin = (short)LINEJOIN_MITRE;
	    lineAttrsMask = LBB_WIDTH | LBB_TYPE | LBB_END | LBB_JOIN;

/*
            XSetDashes(
                _world->display(), dashgc, 0, (char*)info, count
            );
*/
        }
    }
}


/*
 * Short-hand for allocating a vector of X points.
 * The idea is to use a static array if possible; otherwise
 * allocate/deallocate off the heap.
 */


static const XPointListSize = 200;
static XPoint xpoints[XPointListSize];

inline XPoint* AllocPts(int n) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) AllocPts");
  }
#endif

    return (n <= XPointListSize) ? xpoints : new XPoint[n];
}

inline void FreePts(XPoint* v) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) FreePts");
  }
#endif

    if (v != xpoints) {
	delete v;
    }
}

Painter::Painter() {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Painter::Painter");
  }
#endif

    rep = new PainterRep;
    Init();
}

Painter::Painter(Painter* copy) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Painter::Painter");
  }
#endif

    rep = new PainterRep;

    rep->fillbg = copy->rep->fillbg;
    rep->overwrite = copy->rep->overwrite;
    Copy(copy);
    if (copy->rep->xor) {
	Begin_xor();
    }
    rep->xor = copy->rep->xor;
/*
    if (rep->overwrite) {
	XSetSubwindowMode(_world->display(), rep->fillgc, IncludeInferiors);
	XSetSubwindowMode(_world->display(), rep->dashgc, IncludeInferiors);
    }
*/
}

Painter::~Painter() {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Painter::~Painter");
  }
#endif

    Unref(matrix);
    Unref(font);
    Unref(br);
    Unref(foreground);
    Unref(background);
    Unref(pattern);
    delete rep;
}

void Painter::FillBg(boolean b) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Painter::FillBg");
  }
#endif

    if (rep->fillbg != b) {
        if (rep->xor) {
            End_xor();
        }
        rep->fillbg = b;
        if (pattern != nil) {
            rep->PrepareFill(pattern->info);
        }
        if (br != nil) {
            rep->PrepareDash(br->width, br->rep->info, br->rep->count);
        }
    }
}

boolean Painter::BgFilled() {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Painter::BgFilled");
  }
#endif

  return rep->fillbg;
}

void Painter::SetColors(Color* f, Color* b) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Painter::SetColors");
  }
#endif

  if (rep->xor) {                  
    End_xor();
  }
  if (f != nil && foreground != f) { 
    Unref(foreground);
    foreground = f;
    foreground->Reference();

    /*
     *X11
     *XSetForeground(
     *         _world->display(), rep->fillgc, foreground->PixelValue()
     *);
     *XSetForeground(
     *         _world->display(), rep->dashgc, foreground->PixelValue()
     *);
     */

    rep->SetForegroundColor(foreground->PixelValue());
  }
  if (b != nil && background != b) {
    Unref(background);
    background = b;
    background->Reference();

    /*
     *X11
     *XSetBackground(
     *        _world->display(), rep->fillgc, background->PixelValue()
     *);
     *XSetBackground(
     *        _world->display(), rep->dashgc, background->PixelValue()
     *);
     */

    rep->SetBackgroundColor(background->PixelValue());
  }
}

void Painter::SetPattern(Pattern* pat) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Painter::SetPattern");
  }
#endif

    if (rep->xor) {
	End_xor();
    }
    if (pattern != pat) {
        Unref(pattern);
	pattern = pat;
        if (pattern != nil) {
            pattern->Reference();
            rep->PrepareFill(pattern->info);
        }
    }
}

void Painter::SetBrush(Brush* b) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Painter::SetBrush");
  }
#endif

    if (rep->xor) {
	End_xor();
    }
    if (br != b) {
        Unref(br);
	br = b;
	if (br != nil) {
            br->Reference();
            rep->PrepareDash(br->width, br->rep->info, br->rep->count);
	}
    }
}

void Painter::SetFont(Font* f) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Painter::SetFont");
  }
#endif

    if (font != f) {
        Unref(font);
        font = f;
        if (font != nil) {
            font->Reference();

            //XSetFont(_world->display(), rep->fillgc, (XFont)font->rep->id);

            //GpiSetCharSet(rep->fillgc, ID_LCID); 
        }
    }
}

void Painter::Clip(
    Canvas*, Coord, Coord, Coord, Coord
) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Painter::Clip");
  }
#endif

}

void Painter::NoClip() {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Painter::NoClip");
  }
#endif

}

void Painter::SetOverwrite(boolean) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Painter::SetOverwrite");
  }
#endif

}

void Painter::SetPlaneMask(int ) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Painter::setPlaneMask");
  }
#endif

}

void Painter::Map(Canvas*, Coord x, Coord y, Coord& mx, Coord& my) {

#ifdef DEBUG
  if (debugP(6)) {
    debug("(Done) Painter::Map");
  }
#endif

    if (matrix == nil) {
	mx = x; my = y;
    } else {
	matrix->Transform(x, y, mx, my);
    }
    mx += xoff;

    /* my = c->height - 1 - (my + yoff); */
    my += yoff;
}

void Painter::MapList(
    Canvas* c, Coord x[], Coord y[], long n, Coord mx[], Coord my[]
) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Painter::MapList");
  }
#endif

    register Coord* xp, * yp, * mxp, * myp;
    Coord* lim;

    xp = x; yp = y;
    mxp = mx; myp = my;
    lim = &x[n];
    if (matrix == nil) {
	for (; xp < lim; xp++, yp++, mxp++, myp++) {
	    *mxp = *xp + xoff;
	    *myp = c->height - 1 - (*yp + yoff);
	}
    } else {
	for (; xp < lim; xp++, yp++, mxp++, myp++) {
	    matrix->Transform(*xp, *yp, *mxp, *myp);
	    *mxp += xoff;
	    *myp = c->height - 1 - (*myp + yoff);
	}
    }
}

void Painter::MapList(
    Canvas* c, float x[], float y[], long n, Coord mx[], Coord my[]
) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Painter::MapList(Canvas* c, float");
  }
#endif

    register float* xp, * yp;
    register Coord* mxp, * myp;
    float tmpx, tmpy, * lim;

    xp = x; yp = y;
    mxp = mx; myp = my;
    lim = &x[n];
    if (matrix == nil) {
	for (; xp < lim; xp++, yp++, mxp++, myp++) {
	    *mxp = round(*xp + xoff);
	    *myp = round(c->height - 1 - (*yp + yoff));
	}
    } else {
	for (; xp < lim; xp++, yp++, mxp++, myp++) {
	    matrix->Transform(*xp, *yp, tmpx, tmpy);
	    *mxp = round(tmpx + xoff);
	    *myp = round(c->height - 1 - (tmpy + yoff));
	}
    }
}

void Painter::Begin_xor() {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Painter::Begin_xor");
  }
#endif

}

void Painter::End_xor() {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Painter::End_xor");
  }
#endif

}

inline char _txkey (Coord i) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) _txkey");
  }
#endif

    if (i >= 0) {
        return (
            i < 32 ? i
            : i < 160 ? (24 + (i>>2))
            : i < 672 ? (54 + (i>>4))
            : 127
        );
    } else {
        return (
            i > -32 ? i
            : i > -160 ? (-24 - (i>>2))
            : i > -672 ? (-54 - (i>>4))
            : -127
        );
    }
}

static int TxKey(Transformer* t, int x, int y) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) TxKey");
  }
#endif

    if (t == nil) {
        return 0;
    } else {
        Coord x1, y1, x2, y2, x3, y3;
        t->Transform(0, 0, x1, y1);
        t->Transform(0, y, x2, y2);
        t->Transform(x, 0, x3, y3);
        return (
              (_txkey(x2 - x1) << 24)
            + (_txkey(y2 - y1 - y) << 16)
            + (_txkey(x3 - x1 - x) << 8)
            + (_txkey(y3 - y1))
        );
    }
}

void Painter::Stencil(Canvas*, Coord, Coord, Bitmap*, Bitmap*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Painter::Stencil");
  }
#endif

}

void Painter::RasterRect(Canvas*, Coord, Coord, Raster*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Painter::RasterRect");
  }
#endif

}

static Bitmap* GetCharBitmap(
    Font*, int, int, Transformer*
) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Painter::GetCharBitmap");
  }
#endif

  return 0;
}

void Painter::Text(Canvas* c, const char* s, long len, Coord x, Coord y) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Painter::Text");
  }
#endif

    Coord x0, y0;
    Coord ybase = y + font->Baseline();
    Coord ytop = y + font->Height() - 1;
    int txstring = TxKey(matrix, font->Width(s, (int)len), font->Height());
    HPS hps;
    POINTL point;

#ifdef DEBUG
      if (debugP(8)) {
      if (c != (Canvas*)NULL) {
	DoPrintMsg("Painter::Text ", c->id, 1);
      }
      }
#endif

    if (style & Reversed) {
        SetColors(GetBgColor(), GetFgColor());
    }
    if (c != (Canvas*)NULL) {
      hps = WinGetPS((HWND)c->id);
      if (txstring == 0) {
	Map(c, x, ybase, x0, y0);
	if (rep->fillbg) {

	  /*
	    X11
            XDrawImageString(
               _world->display(), (Drawable)c->id,
                rep->fillgc, x0, y0, s, len
            );

	    Not doing this yet.
	    rep->Update(c);
	    GpiCharStringPos((HPS)c->id,
	    */

	  point.x = x0;
	  point.y = y0;
	  rep->Update(hps);
	  if (GpiCharStringAt(hps, &point, len, (PCH)s) == GPI_ERROR) {
            ReportError(_world->hab(), "(30) ");
          }
        } else {

          /*
	    X11
            XDrawString(
                _world->display(), (Drawable)c->id,
                rep->fillgc, x0, y0, s, len
            );
	    */

	  point.x = x0;
	  point.y = y0;
	  rep->Update(hps);
	  if (GpiCharStringAt(hps, &point, len, (PCH)s) == GPI_ERROR) {
            ReportError(_world->hab(), "(5) ");
          }
        }
        if (style & Boldface) {

          /*
	    X11
            XDrawString(
                _world->display(), (Drawable)c->id,
                rep->fillgc, x0-1, y0, s, len
            );
	    */

	  point.x = x0 - 1;
	  point.y = y0;
	  rep->Update(hps);
	  if (GpiCharStringAt(hps, &point, len, (PCH)s) == GPI_ERROR) {
            ReportError(_world->hab(), "(6) ");
          }
        }
      } else {
        Coord curx = x;
        float fx0, fy0;
        Transformer notrans(matrix);
        notrans.Transform(0.0, 0.0, fx0, fy0);
        notrans.Translate(-fx0, -fy0);
        int txchar = TxKey(matrix, font->Width("M"), font->Height());
        Bitmap* bits;
        for (int i = 0; i < len; ++i) {
            Coord nextx = curx + font->Width(s+i, 1);
            if (rep->fillbg) {
                ClearRect(c, curx, y, nextx, ytop);
            }
            switch (_world->txfonts()) {
            case TxFontsOff:
                Map(c, curx, ybase, x0, y0);

		//X11
                //XDrawString(
                //    _world->display(), (Drawable)c->id,
                //    rep->fillgc, x0, y0, s+i, 1
                //);

		point.x = x0;
		point.y = y0;
		rep->Update(hps);
		if (GpiCharStringAt(hps, &point, 1, (PCH)s+i) == GPI_ERROR) {
                  ReportError(_world->hab(), "(7) ");
                }
                if (style & Boldface) {

		    //X11
                    //XDrawString(
                    //    _world->display(), (Drawable)c->id,
                    //    rep->fillgc, x0-1, y0, s+i, 1
                    //);
		    point.x = x0 - 1;
		    point.y = y0;
		    rep->Update(hps);
		    if (GpiCharStringAt(hps,&point,1,(PCH)s+i) == GPI_ERROR) {
                      ReportError(_world->hab(), "(8) ");
                    }
                }
                break;
            case TxFontsOn:
                bits = new Bitmap(font, s[i]);
                Stencil(c, curx, ybase, bits, bits);
                if (style & Boldface) {
                    Stencil(c, curx-1, ybase, bits, bits);
                }
                break;
            case TxFontsCache:
            case TxFontsDefault:
		/*
                bits = GetCharBitmap(font, s[i], txchar, &notrans);
                Transformer* oldmatrix = matrix;
                matrix = nil;
                oldmatrix->Transform(curx, ybase, x0, y0);
                Stencil(c, x0, y0, bits, bits);
                if (style & Boldface) {
                    oldmatrix->Transform(curx+1, ybase, x0, y0);
                    Stencil(c, x0, y0, bits, bits);
                }
                matrix = oldmatrix;
		*/
                break;
            }
            curx = nextx;
        }
      }
      WinReleasePS(hps);
    } else {
#ifdef DEBUG
    debug("*** Painter::Text - No input canvas ***");
#endif
    }
    if (style & Outlined) {
        /* unimplemented */
    }
    if (style & Underlined) {
	Line(c, x, ybase, x + font->Width(s, (int)len) - 1, ybase);
    }
    if (style & Reversed) {
        SetColors(GetBgColor(), GetFgColor());
    }
}

void Painter::Point(Canvas*, Coord, Coord) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Painter::Point");
  }
#endif

}

void Painter::MultiPoint(Canvas*, Coord [], Coord [], long) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Painter::MultiPoint");
  }
#endif

}

void Painter::Line(Canvas* c, Coord x1, Coord y1, Coord x2, Coord y2) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Painter::Line");
  }
#endif


  Coord mx1, my1, mx2, my2;
  POINTL point;
  HPS hps;

  Map(c, x1, y1, mx1, my1);
  Map(c, x2, y2, mx2, my2);
  
  /*
    X11
    XDrawLine(
        _world->display(), (Drawable)c->id, rep->dashgc, mx1, my1, mx2, my2
    );
   */

  if (c != (Canvas*)NULL) {
    hps = WinGetPS((HWND)c->id);
    rep->Update(hps);
    
    point.x = mx1;
    point.y = my1;
    if (GpiMove(hps, &point) == FALSE) {
      ReportError(_world->hab(), "(33) ");
    }
    point.x = mx2;
    point.y = my2;
    if (GpiLine(hps, &point) == GPI_ERROR) {
      ReportError(_world->hab(), "(34) ");
    }

    WinReleasePS(hps);
  } else {
#ifdef DEBUG
    debug("*** Painter::Line - No input canvas ***");
#endif
  }
}

void Painter::Rect(Canvas* c, Coord x1, Coord y1, Coord x2, Coord y2) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Painter::Rect");
  }
#endif

  if (matrix != nil && matrix->Rotated() && !matrix->Rotated90()) {
    Coord x[4], y[4];
    
    x[0] = x[3] = x1;
    x[1] = x[2] = x2;
    y[0] = y[1] = y1;
    y[2] = y[3] = y2;
    Polygon(c, x, y, 4);
  } else {
    Coord left, bottom, right, top, tmp;
    int w, h;
    HPS hps;
    XPoint point[5];

    Map(c, x1, y1, left, bottom);
    Map(c, x2, y2, right, top);
    if (left > right) {
      tmp = left; left = right; right = tmp;
    }
    if (top > bottom) {
      tmp = bottom; bottom = top; top = tmp;
    }
    w = (int)(right - left);
    h = (int)(bottom - top);

    //X11
    //XDrawRectangle(
    //	_world->display(), (Drawable)c->id, rep->dashgc, left, top, w, h
    //);

    
    if (c != (Canvas*)NULL) {
      hps = WinGetPS((HWND)c->id);

      rep->Update(hps);

      if (GpiBeginPath(hps, (LONG)1) == FALSE) {
	ReportError(_world->hab(), "(65) ");
      }
      point[0].x = left; 
      point[0].y = bottom;
      point[1].x = left; 
      point[1].y = top;
      point[2].x = right;
      point[2].y = top;
      point[3].x = right;
      point[3].y = bottom;
      point[4].x = left;
      point[4].y = bottom;

      if (GpiSetCurrentPosition(hps, &point[0]) == FALSE) {
	ReportError(_world->hab(), "(53) ");
      }
      if (GpiPolyLine(hps, 5L, point) == GPI_ERROR) {
	ReportError(_world->hab(), "(54) ");
      }
      if (GpiEndPath(hps) == FALSE) {
	ReportError(_world->hab(), "(67) ");
      }
      GpiStrokePath(hps, 1L, 0L);

      WinReleasePS(hps);
    } else {
#ifdef DEBUG
      debug("*** Painter::Rect - No input canvas ***");
#endif
    }
  }
}

void Painter::FillRect(Canvas* c, Coord x1, Coord y1, Coord x2, Coord y2) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Painter::FillRect");
  }
#endif

    if (matrix != nil && matrix->Rotated() && !matrix->Rotated90()) {
	Coord x[4], y[4];

	x[0] = x[3] = x1;
	x[1] = x[2] = x2;
	y[0] = y[1] = y1;
	y[2] = y[3] = y2;
	FillPolygon(c, x, y, 4);
    } else {
	Coord left, bottom, right, top, tmp;
        HPS hps;
        POINTL ptl;

	Map(c, x1, y1, left, bottom);
	Map(c, x2, y2, right, top);
	if (left > right) {
	    tmp = left; left = right; right = tmp;
	}
	if (bottom > top) {
	    tmp = bottom; bottom = top; top = tmp;
	}

        /*
         *X11
	 *w = right - left + 1;
	 *h = bottom - top + 1;
	 *XFillRectangle(
         *    _world->display(), (Drawable)c->id, rep->fillgc, left, top, w, h
         *);
         */
    
	if (c != (Canvas*)NULL) {
	  hps = WinGetPS((HWND)c->id);
	  rep->Update(hps);

//	  ptl.x = left; ptl.y = bottom + 1;
	  ptl.x = left; ptl.y = bottom - 1;
	  if (GpiMove(hps, &ptl) == FALSE) {
	    ReportError(_world->hab(), "(31) ");
	  }
//	  ptl.x = right - 1; ptl.y = top;
	  ptl.x = right + 1; ptl.y = top;
	  if (GpiBox(hps, DRO_FILL, &ptl, 0L, 0L) == GPI_ERROR) {
	    ReportError(_world->hab(), "(64) ");
	  }

#ifdef DEBUG
          if (debugP(8)) {
            char buf[80];
            RECTL rcl;
            DoPrintMsg("FillRect", (HWND)c->id, 1002);
            sprintf(buf, "FillRect:  left = %ld, bottom = %ld, right = %ld, top = %ld", 
                   left, bottom, right, top);
            debug(buf);
            WinQueryWindowRect((HWND)c->id, (PRECTL)&rcl);
            sprintf(buf, "WinQueryWindowRect:  left = %ld, bottom = %ld, right = %ld, top = %ld\n",
                    rcl.xLeft, rcl.yBottom, rcl.xRight, rcl.yTop);
            debug(buf);
          }
#endif
          WinReleasePS(hps);
	} else {
#ifdef DEBUG
	  debug("*** Painter::FillRect - No input canvas ***");
#endif
	}
    }
}

void Painter::ClearRect(Canvas* c, Coord x1, Coord y1, Coord x2, Coord y2) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Painter::ClearRect");
  }
#endif

    /*
     *X11
     *XSetForeground(_world->display(), rep->fillgc, background->PixelValue());
     */

    rep->SetForegroundColor(background->PixelValue());

/*
 *    Pattern* curpat = pattern;
 *    SetPattern(solid);
 */
    FillRect(c, x1, y1, x2, y2);

    /*
     *X11
     *XSetForeground(_world->display(), rep->fillgc, foreground->PixelValue());
     */

  rep->SetForegroundColor(foreground->PixelValue());

/*
    SetPattern(curpat);
*/
}

void Painter::Circle(Canvas* c, Coord x, Coord y, long r) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Painter::Circle");
  }
#endif

  if (matrix != nil && (matrix->Stretched() || matrix->Rotated())) {
    Ellipse(c, x, y, r, r);
  } else {
    Coord left, top, right, bottom;

    Map(c, x-r, y+r, left, top);
    Map(c, x+r, y-r, right, bottom);

    /*
     *X11
     *XDrawArc(
     *	     _world->display(), (Drawable)c->id, rep->dashgc,
     *	     left, top, right-left, bottom-top, 0, 360*64
     *	     );
     */
    HPS hps = WinGetPS((HWND)c->id);
    rep->Update(hps);
    POINTL ptl;
    ptl.x = left;
    ptl.y = bottom;
    if (GpiSetCurrentPosition(hps, &ptl) == FALSE) {
      ReportError(_world->hab(), "(83) ");
    }
    ptl.x = right;
    ptl.y = top;
    if (GpiBox(hps, DRO_OUTLINE, &ptl, right-left, bottom-top) == GPI_ERROR) {
      ReportError(_world->hab(), "(84) ");
    }
    WinReleasePS(hps);
  }
}

void Painter::FillCircle(Canvas* c, Coord x, Coord y, long r) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Painter::FillCircle");
  }
#endif

  if (matrix != nil && (matrix->Stretched() || matrix->Rotated())) {
    FillEllipse(c, x, y, r, r);
  } else {
    Coord left, top, right, bottom;

    Map(c, x-r, y+r, left, top);
    Map(c, x+r, y-r, right, bottom);
    /*
     *X11
     *XFillArc(
     *	     _world->display(), (Drawable)c->id, rep->fillgc, 
     *	     left, top, right-left, bottom-top, 0, 360*64
     *	     );
     */
    HPS hps = WinGetPS((HWND)c->id);
    rep->Update(hps);
    POINTL ptl;
    ptl.x = left;
    ptl.y = bottom;
    if (GpiSetCurrentPosition(hps, &ptl) == FALSE) {
      ReportError(_world->hab(), "(85) ");
    }
    ptl.x = right;
    ptl.y = top;
    if (GpiBox(hps, DRO_OUTLINEFILL, &ptl, right-left, bottom-top)
	== GPI_ERROR) {
      ReportError(_world->hab(), "(86) ");
    }
    WinReleasePS(hps);
  }
}

void Painter::MultiLine(Canvas* c, Coord x[], Coord y[], long n) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Painter::MultiLine");
  }
#endif

  XPoint v;
  int i;

  /*
   *X11
   *XDrawLines(
   *	     _world->display(), (Drawable)c->id,
   *	     rep->dashgc, v, n, CoordModeOrigin
   *	     );
   */

  if (c != (Canvas*)NULL) {
    HPS hps = WinGetPS((HWND)c->id);
    rep->Update(hps);
    
    Map(c, x[0], y[0], v.x, v.y);
    if (GpiMove(hps, &v) == FALSE) {
      ReportError(_world->hab(), "(87) ");
    }
    for (i = 1; i < n; i++) {
      Map(c, x[i], y[i], v.x, v.y);
      if (GpiLine(hps, &v) == GPI_ERROR) {
	ReportError(_world->hab(), "(88) ");
      }
    }
    WinReleasePS(hps);
  } else {
#ifdef DEBUG
    debug("*** Painter::MultiLine - No input canvas ***");
#endif
  }
}

void Painter::MultiLineNoMap(Canvas* c, Coord x[], Coord y[], long n) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Painter::MultiLineNoMap");
  }
#endif

  XPoint v;
  int i;

  /*
   *XDrawLines(
   *	     _world->display(), (Drawable)c->id,
   *	     rep->dashgc, v, n, CoordModeOrigin
   *	     );
   */

  if (c != (Canvas*)NULL) {
    HPS hps = WinGetPS((HWND)c->id);
    rep->Update(hps);
    
    v.x = x[0];
    v.y = y[0];
    if (GpiMove(hps, &v) == FALSE) {
      ReportError(_world->hab(), "(89) ");
    }
    for (i = 1; i < n; i++) {
      v.x = x[i];
      v.y = y[i];
      if (GpiLine(hps, &v) == GPI_ERROR) {
	ReportError(_world->hab(), "(90) ");
      }
    }
    WinReleasePS(hps);
  } else {
#ifdef DEBUG
    debug("*** Painter::MultiLineNoMap - No input canvas ***");
#endif
  }
}

void Painter::Polygon(Canvas* c, Coord x[], Coord y[], long n) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Painter::Polygon");
  }
#endif

  register int i;
  XPoint* v;
  HPS hps;
  Coord xx, yy;

  v = AllocPts((int)(n+1));
  for (i = 0; i < n; i++) {
    /*
     * The compiler didn't like passing in v[i].x and v[i].y to Map.
     */
    xx = v[i].x;  
    yy = v[i].y;
    Map(c, x[i], y[i], xx, yy);
    v[i].x = xx;
    v[i].y = yy;
  }

  /*
    X11
    XDrawLines(
       _world->display(), (Drawable)c->id,
       rep->dashgc, v, i, CoordModeOrigin
    );
    */

  if (c != (Canvas*)NULL) {
    hps = WinGetPS((HWND)c->id);
    rep->Update(hps);

    if (GpiBeginPath(hps, (LONG)1) == FALSE) {
      ReportError(_world->hab(), "(55) ");
    }
    if (GpiSetCurrentPosition(hps, v) == FALSE) {
      ReportError(_world->hab(), "(56) ");
    }
    if (GpiPolyLine(hps, n, v) == GPI_ERROR) {
      ReportError(_world->hab(), "(57) ");
    }
    if (GpiEndPath(hps) == FALSE) {
      ReportError(_world->hab(), "(58) ");
    }
    GpiStrokePath(hps, 1L, 0L);

    WinReleasePS(hps);
    FreePts(v);
  } else {
#ifdef DEBUG
    debug("*** Painter::Polygon - No input canvas ***");
#endif
  }
}

void Painter::FillPolygonNoMap(Canvas* c, Coord x[], Coord y[], long n) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Painter::FillPolygonNoMap");
  }
#endif

  register int i;
  XPoint* v;
  HPS hps;

  v = AllocPts((int)(n+1));
  for (i = 0; i < n; i++) {
    v[i].x = x[i];
    v[i].y = y[i];
  }

  /*
   *X11
   *XFillPolygon(
   *	       _world->display(), (Drawable)c->id, rep->fillgc,
   *	       v, n, Complex, CoordModeOrigin
   *	       );
   */

  if (c != (Canvas*)NULL) {
    hps = WinGetPS((HWND)c->id);
    rep->Update(hps);

    if (GpiBeginPath(hps, (LONG)1) == FALSE) {
      ReportError(_world->hab(), "(91) ");
    }
    if (GpiSetCurrentPosition(hps, v) == FALSE) {
      ReportError(_world->hab(), "(92) ");
    }
    if (GpiPolyLine(hps, n, v) == GPI_ERROR) {
      ReportError(_world->hab(), "(93) ");
    }
    if (GpiEndPath(hps) == FALSE) {
      ReportError(_world->hab(), "(94) ");
    }
    if (GpiFillPath(hps, (LONG)1, FPATH_ALTERNATE) == GPI_ERROR) {
      ReportError(_world->hab(), "(95) ");
    }

    WinReleasePS(hps);
    FreePts(v);
  } else {
#ifdef DEBUG
    debug("*** Painter::FillPolygonNoMap - No input canvas ***");
#endif
  }
}

void Painter::FillPolygon(Canvas* c, Coord x[], Coord y[], long n) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Painter::FillPolygon");
  }
#endif

  register int i;
  XPoint* v;
  HPS hps;
  Coord xx, yy;

  v = AllocPts((int)(n+1));
  for (i = 0; i < n; i++) {
    /*
     * The compiler didn't like passing in v[i].x and v[i].y to Map.
     */
    xx = v[i].x;  
    yy = v[i].y;
    Map(c, x[i], y[i], xx, yy);
    v[i].x = xx;
    v[i].y = yy;
  }

  /*
   *X11
   *XFillPolygon(
   *	       _world->display(), (Drawable)c->id, rep->fillgc,
   *	       v, n, Complex, CoordModeOrigin
   *	       );
   */

  if (c != (Canvas*)NULL) {
    hps = WinGetPS((HWND)c->id);
    rep->Update(hps);

    if (GpiBeginPath(hps, (LONG)1) == FALSE) {
      ReportError(_world->hab(), "(35) ");
    }
    if (GpiSetCurrentPosition(hps, v) == FALSE) {
      ReportError(_world->hab(), "(36) ");
    }
    if (GpiPolyLine(hps, n, v) == GPI_ERROR) {
      ReportError(_world->hab(), "(37) ");
    }
    if (GpiEndPath(hps) == FALSE) {
      ReportError(_world->hab(), "(38) ");
    }
    if (GpiFillPath(hps, (LONG)1, FPATH_ALTERNATE) == GPI_ERROR) {
      ReportError(_world->hab(), "(39) ");
    }

    WinReleasePS(hps);
    FreePts(v);
  } else {
#ifdef DEBUG
    debug("*** Painter::FillPolygon - No input canvas ***");
#endif
  }
}

void Painter::Copy(
    Canvas* , Coord, Coord, Coord, Coord,
    Canvas* , Coord, Coord
) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Painter::Copy");
  }
#endif

}

/* Done */
void Painter::Read(
    Canvas* c, void* dst, Coord x1, Coord y1, Coord x2, Coord y2
) {
    /* unimplemented -- use Raster::Raster(Canvas*, ...) */
}

/* Done */
void Painter::Write(
    Canvas* c, const void* src, Coord x1, Coord y1, Coord x2, Coord y2
) {
    /* unimplemented -- use Painter::RasterRect(Canvas*, ...) */
}

/*
 * New
 */

void PainterRep::SetForegroundColor(unsigned long index)
{

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) PainterRep::SetForegroundColor");
  }
#endif

  lineAttrs.lColor = index;
  lineAttrsMask |= LBB_COLOR;
  charAttrs.lColor = index;
  charAttrsMask |= CBB_COLOR;
  markerAttrs.lColor = index;
  markerAttrsMask |= MBB_COLOR;
  areaAttrs.lColor = index;
  areaAttrsMask |= ABB_COLOR;
  imageAttrs.lColor = index;
  imageAttrsMask |= IBB_COLOR;
}

void PainterRep::SetBackgroundColor(long index)
{

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) PainterRep::SetBackgroundColor");
  }
#endif

  charAttrs.lBackColor = index;
  charAttrsMask |= CBB_BACK_COLOR;
  markerAttrs.lBackColor = index;
  markerAttrsMask |= MBB_BACK_COLOR;
  areaAttrs.lBackColor = index;
  areaAttrsMask |= ABB_BACK_COLOR;
  imageAttrs.lBackColor = index;
  imageAttrsMask |= IBB_BACK_COLOR;
}

void PainterRep::SetFillStyle(long FgMixMode, long BgMixMode)
{

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) PainterRep::SetFillStyle");
  }
#endif

  areaAttrs.usMixMode = (USHORT)FgMixMode;
  areaAttrsMask |= ABB_MIX_MODE;
  areaAttrs.usBackMixMode = (USHORT)BgMixMode;
  areaAttrsMask |= ABB_BACK_MIX_MODE;
}

void PainterRep::SetFillPattern(HBITMAP hbm)
{

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) PainterRep::SetFillPattern");
  }
#endif

  FillPattern = hbm;
}

void PainterRep::Update(HPS hps)
{

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) PainterRep::Update");
  }
#endif

  if (GpiSetAttrs(hps, 
		  PRIM_LINE, 
		  lineAttrsMask,
		  0L,
		  (PBUNDLE)&lineAttrs)
      == FALSE) {
    ReportError(_world->hab(), "(9) ");
  }
  if (GpiSetAttrs(hps, 
		  PRIM_CHAR,
		  charAttrsMask,
		  0L,
		  (PBUNDLE)&charAttrs)
      == FALSE) {
    ReportError(_world->hab(), "(10) ");
  }
    
  if (GpiSetAttrs(hps, 
		  PRIM_MARKER, 
		  markerAttrsMask,
		  0L,
		  (PBUNDLE)&markerAttrs)
      == FALSE) {
    ReportError(_world->hab(), "(11) ");
  }

  if (GpiSetAttrs(hps, 
		  PRIM_AREA,
		  areaAttrsMask,
		  0L,
		  (PBUNDLE)&areaAttrs)
      == FALSE) {
    ReportError(_world->hab(), "(12) ");
  }
  if (FillPattern) {
    if (GpiSetBitmapId(hps, (HBITMAP)FillPattern, 1) == GPI_ERROR) {
      ReportError(_world->hab(), "(97) ");
    }
    if (GpiSetPatternSet(hps, 1) == GPI_ERROR) {
      ReportError(_world->hab(), "(98) ");
    }
  }

  if (GpiSetAttrs(hps, 
		  PRIM_IMAGE, 
		  imageAttrsMask,
		  0L,
		  (PBUNDLE)&imageAttrs)
      == FALSE) {
    ReportError(_world->hab(), "(13) ");
  }
}
