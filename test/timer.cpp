// File: timer.cpp

#include "final.h"

//----------------------------------------------------------------------
// class timer
//----------------------------------------------------------------------

class timer : public FWidget
{
 public:
   explicit timer (FWidget* = 0);
 protected:
   void onTimer (FTimerEvent*);
   void onAccel (FAccelEvent*);
   void draw();
};

//----------------------------------------------------------------------
timer::timer (FWidget* parent)
  : FWidget(parent)
{
  addTimer (60000);        // 1-minute timer
  int id = addTimer (50);  // 50-millisecond timer
  addTimer (1000);         // 1-second timer
  delTimer (id);
  addTimer (250);          // 250-millisecond timer

  resetXTermForeground();
  resetXTermBackground();
  wc.term_fg = -1;
  wc.term_bg = -1;
}

//----------------------------------------------------------------------
void timer::onTimer (FTimerEvent* ev)
{
  ::printf("timer event, id %d\n\r", ev->timerId() );
}

//----------------------------------------------------------------------
void timer::onAccel (FAccelEvent* ev)
{
  quit();
  ev->accept();
}

//----------------------------------------------------------------------
void timer::draw()
{
  setNormal();
  setColor(-1,-1);
  clrscr();
  gotoxy (1,1);
  print ("---------------\n");
  print ("Press Q to quit\n");
  print ("---------------\n");
  setCursorPos (1,4);
}


//----------------------------------------------------------------------
//                               main part
//----------------------------------------------------------------------
int main (int argc, char* argv[])
{
  FApplication app(argc, argv);
  timer t(&app);
  t.addAccelerator('q');
  app.setMainWidget(&t);
  t.show();
  return app.exec();
}
