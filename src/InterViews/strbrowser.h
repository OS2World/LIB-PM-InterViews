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
 * StringBrowser - a Mac minifinder-like object for perusing and choosing 
 * from list of strings.
 */

#ifndef strbrowser_h
#define strbrowser_h

#include <InterViews/interactor.h>

static const char* SBDone = "\r\t\007\033";

static const char SBFirstString = 'g';
static const char SBLastString = 'G';
static const char SBSelectAll = 'a';
static const char SBUnselectAll = '\177';
static const char SBUnselectAllAlt = '\010';
static const char SBSelectPreviousString = 'p';
static const char SBSelectNextString = 'n';
static const char SBSelectTopString = '<';
static const char SBSelectBottomString = '>';
static const char SBScrollDown = 'j';
static const char SBScrollUp = 'k';
static const char SBPageDown = ' ';
static const char SBPageUp = 'b';
static const char SBHalfPageDown = 'd';
static const char SBHalfPageUp = 'u';

class ButtonState;
class TextDisplay;

class StringBrowser : public Interactor {
public:
    StringBrowser(
        ButtonState*, long rows, long cols, 
        boolean uniqueSel = true, long highlight = Reversed,
        const char* done = SBDone
    );
    StringBrowser(
        const char* name, ButtonState*, long, long,
        boolean = true, long = Reversed, const char* = SBDone
    );
    virtual ~StringBrowser();

    void Browse();
    void Insert(const char*, long index);
    void Append(const char*);
    void Remove(long index);

    long Index(const char*);
    char* String(long);
    long Count();
    void Clear();

    void Select(long index);
    void SelectAll();
    void Unselect(long index);
    void UnselectAll();
    long Selection(long selindex = 0);
    long SelectionIndex(long index);
    long Selections();
    boolean Selected(long index);

    virtual void Handle(Event&);
    virtual void Adjust(Perspective&);
protected:
    void Select(long dot, long mark);
    void Unselect(long dot, long mark);
    void ScrollBy(long, long);
    void ScrollBy(long lines);
    void ScrollTo(long, long);
    void ScrollTo(long index);
    void ScrollToView(Coord, Coord);
    void GrabScroll(Event&);
    void RateScroll(Event&);

    long Locate(Coord, Coord);
    boolean DoubleClicked(Event&);

    virtual boolean HandleChar(char);
    virtual boolean LeftButtonDown(Event&);

    virtual void Reconfig();
    virtual void Redraw(Coord, Coord, Coord, Coord);
    virtual void Resize();
protected:
    long rows;
    long columns;
    boolean uniqueSel;
    boolean singleClick;
    long clickDelay;
    long highlight;
    ButtonState* subject;
    const char* done;
private:
    void Init(ButtonState*, long, long, boolean, long, const char*);
    void InitTextDisplay();
    void UpdateSelection(long dot, long mark, long style);
    void Note(Event&);

    boolean HandleDownEvent(Event&);
    boolean HandleKeyEvent(Event&);
private:
    char** strbuf;
    long strbufsize;
    long strcount;
    char** selbuf;
    long selbufsize;
    long selcount;

    TextDisplay* display;
    long lineheight;
    unsigned long lasttime;
    Coord lastx, lasty;
    long lastdot, lastmark;
    boolean firstResize;
};

inline void StringBrowser::Append (const char* s) { Insert(s, strcount); }
inline long StringBrowser::Count () { return strcount; }
inline long StringBrowser::Selections () { return selcount; }
inline boolean StringBrowser::Selected (long i) {return SelectionIndex(i) >= 0;}

#endif
