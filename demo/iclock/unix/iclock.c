// Iclock is a small clock that also checks your mail.  Written using
// InterViews 2.6. Many ideas were stolen from the mailbox and dclock
// programs supplied with InterViews.
//
// The clock shows hours, minutes, and the day of the month.  When new
// mail arrives the clock is shown in reversed colors (white on black).
// The clock is terminated by typing a `q' inside it.
//
// Option	Resource	Description
//
//   -s         seconds		Display seconds hand.
//  -c n	checkinterval	Check mailbox every `n' seconds.
//
// Iclock runs on Sun-3/4 with SunOS 4.0.3 and X11R4, and perhaps others.
//
// Author:  Dag Bruck, Department of Automatic Control, Lund Institute of
// Technology, Box 118, S-221 00 Lund, SWEDEN (dag@control.lth.se).
//
// History:	1989-06-22	Initial version
//		1990-03-29	Reads mail when starting
//		1990-05-22	Displays day of month


#include "InterViews/canvas.h"
#include "InterViews/event.h"
#include "InterViews/font.h"
#include "InterViews/interactor.h"
#include "InterViews/painter.h"
#include "InterViews/sensor.h"
#include "InterViews/shape.h"
#include "InterViews/world.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


#define lstat stat


static PropertyData props[] = {	// Default values for properties
  {"iclock*geometry", "80x80" },
  {"iclock*font", "*helvetica-bold-r*10*" },
  {"iclock*checkinterval", "60" },
  { nil }
};


static OptionDesc options[] = {	// Options and their default values
  {"-s", "iclock*seconds", OptionValueImplicit, "yes" },
  {"-c", "iclock*checkinterval", OptionValueNext },
  { nil }
};


//int atoi(const char *);


#define XRADIUS (xmax/2)
#define YRADIUS (ymax/2)


// Global functions for reading the system clock and for checking
// the user's mailbox.


void GetTime(int& h, int& m, int& s, int& d)
// Return local time
{
  long t = time(0);
  struct tm local = *localtime(&t);
  h = local.tm_hour;
  m = local.tm_min;
  s = local.tm_sec;
  d = local.tm_mday;
}

// Declaration of class Clock.

class Clock : public Interactor {
public:
  Clock();
  // Initializes clock.
  
  void Redraw(Coord, Coord, Coord, Coord);
  // Redraws clock, e.g., after resizing window,
  // or when clock is inverted to signal new mail.
  
  void Update();
  // Redraws clock hands to reflect current time.  Update() should
  // be called at regular intervals.  Doesn't check for mail.
  
  void Resize();
  // Invoked when canvas (window) changes size.  Clock adapts the size
  // of its shape to fill available space.

  void Reconfig();
  // Initial configuration when clock is mapped.  Reads resources.
  
  void Handle(Event&);
  // Handles Timer and Key events.  Invoking Clock::Run() will normally
  // produce a sequence of timer events that makes the clock work; invokes
  // Update() and CheckMail().  A key event will terminate Clock::Run().
  
  void DrawForeground();
  // Draws clock face and hands.
  
  void DrawHands();
  void EraseHands();
  // Draws/erases hands of clock (seconds hand optional).

  void MakeHand(Coord x[4], Coord y[4], float length, float angle);
  // Create polygon representing a hand.
  
private:
  int hours, minutes, seconds, day;
  // Time and day of month currently displayed by clock.
  
  boolean inverted;
  // Clock is inverted to indicate unread mail.

  boolean draw_seconds;
  // Draw seconds hand?

  int update_interval;
  // Interval between clock updates (seconds).

};

// Implementation of class clock.


Clock :: Clock()
{
  SetClassName("iclock");
  GetTime(hours, minutes, seconds, day);
  inverted = false;
  draw_seconds = false;
  update_interval = 60;
}


void Clock :: Reconfig()
{
  draw_seconds = (GetAttribute("seconds") != nil);
  update_interval = draw_seconds ? 2 : 60;

  output = new Painter(output);

  input = new Sensor;
  input->Catch(KeyEvent);
  input->CatchTimer(update_interval, 0);
}


void Clock :: Resize()
{
  if (shape == nil)
    shape = new Shape;
  shape->Rect(canvas->Width(), canvas->Height());
  shape->Rigid(hfil, hfil, vfil, vfil);

  output->SetOrigin(XRADIUS, YRADIUS);
}


void Clock :: Redraw(Coord, Coord, Coord, Coord)
{
  output->ClearRect(canvas, -XRADIUS, -YRADIUS, XRADIUS+1, YRADIUS+1);
  DrawForeground();
}


void Clock :: Update()
{
  int h, m, s, d;
  GetTime(h, m, s, d);
  EraseHands();
  hours = h; minutes = m; seconds = s; day = d;
  DrawForeground();
}


void Clock :: DrawForeground()
{
  DrawHands();
  
  // Draw 5-minute marks of clock (could use symmetry)
  
  for (float tic = 0; tic < 2*M_PI; tic += M_PI/6) {
    float s = YRADIUS * sin(tic);
    float c = XRADIUS * cos(tic);
    
    output->Line(canvas, round(0.9*c), round(0.9*s),
		 round(0.97*c), round(0.97*s));
  }

  // Print day of month.

  static char s[10];
  sprintf(s, "%d", day);
  Font* fnt = output->GetFont();
  output->Text(canvas, s, -fnt->Width(s)/2, -XRADIUS);
}


void Clock :: MakeHand(Coord x[4], Coord y[4], float length, float angle)
{
  const float base = 0.075;
  register float s = YRADIUS * sin(2*M_PI * angle);
  register float c = XRADIUS * cos(2*M_PI * angle);
  
  x[0] = round(length*c);
  x[1] = round(-base*s); x[2] = round(-base*c); x[3] = round(base*s);
  y[0] = round(length*s);
  y[1] = round(base*c); y[2] = round(-base*s); y[3] = round(-base*c);
}


void Clock :: DrawHands()
{
  Coord x[4], y[4];

  if (draw_seconds) {
    float s = YRADIUS * sin(2*M_PI * (15-seconds) / 60.0);
    float c = XRADIUS * cos(2*M_PI * (15-seconds) / 60.0);
    output->Line(canvas, round(0.8*c), round(0.8*s),
		 round(0.97*c), round(0.97*s));
  }

  float m = minutes + (draw_seconds ? 0 : seconds / 60.0);
  MakeHand(x, y, 0.97,(15-m) / 60.0);
  output->FillPolygon(canvas, x, y, 4);
  
  float h = hours + minutes / 60.0;
  MakeHand(x, y, 0.60, (3-h) / 12.0);
  output->FillPolygon(canvas, x, y, 4);
}


void Clock :: EraseHands()
{
  Color* cf = output->GetFgColor();
  output->SetColors(output->GetBgColor(), nil);

  DrawHands();

  output->SetColors(cf, nil);
}

void Clock :: Handle(Event& e)
{
  static int countdown = 0;

  switch (e.eventType) {
  case TimerEvent:
    Update();
    break;
  case KeyEvent:
    QuitRunning(e);
    break;
  }
}


main (int argc, char* argv[])
{
  World* w = new World("iclock", props, options, argc, argv);
  Clock* clk = new Clock;
  w->InsertApplication(clk);
  clk->Run();
}
