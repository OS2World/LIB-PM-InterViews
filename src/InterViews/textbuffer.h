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
 * TextBuffer - editable text buffer
 */

#ifndef textbuffer_h
#define textbuffer_h

#include <InterViews/defs.h>

class Regexp;

class TextBuffer {
public:
    TextBuffer(char* buffer, long length, long size);
    ~TextBuffer();

    long Search(Regexp* regexp, long index, long range, long stop);
    long ForwardSearch(Regexp* regexp, long index);
    long BackwardSearch(Regexp* regexp, long index);

    long Match(Regexp* regexp, long index, long stop);
    boolean ForwardMatch(Regexp* regexp, long index);
    boolean BackwardMatch(Regexp* regexp, long index);

    virtual long Insert(long index, const char* string, long count);
    virtual long Delete(long index, long count);
    long Copy(long index, char* buffer, long count);

    long Height();
    long Width();
    long Length();

    const char* Text();
    const char* Text(long index);
    const char* Text(long index1, long index2);
    char Char (long index);

    long LineIndex(long line);
    long LinesBetween(long index1, long index2);
    long LineNumber(long index);
    long LineOffset (long index);

    long PreviousCharacter(long index);
    long NextCharacter(long index);

    boolean IsBeginningOfText(long index);
    long BeginningOfText();

    boolean IsEndOfText(long index);
    long EndOfText();

    boolean IsBeginningOfLine(long index);
    long BeginningOfLine(long index);
    long BeginningOfNextLine(long index);

    boolean IsEndOfLine(long index);
    long EndOfLine(long index);
    long EndOfPreviousLine(long index);

    boolean IsBeginningOfWord(long index);
    long BeginningOfWord(long index);
    long BeginningOfNextWord(long index);

    boolean IsEndOfWord(long index);
    long EndOfWord(long index);
    long EndOfPreviousWord(long index);
protected:
    char* text;
    long length;
    long size;
private:
    long linecount;
    long lastline;
    long lastindex;
};

inline char TextBuffer::Char (long i) {
    return (i<0) ? text[0] : (i>length) ? text[length] : text[i];
}
inline const char* TextBuffer::Text () {
    return text;
}
inline const char* TextBuffer::Text (long i) {
    return text + ((i<0) ? 0 : (i>length) ? length : i);
}
inline const char* TextBuffer::Text (long i, long) {
    return text + ((i<0) ? 0 : (i>length) ? length : i);
}
inline long TextBuffer::PreviousCharacter (long i) {
    return (i<=0) ? 0 : i-1;
}
inline long TextBuffer::NextCharacter (long i) {
    return (i>=length) ? length : i+1;
}
inline boolean TextBuffer::IsBeginningOfText (long i) {
    return i <= 0;
}
inline long TextBuffer::BeginningOfText () {
    return 0;
}
inline boolean TextBuffer::IsEndOfText (long i) {
    return i >= length;
}
inline long TextBuffer::EndOfText () {
    return length;
}
inline long TextBuffer::Height () {
    return linecount;
}
inline long TextBuffer::Length () {
    return length;
}

#endif
