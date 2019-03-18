#include <iostream>
#include <libparsley/PTcpSocket.h>
#include <libparsley/PTimer.h>

#include <uv.h>

using namespace std;
using namespace Parsley;

static TcpSocket *client;

void timeout_cb(Timer *t)
{  
  client->write("hello");
  cout << "written" << endl;
//  t->stop();
}

void connected_cb()
{
  cout << "connected." << endl;
}

int main()
{
  Loop loop;

  client = new TcpSocket(&loop);
  connect(&client->onConnected, &connected_cb);
  cout << uv_strerror(client->connect("127.0.0.1", 63779)) << endl;
  client->start();

  Timer *timer = new Timer(2000, 500, &loop);
  connect(&timer->onTimedOut, &timeout_cb);
  timer->start();

  return loop.run();
}
