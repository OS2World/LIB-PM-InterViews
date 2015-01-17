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
 * Sensors describe input events of interest.
 */

#ifndef sensor_h
#define sensor_h

#include <InterViews/event.h>
#include <InterViews/resource.h>

class Sensor : public Resource {
public:
    Sensor();
    Sensor(Sensor*);
    ~Sensor();

    void Catch(EventType);
    void CatchButton(EventType, long);
    void CatchChannel(long);
    void CatchTimer(long, long);
    void Ignore(EventType);
    void IgnoreButton(EventType, long);
    void IgnoreChannel(long);
    void CatchRemote();
    void IgnoreRemote();
    virtual boolean Interesting(Event&);
protected:
    unsigned long mask;
    unsigned long down[8];
    unsigned long up[8];
    static unsigned long channels;
    static long maxchannel;
    boolean timer;
    long sec, usec;

    long ButtonIndex (unsigned long b) { return (b >> 5) & 07; }
    unsigned long ButtonFlag (unsigned long b) { return 1L << (b & 0x1f); }
    void SetButton (unsigned long a[], unsigned long b) {
	a[ButtonIndex(b)] |= ButtonFlag(b);
    }
    void ClearButton (unsigned long a[], unsigned long b) {
	a[ButtonIndex(b)] &= ~ButtonFlag(b);
    }
    boolean ButtonIsSet (unsigned long a[], unsigned long b) {
	return (a[ButtonIndex(b)] & ButtonFlag(b)) != 0;
    }
    void SetMouseButtons (unsigned long a[]) { a[0] |= 0x7; }
    void ClearMouseButtons (unsigned long a[]) { a[0] &= ~0x7; }
    boolean MouseButtons (unsigned long a[]) { return (a[0] & 0x7) != 0; }
private:
    friend class Interactor;
    friend class Scene;
};

extern Sensor* allEvents;
extern Sensor* onoffEvents;
extern Sensor* updownEvents;
extern Sensor* noEvents;

#endif
