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
 * StringEditor - interactive editor for character strings
 */

#ifndef streditor_h
#define streditor_h

#include <InterViews/interactor.h>

static const char* SEDone = "\r\t\007\033";

static const char SEBeginningOfLine = '\001';
static const char SEEndOfLine = '\005';
static const char SESelectAll = '\025';
static const char SESelectWord = '\027';
static const char SEPreviousCharacter = '\002';
static const char SENextCharacter = '\006';
static const char SEDeleteNextCharacter = '\004';
static const char SEDeletePreviousCharacter = '\177';
static const char SEDeletePreviousCharacterAlt = '\010';

class ButtonState;
class TextDisplay;
class TextBuffer;

class StringEditor : public Interactor {
public:
    StringEditor(ButtonState*, const char* sample, const char* done = SEDone);
    StringEditor(
        const char* name, ButtonState*, const char*, const char* = SEDone
    );
    virtual ~StringEditor();

    void Message(const char* text);
    void Select(long);
    void Select(long left, long right);
    void Edit();
    void Edit(const char* text, long left, long right);

    const char* Text();

    virtual void Handle(Event&);
    virtual void Reconfig();
protected:
    virtual boolean HandleChar(char);
    void InsertText(const char*, long);
    void DoSelect(long left, long right);

    virtual void Redraw(Coord, Coord, Coord, Coord);
    virtual void Resize();

    TextBuffer* text;
    long left, right;
    ButtonState* subject;
    const char* done;
    const char* sample;
    char* buffer;
    long size;
    TextDisplay* display;
private:
    void Init(ButtonState*, const char*, const char*);
};

#endif
