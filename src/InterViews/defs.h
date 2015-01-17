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
 * Common definitions for all of InterViews.
 */

#ifndef iv_defs_h
#define iv_defs_h

/*
 * A boolean type can't be an enum because C++ enums should not be
 * promoted to ints.  So, boolean f() { return !g(); } is an error
 * if boolean is an enum.
 */

typedef unsigned boolean;

static const boolean false = 0;
static const boolean true = 1;

/*
 * Alignment needs to be unsigned so that it can be stored
 * as a bit field.
 */

typedef unsigned long Alignment;

static const Alignment TopLeft = 0L;
static const Alignment TopCenter = 1L;
static const Alignment TopRight = 2L;
static const Alignment CenterLeft = 3L;
static const Alignment Center = 4L;
static const Alignment CenterRight = 5L;
static const Alignment BottomLeft = 6L;
static const Alignment BottomCenter = 7L;
static const Alignment BottomRight = 8L;
static const Alignment Left = 9L;
static const Alignment Right = 10L;
static const Alignment Top = 11L;
static const Alignment Bottom = 12L;
static const Alignment HorizCenter = 13L;
static const Alignment VertCenter = 14L;

enum TextStyle {
    Plain = 0x0,
    Boldface = 0x1,
    Underlined = 0x2,
    Reversed = 0x4,
    Outlined = 0x8
};

typedef long Coord;

extern double inch, inches, cm, point, points;
static const int pixels = 1;

#ifndef nil
#define nil 0
#endif

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#if !defined(__cplusplus)
overload min;
overload max;
overload abs;
#endif

inline int min (int a, int b) { return a < b ? a : b; }
inline long min (long a, long b) { return a < b ? a : b; }
inline float min (float a, float b) { return a < b ? a : b; }
inline double min (double a, double b) { return a < b ? a : b; }

inline int max (int a, int b) { return a > b ? a : b; }
inline long max (long a, long b) { return a > b ? a : b; }
inline float max (float a, float b) { return a > b ? a : b; }
inline double max (double a, double b) { return a > b ? a : b; }

inline long abs (long a) { return a < 0L ? -a : a; }

/* inline int round (double x) { return x > 0 ? int(x+0.5) : -int(-x+0.5); } */
inline long round (double x) { return x > 0 ? long(x+0.5) : -long(-x+0.5); }

#endif
