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

#include <InterViews/bitmap.h>
#include <InterViews/button.h>
#include <InterViews/cursor.h>
#include <InterViews/font.h>
#include <InterViews/painter.h>
#include <InterViews/sensor.h>
#include <InterViews/shape.h>
#include <InterViews/streditor.h>
#include <InterViews/textbuffer.h>
#include <InterViews/textdisplay.h>

#include <InterViews/Bitmaps/hand.bm>
#include <InterViews/Bitmaps/handMask.bm>
#include <InterViews/Bitmaps/lfast.bm>
#include <InterViews/Bitmaps/lfastMask.bm>
#include <InterViews/Bitmaps/rfast.bm>
#include <InterViews/Bitmaps/rfastMask.bm>

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static const int BUFFERSIZE = 1000;

StringEditor::StringEditor (
    ButtonState* s, const char* samp, const char* done
) {
    Init(s, samp, done);
}

StringEditor::StringEditor (
    const char* name, ButtonState* s, const char* samp, const char* done
) {
    SetInstance(name);
    Init(s, samp, done);
}

void StringEditor::Init (ButtonState* s, const char* samp, const char* d) {
    SetClassName("StringEditor");
    sample = samp;
    size = BUFFERSIZE;
    buffer = new char[size];
    text = new TextBuffer(buffer, 0, size);
    left = 0;
    right = 0;
    subject = s;
    if (subject != nil) {
        subject->Attach(this);
    }
    done = d;
    display = new TextDisplay();
    display->CaretStyle(NoCaret);
    input = new Sensor();
    input->Catch(KeyEvent);
    input->Catch(DownEvent);
    Message(sample);
}

StringEditor::~StringEditor () {
    if (subject != nil) {
        subject->Detach(this);
    }
    delete text;
    delete buffer;
    delete display;
}

static Cursor* handCursor;
static Cursor* leftCursor;
static Cursor* rightCursor;

void StringEditor::Reconfig () {
    if (handCursor == nil) {
        Bitmap hand(
            hand_bits, hand_width, hand_height, hand_x_hot, hand_y_hot
        );
        Bitmap handmask(hand_mask_bits, hand_mask_width, hand_mask_height);
        Bitmap left(
            lfast_bits, lfast_width, lfast_height, lfast_x_hot, lfast_y_hot
        );
        Bitmap leftmask(lfast_mask_bits, lfast_mask_width, lfast_mask_height);
        Bitmap right(
            rfast_bits, rfast_width, rfast_height, rfast_x_hot, rfast_y_hot
        );
        Bitmap rightmask(rfast_mask_bits, rfast_mask_width, rfast_mask_height);

        handCursor = new Cursor(
            &hand, &handmask, output->GetFgColor(), output->GetBgColor()
        );
        leftCursor = new Cursor(
            &left, &leftmask, output->GetFgColor(), output->GetBgColor()
        );
        rightCursor = new Cursor(
            &right, &rightmask, output->GetFgColor(), output->GetBgColor()
        );
    }

    Font* f = output->GetFont();
    shape->Rect(f->Width(sample), f->Height());
    shape->Rigid(hfil, hfil, 0, 0);
    display->LineHeight(f->Height());
}

void StringEditor::Resize () {
    display->Draw(output, canvas);
    display->Resize(0, 0, xmax, ymax);
    Select(left, right);
}

void StringEditor::Redraw (Coord l, Coord b, Coord r, Coord t) {
    display->Draw(output, canvas);
    display->Redraw(l, b, r, t);
}

void StringEditor::Message (const char* t) {
    text->Delete(text->BeginningOfText(), text->Length());
    text->Insert(0, t, strlen(t));
    long bol = text->BeginningOfLine(0);
    long eol = text->EndOfLine(0);
    display->Draw(output, canvas);
    display->ReplaceText(0, text->Text(bol, eol), eol - bol);
    Select(eol);
}

void StringEditor::Select (long l) {
    Select(l, l);
}

void StringEditor::Select (long l, long r) {
    display->Draw(output, canvas);
    long origin = display->Left(0, 0);
    if (display->Left(0, r - 1) < 0) {
        origin += xmax/2 - display->Left(0, r - 1);
    } else if (display->Right(0, r + 1) > xmax) {
        origin += xmax/2 - display->Right(0, r + 1);
    }
    origin = min(0L, max(min(0L, xmax - display->Width()), origin));
    display->Scroll(0, origin, ymax);
    DoSelect(l, r);
}

