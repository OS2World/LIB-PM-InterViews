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
 * TextEditor - basic interactive editor for mulit-line text
 */

#ifndef texteditor_h
#define texteditor_h

#include <InterViews/interactor.h>

class TextDisplay;
class TextBuffer;

class TextEditor : public Interactor {
public:
    TextEditor(long rows, long cols, long tabsize, long highlight);
    TextEditor(const char* name, long r, long c, long t, long h);
    virtual ~TextEditor();

    void Edit(TextBuffer*, long index = 0);

    long Dot();
    long Mark();

    void InsertText(const char*, long);
    void DeleteText(long);
    void DeleteSelection();

    void BackwardCharacter(long = 1),    ForwardCharacter(long = 1);
    void BackwardLine(long = 1),         ForwardLine(long = 1);
    void BackwardWord(long = 1),         ForwardWord(long = 1);
    void BackwardPage(long = 1),         ForwardPage(long = 1);

    void BeginningOfLine(),             EndOfLine();
    void BeginningOfWord(),             EndOfWord();
    void BeginningOfSelection(),        EndOfSelection();
    void BeginningOfText(),             EndOfText();

    void SetScrollAlignment(Alignment);
    Alignment GetScrollAlignment();

    void ScrollToSelection(boolean always = false);
    void ScrollToView(Coord x, Coord y);
    void ScrollBy(Coord dx, Coord dy);
    void GrabScroll(Event&);
    void RateScroll(Event&);
    virtual void Adjust(Perspective&);

    void Select(long dot);
    void SelectMore(long mark);
    void SelectAll();
    void Select(long dot, long mark);

    long Locate(Coord x, Coord y);
protected:
    virtual void Reconfig();
    virtual void Redraw(Coord, Coord, Coord, Coord);
    virtual void Resize();

    TextBuffer* text;
    TextDisplay* display;
private:
    void ScrollTo(long x, long y);
    void Init(long r, long c, long t, long h);

    long dot, mark;
    long tabsize;
    long lineheight;
    long highlight;
    long shaperows;
    long shapecolumns;
    Alignment scrollalign;
};

inline long TextEditor::Dot () { return dot; }
inline long TextEditor::Mark () { return mark; }
inline void TextEditor::SetScrollAlignment (Alignment a) { scrollalign = a; }
inline Alignment TextEditor::GetScrollAlignment() { return scrollalign; }

#endif
