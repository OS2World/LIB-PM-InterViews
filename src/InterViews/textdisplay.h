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
 * TextDisplay - basic text displaying
 */

#ifndef textdisplay_h
#define textdisplay_h

#include <InterViews/defs.h>

enum CaretStyleOptions {
    NoCaret, DefaultCaret, BarCaret, UnderscoreCaret, OutlineCaret
};

class Painter;
class Canvas;

class TextDisplay {
public:
    TextDisplay(boolean autosized = false);
    ~TextDisplay();

    void Draw(Painter*, Canvas*);
    void LineHeight(Coord height);
    void TabWidth(Coord width);

    void Scroll(long line, Coord x, Coord y);

    void Resize(Coord xmin, Coord ymin, Coord xmax, Coord ymax);
    void Bounds(Coord& xmin, Coord& ymin, Coord& xmax, Coord& ymax);
    void Redraw(Coord l, Coord b, Coord r, Coord t);

    void InsertLinesAfter(long line, long count = 1);
    void InsertLinesBefore(long line, long count = 1);
    void DeleteLinesAfter(long line, long count = 1);
    void DeleteLinesBefore(long line, long count = 1);

    void InsertText(long line, long index, const char*, long count);
    void DeleteText(long line, long index, long count);
    void ReplaceText(long line, const char*, long count);

    void Style(long line1, long index1, long line2, long index2, long style);
    void AddStyle(long line1, long index1, long line2, long index2, long style);
    void RemoveStyle(long line1, long index1, long line2, long index2, long style);

    void Caret(long line, long index);
    void CaretStyle(long);

    long LineNumber(Coord y);
    long LineIndex(long line, Coord x, boolean between = true);

    Coord Width();
    Coord Height();

    Coord Base(long line);
    Coord Top(long line);
    Coord Left(long line, long index);
    Coord Right(long line, long index);
private:
friend class TextLine;

    void Size(long, long);
    TextLine* Line(long, boolean);
    long Index(long);
    void HideCaret();
    void ShowCaret();

    Painter* painter;
    Canvas* canvas;
    boolean autosized;
    Coord xmin, xmax;
    Coord ymin, ymax;
    Coord x0, y0;
    Coord width;
    long lineheight;
    long tabwidth;
    TextLine** lines;
    long maxlines;
    long firstline;
    long lastline;
    long topline;
    long bottomline;
    long widestline;
    long caretline;
    long caretindex;
    long caretstyle;
};

#endif
