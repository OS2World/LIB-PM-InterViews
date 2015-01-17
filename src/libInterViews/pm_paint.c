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
 * PM-dependent paint code
 */

#include <InterViews/bitmap.h>
#include <InterViews/brush.h>
#include <InterViews/color.h>
#include <InterViews/font.h>
#include <InterViews/pattern.h>
#include <InterViews/PM/worldrep.h>
#include <InterViews/PM/PM_lib.h>
#include <InterViews/PM/PM_paint.h>
#include <string.h>
#include <malloc.h>
#include <process.h>
#include <ctype.h>

HBITMAP CreatePattern(char* data);
HBITMAP MakeStipple (int* p);

/*
 * class Brush
 */

static boolean PatternBit (int i, int* pattern, int count) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) PatternBit");
  }
#endif

    boolean bit = true;
    int index = 0;
    while (i >= pattern[index]) {
        i -= pattern[index];
        bit = !bit;
        index = (index + 1) % count;
    }
    return bit;
}

BrushRep::BrushRep (int*, int, int) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("BrushRep::BrushRep");
  }
#endif

  count = 0;
  info = nil;
}

BrushRep::~BrushRep () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("BrushRep::~BrushRep");
  }
#endif

}

/*
 * class Color
 */

ColorRep::ColorRep (ColorIntensity r, ColorIntensity g, ColorIntensity b) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) ColorRep::ColorRep");
  }
#endif

  XColor* c = new XColor;
  c->u.rgb.bRed = (unsigned char)r;
  c->u.rgb.bGreen = (unsigned char)g;
  c->u.rgb.bBlue = (unsigned char)b;

  //X11
  // if (XAllocColor(_world->display(), _world->cmap(), c)) 
    
/*
  if ((c->rgbIndex = GpiQueryColorIndex(_world->hpsMemory(), 
                                        LCOLOPT_REALIZED, 
                                        c->GetRgb())) 
      >= 0) { 
    info = (void*)c;
  } else {
    delete c;
    info = nil;
  }
*/

  if ((c->rgbIndex = GpiQueryNearestColor(_world->hpsMemory(),
					  0L,
					  c->GetRgb())) 
      == GPI_ALTERROR) { 
    ReportError(_world->hab(), "(98) ");
  }
  if ((c->rgbIndex = GpiQueryColorIndex(_world->hpsMemory(), 
					0L,
					c->rgbIndex))
      == QLCT_ERROR) { 
    ReportError(_world->hab(), "(99) ");
  }

  info = (void*)c;
}

ColorRep::ColorRep (
    long p, ColorIntensity& r, ColorIntensity& g, ColorIntensity& b
) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) ColorRep::ColorRep (int");
  }
#endif

    LONG RgbColor;
    XColor* c = new XColor;
    c->rgbIndex = p;

    //X11
    // XQueryColor(_world->display(), _world->cmap(), c);

    RgbColor = GpiQueryRGBColor(_world->hpsMemory(), 
				LCOLOPT_REALIZED, 
				c->rgbIndex);  
    if (RgbColor == GPI_ALTERROR) {
      ReportError(_world->hab(), "(14) ");
    }
    c->SetRgb(RgbColor);
    r = c->u.rgb.bRed;
    g = c->u.rgb.bGreen;
    b = c->u.rgb.bBlue;
    info = (void*)c;
}

/* 
 * New 
 */
LONG colorLookup(const char* name)
{
  char *newName, *j;
  const char *i;

  /*
   * Lowercase string
   */
  newName = (char*)malloc(sizeof(name) + 1);
  if (newName == 0) {
    PMMessageBox("[2]  Out of memory");
    WinTerminate(_world->hab());
    exit(1);
  }  
  j = newName;
  for (i = name; *i != '\0'; i++) {
    *(j++) = (islower(*i)) ? toupper(*i) : *i;
  }
  *j = '\0';

  if (strcmp(newName, "WHITE") == 0) {
    free(newName);
    return CLR_WHITE;
  }
  if (strcmp(newName, "BLACK") == 0) {
    free(newName);
    return CLR_BLACK;
  }
  if (strcmp(newName, "BACKGROUND") == 0) {
    free(newName);
    return CLR_BACKGROUND;
  }
  if (strcmp(newName, "BLUE") == 0) {
    free(newName);
    return CLR_BLUE;
  }
  if (strcmp(newName, "RED") == 0) {
    free(newName);
    return CLR_RED;
  }
  if (strcmp(newName, "PINK") == 0) {
    free(newName);
    return CLR_PINK;
  }
  if (strcmp(newName, "GREEN") == 0) {
    free(newName);
    return CLR_GREEN;
  }
  if (strcmp(newName, "CYAN") == 0) {
    free(newName);
    return CLR_CYAN;
  }
  if (strcmp(newName, "YELLOW") == 0) {
    free(newName);
    return CLR_YELLOW;
  }
  if (strcmp(newName, "NEUTRAL") == 0) {
    free(newName);
    return CLR_NEUTRAL;
  }
  free(newName);
  return(CLR_ERROR);
}

