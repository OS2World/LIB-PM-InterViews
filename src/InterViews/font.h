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
 * Text drawing fonts
 */

#ifndef font_h
#define font_h

#include <InterViews/resource.h>

class Font : public Resource {
public:
    Font(const char*);
    Font(const char*, int);
    ~Font();

    int Baseline();
    boolean FixedWidth();
    int Height();
    int Index(const char*, int offset, boolean between);
    int Index(const char*, int, int offset, boolean between);
    boolean Valid();
    int Width(const char*);
    int Width(const char*, int);
    void* Info();
private:
    friend class Painter;

    class FontRep* rep;

    void GetFontByName(const char*);
    void Init();
    boolean Lookup(const char*, int);
};

class FontRep : public Resource {
    friend class Font;
    friend class Painter;

    void* id;
    void* info;
    int height;

    FontRep() {}
    ~FontRep();
};

extern Font* stdfont;

#endif
