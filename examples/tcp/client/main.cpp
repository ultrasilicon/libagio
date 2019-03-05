#include <iostream>
#include <libParsley/PTcpSocket.h>
#include <libParsley/PTimer.h>

using namespace std;
using namespace Parsley;

TcpSocket *client;

void timeout_cb(Timer *t)
{
  client->write("hello");
}

int main()
{
  Loop loop;

  client = new TcpSocket(&loop);
  client->connect("127.0.0.1", 63773);
  client->start();

  Timer *timer = new Timer(500, &loop);
  connect(&timer->onTimedOut, &timeout_cb);
  timer->start();

  return loop.run();
}
