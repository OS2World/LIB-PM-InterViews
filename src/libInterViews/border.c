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
 * Border implementation.
 */

#include <InterViews/border.h>
#include <InterViews/painter.h>
#include <InterViews/shape.h>

Border::Border (long t) {
    thickness = t;
}

Border::Border (const char* name, long t) {
    SetInstance(name);
    thickness = t;
}

Border::Border (Painter* out, long t) : (nil, out) {
    thickness = t;
}

void Border::Redraw (Coord x1, Coord y1, Coord x2, Coord y2) {
    output->FillRect(canvas, x1, y1, x2, y2);
}

HBorder::HBorder (long t) : (t) {
    Init();
}

HBorder::HBorder (const char* name, long t) : (name, t) {
    Init();
}

HBorder::HBorder (Painter* out, long t) : (out, t) {
    Init();
    Reconfig();
}

void HBorder::Init () {
    SetClassName("HBorder");
}

void HBorder::Reconfig () {
    shape->height = thickness;
    shape->Rigid(hfil, hfil, 0, 0);
}

VBorder::VBorder (long t) : (t) {
    Init();
}

VBorder::VBorder (const char* name, long t) : (name, t) {
    Init();
}

VBorder::VBorder (Painter* out, long t) : (out, t) {
    Init();
    Reconfig();
}

void VBorder::Init () {
    SetClassName("VBorder");
}

void VBorder::Reconfig () {
    shape->width = thickness;
    shape->Rigid(0, 0, vfil, vfil);
}
