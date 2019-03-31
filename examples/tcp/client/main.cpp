#include <iostream>
#include <libparsley/tcp_socket.h>
#include <libparsley/timer.h>

#include <uv.h>

using namespace std;
using namespace Parsley;

static TcpSocket *client;

void timeout_cb(Timer *t)
{  
  if(client->write("hello") == 0)
    cout << "written" << endl;
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
  client->connect("127.0.0.1", 63773);
  client->start();

  Timer *timer = new Timer(2000, 500, &loop);
  connect(&timer->onTimedOut, &timeout_cb);
  timer->start();

  return loop.run();
}
