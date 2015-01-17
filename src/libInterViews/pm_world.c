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
 * PM-dependent world code
 */

#include "btable.h"
#include "itable.h"
#include <InterViews/bitmap.h>
#include <InterViews/canvas.h>
#include <InterViews/color.h>
#include <InterViews/font.h>
#include <InterViews/painter.h>
#include <InterViews/interactor.h>
#include <InterViews/shape.h>
#include <InterViews/world.h>
#include <InterViews/PM/worldrep.h>
#include <InterViews/PM/PM_lib.h>
#include <InterViews/PM/PM_world.h>
#include <InterViews/PM/canvasrep.h>

// * Both os2def.h and stdio.h define NULL.  os2def.h is included by PM_lib.h.

#ifdef NULL
#undef NULL
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void InitSensors(), InitCursors(Color*, Color*);
extern "C" {
MRESULT EXPENTRY PMGetEvent( HWND, USHORT, MPARAM, MPARAM);
}

WorldRep* _world;

int saved_argc;
char** saved_argv;

#if defined(hpux)
#include <sys/utsname.h>
#endif

char* WorldRep::gethostname () {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) WorldRep::gethostname");
  }
#endif

    strcpy(_host, "NONAME");
    return _host;
}

void World::Init (const char*) {
    rep = new WorldRep;

    rep->_szClientClass = "PM";
    rep->_flFrameFlags = FCF_TITLEBAR | FCF_SYSMENU       | FCF_SIZEBORDER |
                         FCF_MINMAX   | FCF_SHELLPOSITION | FCF_TASKLIST;

    /* 
     * Initialize PM and create a PM message queue.
     */
    rep->_hab = WinInitialize((int)NULL);
    rep->_hmq = WinCreateMsgQueue(rep->hab(), 0);  
    if (rep->hab() == (HMQ)NULL) {
        WinTerminate(rep->_hab);
	fprintf(stderr, "Error Initializing PM\n");
	exit(1);
    }
    WinRegisterClass(rep->hab(),
		     (PSZ)(rep->_szClientClass),
		     PMGetEvent,
		     CS_SIZEREDRAW | CS_CLIPSIBLINGS,
		     IV_BYTES);
    /*
     * Open memory DC and create PS associated with it.  This is used 
     * for bitmaps.
     */
    rep->_hdc = DevOpenDC(rep->hab(),
			  OD_MEMORY,
			  (PSZ)"*",
			  8L,
			  (PDEVOPENDATA)pszDcData,
                          (HDC)NULL
                          );
    if ((int)rep->_hdc == DEV_ERROR) {
      ReportError(rep->_hab, "(22) ");
    }

    SIZEL sizl;
    sizl.cx = 0;
    sizl.cy = 0;
    rep->_hpsMemory = GpiCreatePS(rep->_hab,
				  rep->_hdc,
				  &sizl,
				  PU_PELS      | 
				  GPIF_DEFAULT |
				  GPIT_MICRO   |
				  GPIA_ASSOC);
    if ((int)rep->_hpsMemory == GPI_ERROR) {
      ReportError(rep->_hab, "(23) ");
    }

    rep->_root = HWND_DESKTOP;
//    rep->_cmap = DefaultColormap(rep->display(), rep->screen());
    rep->_xor = 1;
    rep->_txfonts = TxFontsDefault;
    rep->_tximages = TxImagesDefault;
    rep->_dash = DashDefault;
    rep->_windowID = (int)HWND_DESKTOP + 1;
    rep->_savedID = -1;
    rep->_savedHWND = (HWND)-1;
    rep->_savedMP1 = rep->_savedMP2 = (MPARAM)0;
    SetCurrent();
    rep->_itable = new InteractorTable(2048);
    rep->_itable->Insert((void*)rep->root(), this);
    rep->_btable = nil;
    rep->_txtable = nil;
    canvas = new Canvas((void*)rep->root());
    if ((canvas->width = WinQuerySysValue(rep->root(), SV_CXSCREEN)) == 0) {
      ReportError(rep->_hab, "(24) ");
    }
    if ((canvas->height = WinQuerySysValue(rep->root(), SV_CYSCREEN)) == 0) {
      ReportError(rep->_hab, "(25) ");
    }
    canvas->status = CanvasMapped;
    xmax = canvas->width - 1;
    ymax = canvas->height - 1;

    HPS _hps;
    HDC _hdc;
    LONG xPixelsPerMeter;

    if ((_hps = WinGetPS(rep->root())) == HDC_ERROR) {
      ReportError(rep->_hab, "(26) ");
    }

    if ((_hdc = GpiQueryDevice( _hps )) == HDC_ERROR) {
      ReportError(rep->_hab, "(27) ");
    }

    if (DevQueryCaps(_hdc, CAPS_HORIZONTAL_RESOLUTION, 1L, &xPixelsPerMeter)
	== FALSE) {
      ReportError(rep->_hab, "(28) ");
    }

    WinReleasePS(_hps);
    double pixmm = (float)xPixelsPerMeter / 1000.0;
    cm = round(10*pixmm);
    inch = round(25.4*pixmm);
    inches = inch;
    point = inch/72.27;
    points = point;

    black = new Color("black");
    if (!black->Valid()) {
        Unref(black);
//        black = new Color((int)BlackPixel(rep->display(), rep->screen()));
    }
    black->Reference();
    white = new Color("white");
    if (!white->Valid()) {
        Unref(white);
//        white = new Color((int)WhitePixel(rep->display(), rep->screen()));
    }
    white->Reference();
    stdfont = new Font("fixed");
    stdfont->Reference();
    stdpaint = new Painter();
    stdpaint->Reference();
}

