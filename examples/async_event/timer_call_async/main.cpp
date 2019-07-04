#include <iostream>

#include <libagio/async_event.h>
#include <libagio/timer.h>

using namespace std;
using namespace Agio;

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
