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
 * PM-dependent World representation.
 */

#ifndef worldrep_h
#define worldrep_h

#include <InterViews/PM/PM_lib.h>     // <InterViews/X11/Xlib.h>
#include <InterViews/canvas.h>

class BitmapTable;
class InteractorTable;
class WorldRep;

extern WorldRep* _world;

extern int saved_argc;
extern char** saved_argv;

/*
 * The current WorldView instance is only relevant to
 * window manager applications.
 */
extern class WorldView* _worldview;

enum TxFontsOption {
    TxFontsDefault, TxFontsOff, TxFontsOn, TxFontsCache
};

enum TxImagesOption {
    TxImagesDefault, TxImagesAuto, TxImagesDest, TxImagesSource
};

enum DashOption {
    DashDefault, DashNone, DashThin, DashAll
};

class WorldRep {
public:
    WorldRep();
    HAB hab() { return _hab; }   
    HDC hdc() { return _hdc; }
    HMQ hmq() { return _hmq; }
    HPS hpsMemory() { return _hpsMemory; }
    HWND hwndClient() { return _hwndClient; } 
    HWND hwndFrame() { return _hwndFrame; } 
    HWND root() { return _root; }
    char *szClientClass() { return _szClientClass; }
//    XColormap cmap() { return _cmap; }
    int xor() { return _xor; }
    TxFontsOption txfonts() { return _txfonts; }
    TxImagesOption tximages() { return _tximages; }
    DashOption dash() { return _dash; }
    InteractorTable* itable() { return _itable; }
    void addMessage (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2,
		     POINTL ptl, RECTL rcl);
    void NextMessage(PIVMSG ivmsg);
    void putBackMessage(PIVMSG ivmsg);

    /* for caching transformed fonts */
    BitmapTable* _btable;
    BitmapTable* _txtable;

    ULONG _flFrameFlags;         //Frame window creation flags
    USHORT _windowID;            //Next window id to use for window creation.
    int _savedID;                //Last window id the mouse was over.
    HWND _savedHWND;             //Last window handle the mouse was over.
    MPARAM _savedMP1;
    MPARAM _savedMP2;
private:
    friend class World;
    friend class Scene;

    static char _host[100];

    HWND _root;                  //Root (or Desktop) window
    HAB _hab;                    //Anchor block handle
    HDC _hdc;                    //Device context handle
    HPS _hpsMemory;              //Memory presentation space
    HWND _hwndClient;            //Client area of the frame window
    HWND _hwndFrame;             //Frame window
    CHAR *_szClientClass;        //Client window class name
    QMSG _qmsg;

    /*
     * We are going to use two queues to keep track of PM messages.  
     * The PM message queue and our own IV message queue.  
     * First we try to read from the IV message queue and if empty
     * from the PM message queue.  The idea is to process the PM
     *  messages in a manner similar to X.
     */
    HMQ _hmq;                    //PM message queue handle
    PIVMSG _head;                //Front of IV message queue
    PIVMSG _tail;                //End of IV message queue
//    XColormap _cmap;
    int _xor;
    TxFontsOption _txfonts;
    TxImagesOption _tximages;
    DashOption _dash;
    InteractorTable* _itable;

    int getMessage(PIVMSG ivmsg);
    void removeMessage ();

    char* gethostname();
};

#endif
