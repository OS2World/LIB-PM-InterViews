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

#include <InterViews/regexp.h>
#include <InterViews/textbuffer.h>
#include <bstring.h>
#include <ctype.h>
#include <memory.h>
#include <string.h>

static const char NEWLINE = '\012';

TextBuffer::TextBuffer (char* t, long l, long s) {
    text = t;
    length = l;
    size = s;
    bzero(text + length, (int)(size - length));
    linecount = 1 + LinesBetween(0, length);
    lastline = 0;
    lastindex = 0;
}

TextBuffer::~TextBuffer () {
    Text();
}

inline long limit (long l, long x, long h) {
    return (x<l) ? l : (x>h) ? h : x;
}

long TextBuffer::Search (Regexp* regexp, long index, long range, long stop) {
    long s = limit(0, stop, length);
    long i = limit(0, index, s);
    return regexp->Search(text, s, i, range);
}

long TextBuffer::BackwardSearch (Regexp* regexp, long index) {
    long i = limit(0, index, length);
    long r = regexp->Search(text, length, i, -i);
    if (r >= 0) {
        return regexp->BeginningOfMatch();
    } else {
        return r;
    }
}

long TextBuffer::ForwardSearch (Regexp* regexp, long index) {
    long i = limit(0, index, length);
    long r = regexp->Search(text, length, i, length - i);
    if (r >= 0) {
        return regexp->EndOfMatch();
    } else {
        return r;
    }
}

long TextBuffer::Match (Regexp* regexp, long index, long stop) {
    long s = limit(0, stop, length);
    long i = limit(0, index, s);
    return regexp->Match(text, length, i);
}

boolean TextBuffer::BackwardMatch (Regexp* regexp, long index) {
    long i = limit(0, index, length);
    for (long j = i; j >= 0; --j) {
        if (regexp->Match(text, length, j) == i - j) {
            return true;
        }
    }
    return false;
}

boolean TextBuffer::ForwardMatch (Regexp* regexp, long index) {
    long i = limit(0, index, length);
    return regexp->Match(text, length, i) >= 0;
}

long TextBuffer::Insert (long index, const char* string, long count) {
    if (index < 0 || index > length) {
        return 0;
    } else if (count < 0) {
        return Insert(index + count, string, -count);
    } else {
        count = min(count, size - length);
        bcopy(text + index, text + index + count, (int)(length - index));
        bcopy(string, text + index, (int)count);
        length += count;
        long newlines = (count == 1)
            ? (*string == NEWLINE)
            : LinesBetween(index, index + count);
        linecount += newlines;
        if (lastindex > index) {
            lastindex += count;
            lastline += newlines;
        }
        return count;
    }
}

long TextBuffer::Delete (long index, long count) {
    if (index < 0 || index > length) {
        return 0;
    } else if (count < 0) {
        return -Delete(index + count, -count);
    } else {
        count = min(count, length - index);
        long oldlines = (count == 1)
            ? (text[index] == NEWLINE)
            : LinesBetween(index, index + count);
        if (lastindex > index + count) {
            lastindex -= count;
            lastline -= oldlines;
        } else if (lastindex >= index) {
            (void)LineNumber(index);
        }
        bcopy(text + index + count, text + index, (int)(length - (index+count)));
        length -= count;
        bzero(text + length, (int)count);
        linecount -= oldlines;
        return count;
    }
}

long TextBuffer::Copy (long index, char* buffer, long count) {
    if (index < 0 || index > length) {
        return 0;
    } else if (count < 0) {
        return Copy(index + count, buffer, -count);
    } else {
        count = min(count, length - index);
        bcopy(text + index, buffer, (int)count);
        return count;
    }
}

long TextBuffer::Width () {
    long width = 0;
    long i = 0;
    while (i != length) {
        width = max(width, EndOfLine(i) - i);
        i = BeginningOfNextLine(i);
    }
    return width;
}

long TextBuffer::LineIndex(long line) {
    long l = (line<0) ? 0 : (line>=linecount) ? linecount-1 : line;
    while (lastline > l) {
        --lastline;
        lastindex = BeginningOfLine(EndOfPreviousLine(lastindex));
    }
    while (lastline < l) {
        ++lastline;
        lastindex = BeginningOfNextLine(lastindex);
    }
    if (line >= linecount) {
        return EndOfText();
    } else {
        return lastindex;
    }
}

long TextBuffer::LinesBetween (long index1, long index2) {
    if (index1 == index2) {
        return 0;
    } else if (index1 > index2) {
        return -LinesBetween(index2, index1);
    } else {
        const char* start = Text(index1);
        const char* finish = Text(index2);
        const char* tt;
        long l = 0;
	while (start < finish) {
	    tt = (char *)memchr(start, (int)NEWLINE, finish - start);
	    if (tt == nil) {
		break;
	    }
            start = tt + 1;
            ++l;
        }
        return l;
    }
}

long TextBuffer::LineNumber (long index) {
    long l = LinesBetween(lastindex, index);
    lastline += l;
    lastindex = BeginningOfLine(index);
    return lastline;
}

long TextBuffer::LineOffset (long index) {
    return (index<0) ? 0 : (index>length) ? 0 : index-BeginningOfLine(index);
}

boolean TextBuffer::IsBeginningOfLine (long index) {
    const char* t = Text(index);
    return t <= text || *(t-1) == NEWLINE;
}

long TextBuffer::BeginningOfLine (long index) {
    const char* t = Text(index);
    while (t > text && *(t-1) != NEWLINE) {
        --t;
    }
    return t - text;
}

long TextBuffer::BeginningOfNextLine (long index) {
    const char* t = Text(index);
    const char* e = text + length;
    t = (const char *)memchr(t, (int)NEWLINE, e - t);
    if (t == nil) {
        return length;
    } else {
        return t - text + 1;
    }
}

boolean TextBuffer::IsEndOfLine (long index) {
    const char* t = Text(index);
    return t >= text + length || *t == NEWLINE;
}

long TextBuffer::EndOfLine (long index) {
    const char* t = Text(index);
    const char* e = text + length;
    t = (const char *)memchr(t, (int)NEWLINE, e - t);
    if (t == nil) {
        return length;
    } else {
        return t - text;
    }
}

long TextBuffer::EndOfPreviousLine (long index) {
    const char* t = Text(index-1);
    while (t > text && *t != NEWLINE) {
        --t;
    }
    return t - text;
}

boolean TextBuffer::IsBeginningOfWord (long index) {
    const char* t = Text(index);
    return t <= text || !isalnum(*(t-1)) && isalnum(*t);
}

long TextBuffer::BeginningOfWord (long index) {
    const char* t = Text(index);
    while (t > text && !(!isalnum(*(t-1)) && isalnum(*t))) {
        --t;
    }
    return t - text;
}

long TextBuffer::BeginningOfNextWord (long index) {
    const char* t = Text(index+1);
    while (t < text+length && !(!isalnum(*(t-1)) && isalnum(*t))) {
        ++t;
    }
    return t - text;
}

boolean TextBuffer::IsEndOfWord (long index) {
    const char* t = Text(index);
    return t >= text+length || isalnum(*(t-1)) && !isalnum(*t);
}

long TextBuffer::EndOfWord (long index) {
    const char* t = Text(index);
    while (t < text+length && !(isalnum(*(t-1)) && !isalnum(*t))) {
        ++t;
    }
    return t - text;
}

long TextBuffer::EndOfPreviousWord (long index) {
    const char* t = Text(index-1);
    while (t > text && !(isalnum(*(t-1)) && !isalnum(*t))) {
        --t;
    }
    return t - text;
}