void StringEditor::DoSelect (long l, long r) {
    display->Draw(output, canvas);
    if (l > r) {
        long tmp = r;
        r = l;
        l = tmp;
    }
    l = max(l, text->BeginningOfLine(left));
    r = min(r, text->EndOfLine(right));
    if (r < left || l > right) {
        if (right > left) {
            display->Style(0, left, 0, right-1, Plain);
        }
        if (r > l) {
            display->Style(0, l, 0, r-1, Reversed);
        }
    } else {
        if (l < left) {
            display->Style(0, l, 0, left-1, Reversed);
        } else if (l > left) {
            display->Style(0, left, 0, l-1, Plain);
        }
        if (r > right) {
            display->Style(0, right, 0, r-1, Reversed);
        } else if (r < right) {
            display->Style(0, r, 0, right-1, Plain);
        }
    }
    left = l;
    right = r;
    if (left == right) {
        display->Caret(0, left);
    } else {
        display->Caret(-1, 0);
    }
}

void StringEditor::Edit () {
    Event e;
    e.target = nil;
    e.eventType = EnterEvent;
    Handle(e);
}

void StringEditor::Edit (const char* t, long l, long r) {
    Message(t);
    Select(l, r);
    Edit();
}

const char* StringEditor::Text () {
    return text->Text();
}

boolean StringEditor::HandleChar (char c) {
    if (strchr(done, c) != nil) {
        if (subject != nil) {
            subject->SetValue(c);
        }
        return true;
    } else {
        switch (c) {
          case SEBeginningOfLine:
            Select(text->BeginningOfLine(left));
            break;
          case SEEndOfLine:
            Select(text->EndOfLine(right));
            break;
          case SESelectAll:
            Select(text->BeginningOfText(), text->EndOfText());
            break;
          case SESelectWord:
            Select(
                text->BeginningOfWord(text->PreviousCharacter(left)), right
            );
            break;
          case SEPreviousCharacter:
            Select(text->PreviousCharacter(left));
            break;
          case SENextCharacter:
            Select(text->NextCharacter(right));
            break;
          case SEDeleteNextCharacter:
            if (left == right) {
                right = text->NextCharacter(right);
            }
            InsertText("", 0);
            break;
          case SEDeletePreviousCharacter:
          case SEDeletePreviousCharacterAlt:
            if (left == right) {
                left = text->PreviousCharacter(left);
            }
            InsertText("", 0);
            break;
          default:
            if (!iscntrl(c)) {
                InsertText(&c, 1);
            }
            break;
        }
        return false;
    }
}

void StringEditor::InsertText (const char* t, long len) {
    display->Draw(output, canvas);
    if (left != right) {
        text->Delete(left, right-left);
        display->DeleteText(0, left, right-left);
    }
    text->Insert(left, t, len);
    display->InsertText(0, left, t, len);
    long l = left;
    while (len > 0) {
        l = text->NextCharacter(l);
        --len;
    }
    Select(l);
}

void StringEditor::Handle (Event& e) {
    boolean done = false;
    display->Draw(output, canvas);
    display->CaretStyle(BarCaret);
    do {
        switch (e.eventType) {
          case KeyEvent:
            if (e.len != 0) {
                done = HandleChar(e.keystring[0]);
            }
            break;
          case DownEvent:
            if (e.target == this) {
                long origin = display->Left(0, 0);
                long width = display->Width();
                if (e.button == LEFTMOUSE) {
                    long start = display->LineIndex(0, e.x);
                    do {
                        if (e.x < 0) {
                            origin = min(0L, origin - e.x);
                        } else if (e.x > xmax) {
                            origin = max(xmax - width, origin - (e.x - xmax));
                        }
                        display->Scroll(0, origin, ymax);
                        DoSelect(start, display->LineIndex(0, e.x));
                        Poll(e);
                    } while (e.leftmouse);
                } else if (e.button == MIDDLEMOUSE) {
                    Cursor* origCursor = GetCursor();
                    SetCursor(handCursor);
                    long x = e.x;
                    do {
                        origin += e.x - x;
                        origin = min(0L, max(min(0L, xmax - width), origin));
                        display->Scroll(0, origin, ymax);
                        x = e.x;
                        Poll(e);
                    } while (e.middlemouse);
                    SetCursor(origCursor);
                } else if (e.button == RIGHTMOUSE) {
                    Cursor* origCursor = GetCursor();
                    long x = e.x;
                    do {
                        origin += x - e.x;
                        origin = min(0L, max(min(0L, xmax - width), origin));
                        display->Scroll(0, origin, ymax);
                        if (e.x - x < 0) {
                            SetCursor(leftCursor);
                        } else {
                            SetCursor(rightCursor);
                        }
                        Poll(e);
                    } while (e.rightmouse);
                    SetCursor(origCursor);
                }
            } else {
                UnRead(e);
                done = true;
            }
            break;
        }
        if (!done) {
            Read(e);
        }
    } while (!done);
    display->CaretStyle(NoCaret);
}