ColorRep::ColorRep (
    const char* name, ColorIntensity& r, ColorIntensity& g, ColorIntensity& b
) 
{

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) ColorRep::ColorRep(const char*");
  }
#endif

  HPS hps;
  XColor* c = new XColor;
  LONG rgbColor;

  //X11
  // if (XParseColor(_world->display(), _world->cmap(), name, c) &&
  //   	XAllocColor(_world->display(), _world->cmap(), c)

  if ((c->rgbIndex = colorLookup(name)) != CLR_ERROR) {
    hps = WinGetPS(_world->root());
    if ((rgbColor = GpiQueryRGBColor(hps, 0L, c->rgbIndex))
	== GPI_ALTERROR) {
      ReportError(_world->hab(), "(32) ");
    }
    c->SetRgb(rgbColor);
    r = c->u.rgb.bRed;
    g = c->u.rgb.bGreen;
    b = c->u.rgb.bBlue;
    info = (void*)c;
    WinReleasePS(hps);
  } else {
    delete c;
    info = nil;
  }
}

ColorRep::~ColorRep () {
}

long ColorRep::GetPixel () {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) ColorRep::GetPixel");
  }
#endif

    XColor* c = (XColor*)info;
    return c->rgbIndex;
}

void ColorRep::GetIntensities (
    ColorIntensity& red, ColorIntensity& green, ColorIntensity& blue
) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) ColorRep::GetIntensities");
  }
#endif

    XColor* c = (XColor*)info;
    red = c->u.rgb.bRed;
    green = c->u.rgb.bGreen;
    blue = c->u.rgb.bBlue;
}

/*
 * class Font
 */

/*
 * Right now we are always going to get the default font.
 */
void Font::GetFontByName (const char*) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Font::GetFontByName");
  }
#endif

   HPS hps;
   FONTMETRICS* Metrics = new FONTMETRICS;

   //X11
   //rep->info = XLoadQueryFont(_world->display(), name);

   rep->info = Metrics;
   hps = WinGetPS(_world->root());
   if (GpiQueryFontMetrics(hps, (LONG) sizeof(*Metrics), Metrics) == FALSE) {
     ReportError(_world->hab(), "(59) ");
   }
  Init();
  WinReleasePS(hps);
  return;
}

inline boolean IsFixedWidth (FONTMETRICS* i) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) IsFixedWidth");
  }
#endif

  return (i->fsType & FM_TYPE_FIXED);
}

void Font::Init () {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Font::Init");
  }
#endif

    //X11
    //register XFontStruct* i = (XFontStruct*)rep->info;
    //if (i != nil) {
    //	rep->id = (void*)i->fid;
    //	rep->height = i->ascent + i->descent;
    //} else {
    //	rep->height = -1;
    //}

    FONTMETRICS* i = (FONTMETRICS*)rep->info;
    if (i != nil) {
//	rep->id = ID_LCID;
	rep->height = (int)(i->lMaxAscender + i->lMaxDescender);
    } else {
	rep->height = -1;
    }
}

Font::~Font () {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Font::~Font");
  }
#endif

    Unref(rep);
}

FontRep::~FontRep () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("FontRep::~FontRep");
  }
#endif

}

int Font::Baseline () {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Font::Baseline");
  }
#endif

  FONTMETRICS* i = (FONTMETRICS*)rep->info;
  return i->lMaxDescender - 1;
}

int Font::Width (const char* s) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Font::Width");
  }
#endif

  HPS hps;
  POINTL ptl;
  POINTL *aptl;
  int width;

  ptl.x = ptl.y = 0;
  aptl = (POINTL*)malloc(sizeof(POINTL) * (strlen(s) + 1));
  if (aptl == 0) {
    PMMessageBox("[3]  Out of memory");
    WinTerminate(_world->hab());
    exit(1);
  }  
  hps = WinGetPS(_world->root());
  if (GpiQueryCharStringPosAt(hps, &ptl, 0L, strlen(s), (PCH)s, (PLONG)0L, aptl)
      == FALSE) {
    ReportError(_world->hab(), "(60) ");
  }
  WinReleasePS(hps);
  width = (int)(aptl[strlen(s)].x);
  free(aptl);
  return width;
}

