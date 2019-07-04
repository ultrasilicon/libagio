#include <iostream>

#include <libparsley/async_event.h>
#include <libparsley/timer.h>

using namespace std;
using namespace Parsley;

static Loop* loop;
static AsyncEvent* event;
static Timer* timer;

void timeout(Timer*)
{
  event->send();
}

void hello()
{
  cout << "hello" << endl;
}

int main()
{
  loop = new Loop();

  event = new AsyncEvent(loop);
  on(&event->onCalled, &hello);

  timer = new Timer(500, loop);
  on(&timer->onTimedOut, &timeout);

  timer->start();

  return loop->run();
}
