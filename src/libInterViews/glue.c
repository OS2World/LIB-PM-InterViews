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
 * All that's necessary for glue is to set up the shape and
 * redraw the background.
 */

#include <InterViews/glue.h>
#include <InterViews/shape.h>

Glue::Glue () {
    Init();
}

Glue::Glue (const char* name) {
    SetInstance(name);
    Init();
}

Glue::Glue (Painter* bg) : (nil, bg) {
    Init();
}

void Glue::Init () {
    SetClassName("Glue");
}

void Glue::Redraw (Coord, Coord, Coord, Coord) {
    /*
     * Don't do any drawing--assume that an ancestor will take care of it,
     * probably through Canvas::SetBackground.
     */
}

HGlue::HGlue (long nat, long str) {
    Init(nat, nat, str);
}

HGlue::HGlue (const char* name, long nat, long str) : (name) {
    Init(nat, nat, str);
}

HGlue::HGlue (long nat, long shr, long str) {
    Init(nat, shr, str);
}

HGlue::HGlue (const char* name, long nat, long shr, long str) : (name) {
    Init(nat, shr, str);
}

HGlue::HGlue (Painter* bg, long nat, long str) : (bg) {
    Init(nat, nat, str);
}

HGlue::HGlue (Painter* bg, long nat, long shr, long str) : (bg) {
    Init(nat, shr, str);
}

void HGlue::Init (long nat, long shr, long str) {
    SetClassName("HGlue");
    shape->width = nat;
    shape->height = 0;
    shape->Rigid(shr, str, vfil, vfil);
}

VGlue::VGlue (long nat, long str) {
    Init(nat, nat, str);
}

VGlue::VGlue (const char* name, long nat, long str) : (name) {
    Init(nat, nat, str);
}

VGlue::VGlue (long nat, long shr, long str) {
    Init(nat, shr, str);
}

VGlue::VGlue (const char* name, long nat, long shr, long str) : (name) {
    Init(nat, shr, str);
}

VGlue::VGlue (Painter* bg, long nat, long str) : (bg) {
    Init(nat, nat, str);
}

VGlue::VGlue (Painter* bg, long nat, long shr, long str) : (bg) {
    Init(nat, shr, str);
}

void VGlue::Init (long nat, long shr, long str) {
    SetClassName("VGlue");
    shape->width = 0;
    shape->height = nat;
    shape->Rigid(hfil, hfil, shr, str);
}