int Font::Width (const char* s, int len) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Font::Width");
  }
#endif
  HPS hps;
  register const char* p, * q;
  POINTL ptl;
  POINTL *aptl;
  int width;

  q = &s[len];
  for (p = s; *p != '\0' && p < q; p++);
  ptl.x = ptl.y = 0;
  aptl = (POINTL*)malloc(sizeof(POINTL) * (p - s + 1));
  if (aptl == 0) {
    PMMessageBox("[4]  Out of memory");
    WinTerminate(_world->hab());
    exit(1);
  }  
  hps = WinGetPS(_world->root());
  if (GpiQueryCharStringPosAt(hps, &ptl, 0L, p - s, (PCH)s, (PLONG)NULL, aptl)
      == FALSE) {
    ReportError(_world->hab(), "(61) ");
  }
  WinReleasePS(hps);
  width = (int)(aptl[p - s].x);
  free(aptl);
  return width;
}

int Font::Index (const char*, int, int, boolean) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Font::Index");
  }
#endif

  return 0;
}

boolean Font::FixedWidth () {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) Font::FixedWidth");
  }
#endif

  register FONTMETRICS* i = (FONTMETRICS*)rep->info;
  return IsFixedWidth(i);
}

/*
 * class Pattern
 */

/*
 * PM requires that the data for fill patterns be 8x8.  The X version
 * of InterViews assumed that the data was 16x16 and was expanded to
 * 32x32.  This version will assume that the data is 8x8 and is
 * expanded to 32x32.  PM requires that patterns be padded to 32 bits.
 */

static const int PMpatternHeight = 8;
static const int PMpatternWidth = 8;

HBITMAP MakeStipple (int* p) 
{

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) MakeStipple");
  }
#endif
  
  long data[32];
  long i, j;
  unsigned long s1, s2;
  unsigned long src, dst;
  
  for (i = 0; i < PMpatternHeight; i++) {
    dst = 0;
    src = p[i];
    s1 = 1;
    s2 = 1 << (PMpatternWidth - 1);
    for (j = 0; j < PMpatternWidth; j++) {
      if ((s1 & src) != 0) {
	dst |= s2;
      }
      s1 <<= 1;
      s2 >>= 1;
    }
    dst = (dst << 8) | dst;
    data[i] = dst;
    data[i+8] = dst;
  }
  return (HBITMAP)(CreatePattern((char*)data));
}

Pattern::Pattern (Bitmap*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("Pattern::Pattern");
  }
#endif

}

Pattern::Pattern (int p[patternHeight]) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Pattern::Pattern");
  }
#endif

    info = (void*)MakeStipple(p);
}

Pattern::Pattern (int dither) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) Pattern::Pattern (int dither)");
  }
#endif

  if (dither == 0xffff) {
    info = nil;
  } else {
    register int i, seed;
    int r[16];
    
    seed = dither;
    for (i = 0; i < 4; i++) {
      r[i] = (seed & 0xf000) >> 12;
      r[i] |= r[i] << 4;
      r[i] |= r[i] << 8;
      seed <<= 4;
      r[i+4] = r[i];
      r[i+8] = r[i];
      r[i+12] = r[i];
    }
    info = (void*)MakeStipple(r);
  }
}

Pattern::~Pattern () {
  if (info != nil) {
    free((HBITMAP)info);
  }
}

/* New functions */

HBITMAP CreatePattern(char* data)
{
  BITMAPINFOHEADER bmp;
  PBITMAPINFO pbm;
  CHAR cbuffer[80];
  HBITMAP hbm;
  
  hbm = new(HBITMAP);
  bmp.cbFix = sizeof(bmp);
  bmp.cx = 8;
  bmp.cy = 8;
  bmp.cPlanes = 1;
  bmp.cBitCount = 1;

  pbm = (PBITMAPINFO)cbuffer;
  pbm->cbFix = sizeof(bmp);
  pbm->cx = 8;
  pbm->cy = 8;
  pbm->cPlanes = 1;
  pbm->cBitCount = 1;

  pbm->argbColor[0].bRed = 0;  
  pbm->argbColor[0].bGreen = 0;  
  pbm->argbColor[0].bBlue = 0;  
  pbm->argbColor[1].bRed = 255;  
  pbm->argbColor[1].bGreen = 255;  
  pbm->argbColor[1].bBlue = 255;  

  if ((hbm = GpiCreateBitmap(_world->hpsMemory(), 
			     &bmp, CBM_INIT, (PBYTE)data, pbm))
      == (HBITMAP)GPI_ERROR) {
    ReportError(_world->hab(), "(96) ");
  }
  return (hbm);
}