static inline unsigned int MSB (unsigned long i) {
    return (i ^ (i>>1)) & i;
}

void World::FinishInit () {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) World::FinishInit");
  }
#endif

/*
    char buffer[256];
    const char* xorpixel = GetAttribute("RubberbandPixelHint");
    if (xorpixel != nil) {
        sprintf(buffer, "RubberbandPixel:%s", xorpixel);
        SetHint(buffer);
    } else if (rep->visual()->c_class != DirectColor) {
        rep->_xor = black->PixelValue() ^ white->PixelValue();
    } else {
        rep->_xor = (
            MSB(rep->visual()->red_mask) |
            MSB(rep->visual()->green_mask) |
            MSB(rep->visual()->blue_mask)
        );
    }
    const char* txfonts = GetAttribute("TransformFontsHint");
    if (txfonts != nil) {
        sprintf(buffer, "TransformFonts:%s", txfonts);
        SetHint(buffer);
    }
    const char* tximages = GetAttribute("TransformImagesHint");
    if (tximages != nil) {
        sprintf(buffer, "TransformImages:%s", tximages);
        SetHint(buffer);
    }
    const char* dash = GetAttribute("DashHint");
    if (dash != nil) {
        sprintf(buffer, "Dash:%s", dash);
        SetHint(buffer);
    }
*/
    SetCurrent();

    RootConfig();
    InitSensors();
    InitCursors(output->GetFgColor(), output->GetBgColor());
}

World::~World () {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) World::~World");
  }
#endif

    //X11
    //XCloseDisplay(rep->display());

    WinDestroyWindow( rep->hwndFrame() );
    WinDestroyMsgQueue( rep->hmq() );    
    WinTerminate( rep->hab() );          

    delete rep->_itable;
    delete rep;
}

void World::SaveCommandLine (int argc, char* argv[]) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) World::SaveCommandLine");
  }
#endif

    saved_argc = argc;
    saved_argv = new char*[argc + 1];

    for (int i = 0 ; i < argc ; i++) {
	saved_argv[i] = argv[i];
    }
    saved_argv[i] = nil;
}

const char* World::UserDefaults () {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) World::UserDefaults");
  }
#endif

    //return rep->display()->xdefaults;
    return nil;
}

//Done
void World::FinishInsert (Interactor*) {
    /* nothing else to do */
}

void World::DoChange (Interactor* i) {

#ifdef DEBUG
  if (debugP(4)) {
    debug("(Not Done) World::DoChange");
  }
#endif

    Canvas* c = i->canvas;
    if (c != nil && c->status == CanvasMapped) {
	Shape* s = i->GetShape();
	if (c->width != s->width || c->height != s->height) {

            //X11
	    //XResizeWindow(rep->display(), (Window)c->id, 
	    //		    s->width, s->height); 

	    if (WinSetWindowPos((HWND)c->id,  //Window handle
				(HWND)NULL,   //Relative window-placement order
				0,            //Window coordinates
				0,            //  Ignored
				(SHORT)s->width,     //Window size
				(SHORT)s->height,       
				SWP_SIZE)     //Change the window size
		 == FALSE) {
	      ReportError(rep->_hab, "(29) ");
	    }
	} else {
	    i->Resize();
	}
    }
}

void World::DoRemove (Interactor*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("World::DoRemove");
  }
#endif

}

int World::Fileno () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("World::Fileno");
  }
#endif

  return 0;
}

void World::SetCurrent () {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) World::SetCurrent");
  }
#endif

    _world = rep;
}

void World::SetRoot (void* r) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) World::SetRoot");
  }
#endif

  rep->_root = (HWND)r;
}

//Done
void World::SetScreen (int n) {
  /* Do nothing for PM */
}

int World::NPlanes () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("World::NPlanes");
  }
#endif

    return 0;
}

int World::NButtons () {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) World::NButtons");
  }
#endif

    return 2;
}

int World::PixelSize () {

#ifdef DEBUG
  if (debugP(3)) {
    debug("World::PixelSize");
  }
#endif

    return 0;
}

const char* World::GetDefault (const char* name) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) World::GetDefault");
  }
#endif

    return GetAttribute(name);
}

const char* World::GetGlobalDefault (const char* name) {

#ifdef DEBUG
  if (debugP(5)) {
    debug("(Done) World::GetGlobalDefault");
  }
#endif

    return GetAttribute(name);
}

unsigned long World::ParseGeometry (
    const char*, Coord&, Coord&, unsigned long&, unsigned long&
) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("World::ParseGeometry");
  }
