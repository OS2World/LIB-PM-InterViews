/*
 * Copyright (c) 1989 Stanford University
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

#include "logo.h"
#include <InterViews/world.h>
#include <stdio.h>
#include <stdlib.h>

static PropertyData props[] = {
    { "*rainbow", "0" },
    { nil }
};

static OptionDesc options[] = {
    { "-rainbow", "*rainbow", OptionValueNext },
    { "-bitmap", "*bitmap", OptionValueNext },
    { nil }
};

#if !(defined(DOS) || defined(MSDOS))
extern int _Xdebug;
#endif

int main (int argc, char* argv[]) {
#if !(defined(DOS) || defined(MSDOS))
  _Xdebug = 1;
#endif
    World* world = new World("logo", props, options, argc, argv);
    
    if (argc == 1) {
        world->Insert(new Logo);
    } else {
        fprintf(stderr, "Usage: [-bitmap file] [-rainbow count]\n");
        exit(1);
    }
    world->Run();
    return 0;
}
