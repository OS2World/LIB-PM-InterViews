
/****

 Copyright (C) 1990 Texas Instruments Incorporated.

 Permission is granted to any individual or institution to use, copy, modify,
 and distribute this software, provided that this complete copyright and
 permission notice is maintained, intact, in all copies and supporting
 documentation.

 Texas Instruments Incorporated provides this software "as is" without express
 or implied warranty.

****/

#ifndef PM_lib_h
#define PM_lib_h

#include <defs.h>
#if defined(__cplusplus)
extern "C" {
#endif

#define INCL_GPI
#define INCL_WIN

#include <os2.h>


typedef HBITMAP Pixmap;
typedef POINTL XPoint;

/*
 * Number of bytes to reserve in the window handle for IV.
 */
#define IV_BYTES 10

/*
 * Byte index into the window handle of the window id.
 */
#define IV_ID 0

/*
 * Byte index into thw window handle of the event mask.
 */
#define IV_EVENT_MASK 2

/*
 * Byte index into thw window handle of the do_not_propagate mask.
 */
#define IV_DO_NOT_PROPAGATE_MASK 4

/*
 * Byte index into the window handle of the frame window handle flag.
 */
#define IV_HWND_FRAME 6

/*
 * InterViews generated events.
 *
 *   WM_ENTERNOTIFY is generated when the mouse leaves a window.
 *   WM_LEAVENOTIFY is generated when the mouse enters a window.
 */
#define WM_ENTERNOTIFY WM_USER
#define WM_LEAVENOTIFY WM_USER+1

/*
 * Input Event Masks.  
 *
 * NOTE:  We are using an int to store the mask.  An int may only be 16 bits.
 */
typedef unsigned long Mask;

#define NoEventMask			0L
#define KeyPressMask			(1L<<0)  
#define KeyReleaseMask			(1L<<1)  
#define ButtonPressMask			(1L<<2)  
#define ButtonReleaseMask		(1L<<3)  
#define EnterWindowMask			(1L<<4)  
#define LeaveWindowMask			(1L<<5)  
#define PointerMotionMask		(1L<<6)  
#define PointerMotionHintMask		(1L<<7)  
#define FocusChangeMask			(1L<<8) 
#define OwnerGrabButtonMask		(1L<<9) 
#define SubstructureRedirectMask	0L

/*
 * IV message
 */
typedef struct _IVMSG {
  HWND hwnd;
  USHORT msg;
  MPARAM mp1;
  MPARAM mp2;
  ULONG time;
  LONG x_root, y_root;/* Coordinates relative to root */
  RECTL rcl;          /* Untranslated coordinates set by WM_PAINT messages */
  void  *next;
} IVMSG;
typedef IVMSG *PIVMSG;

/*
 * Data structure used by color operations
 */
class XColor {
public:
  long GetRgb();
  void SetRgb(LONG RgbColor);

  union {
    long rgbColor;
    RGB rgb;
  } u;
  long rgbIndex;
  char flags;  /* do_red, do_green, do_blue */
};

VOID ReportError(HAB hab, char *msg);
VOID PMMessageBox(char* message);

#ifdef DEBUG
boolean debugP(int code);
void debug(char* msg);
void DoPrintMsg(char *text, HWND hwnd, int msg);
#endif

#if defined(__cplusplus)
}
#endif

#endif