#endif

  return 0;
}

static void FlushCharBitmaps() {

#ifdef DEBUG
  if (debugP(3)) {
    debug("FlushCharBitmaps");
  }
#endif

}

void World::SetHint (const char*) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("World::SetHint");
  }
#endif

}

void World::RingBell (int) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("World::RingBell");
  }
#endif

}

void World::SetKeyClick (int) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("World::SetKeyClick");
  }
#endif

}

void World::SetAutoRepeat (boolean) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("World::SetAutoRepeat");
  }
#endif

}

void World::SetFeedback (int, int) {

#ifdef DEBUG
  if (debugP(3)) {
    debug("World::SetFeedback");
  }
#endif

}

/*
 * Everything below is new.
 */

/*
 * The IV message queue is a FIFO queue with a header node.
 */

//Done
WorldRep::WorldRep() {
  _head = new(IVMSG);
  _head->next = 0;
  _tail = _head;
}

/*
 * Add a PM message to the end of the IV message queue.
 */

//Done
void WorldRep::addMessage (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2,
			   POINTL ptl, RECTL rcl)
{
  PIVMSG tmpPtr;

  tmpPtr = new(IVMSG);
  tmpPtr->hwnd = hwnd;
  tmpPtr->msg = msg;
  tmpPtr->mp1 = mp1;
  tmpPtr->mp2 = mp2;
  tmpPtr->x_root = ptl.x;
  tmpPtr->y_root = ptl.y;
  tmpPtr->rcl.xLeft = rcl.xLeft;
  tmpPtr->rcl.yBottom = rcl.yBottom;
  tmpPtr->rcl.xRight = rcl.xRight;
  tmpPtr->rcl.yTop = rcl.yTop;
  tmpPtr->next = 0;

  _tail->next = tmpPtr;
  _tail = tmpPtr;
}

/*
 * Return the next PM message from the IV message queue.  The message
 * is removed from the queue.  Return 0 if the queue is empty.
 */

//Done
int WorldRep::getMessage(PIVMSG ivmsg)
{
  if (_head->next != 0) {
    ivmsg->hwnd = ((PIVMSG)(_head->next))->hwnd;
    ivmsg->msg  = ((PIVMSG)(_head->next))->msg;
    ivmsg->mp1  = ((PIVMSG)(_head->next))->mp1;
    ivmsg->mp2  = ((PIVMSG)(_head->next))->mp2;
    ivmsg->x_root = ((PIVMSG)(_head->next))->x_root;
    ivmsg->y_root = ((PIVMSG)(_head->next))->y_root;
    ivmsg->rcl.xLeft = ((PIVMSG)(_head->next))->rcl.xLeft;
    ivmsg->rcl.yBottom = ((PIVMSG)(_head->next))->rcl.yBottom;
    ivmsg->rcl.xRight = ((PIVMSG)(_head->next))->rcl.xRight;
    ivmsg->rcl.yTop = ((PIVMSG)(_head->next))->rcl.yTop;

    ivmsg->next = 0;
    removeMessage();
    return 1;
  } else {
    return 0;
  }
}

/*
 * Get the next message from either the IV queue or PM queue.
 * If WinGetMsg returns FALSE then we are going to shutdown PM.
 */

//Done
void WorldRep::NextMessage(PIVMSG ivmsg)
{
  while (!getMessage(ivmsg)) {
    if ( WinGetMsg( hab(), &_qmsg, (HWND)NULL, 0, 0 ) ) {
      WinDispatchMsg( hab(), &_qmsg );
    } else {  
      WinDestroyWindow( hwndFrame() );
      WinDestroyMsgQueue( hmq() );    
      WinTerminate( hab() );          
      exit(0);
    }
  }
}

/*
 * Put back a PM message to the front of the IV message queue.
 */

//Done
void WorldRep::putBackMessage (PIVMSG ivmsg)
{
  PIVMSG tmpPtr;

  tmpPtr = new(IVMSG);
  tmpPtr->hwnd = ivmsg->hwnd;
  tmpPtr->msg = ivmsg->msg;
  tmpPtr->mp1 = ivmsg->mp1;
  tmpPtr->mp2 = ivmsg->mp2;
  tmpPtr->x_root = ivmsg->x_root;
  tmpPtr->y_root = ivmsg->y_root;
  tmpPtr->rcl.xLeft = ivmsg->rcl.xLeft;
  tmpPtr->rcl.yBottom = ivmsg->rcl.yBottom;
  tmpPtr->rcl.xRight = ivmsg->rcl.xRight;
  tmpPtr->rcl.yTop = ivmsg->rcl.yTop;
  tmpPtr->next = _head->next;
  _head->next = tmpPtr;
}

/*
 * Remove a PM message from the IV message queue.
 */

//Done
void WorldRep::removeMessage ()
{
  PIVMSG tmpPtr;

  if (_head->next != 0) {
    tmpPtr = (PIVMSG)((PIVMSG)(_head->next))->next;
    delete((PIVMSG)(_head->next));
    _head->next = tmpPtr;
    if (_head->next == 0) {
      _tail = _head;
    }
  }
}
