#include <iostream>

#include <libagio/timer.h>

using namespace std;
using namespace Agio;

static int counter = 0;

static Loop *loop;

void timeout(Timer* t)
{
  if(++ counter == 3)
    t->stop();
  cout << counter << endl;
}

int main()
{
  loop = new Loop();

  Timer* timer = new Timer(500, loop);
  on(&timer->onTimedOut, &timeout);
  timer->start();

  return loop->run();
}
