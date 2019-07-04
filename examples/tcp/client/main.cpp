#include <iostream>
#include <libagio/tcp_socket.h>
#include <libagio/timer.h>

#include <uv.h>

using namespace std;
using namespace Agio;

static TcpSocket *client;

void timeout_cb(Timer *)
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
  on(&client->onConnected, &connected_cb);
  client->connect("127.0.0.1", 63773);
  client->start();

  Timer *timer = new Timer(2000, 500, &loop);
  on(&timer->onTimedOut, &timeout_cb);
  timer->start();

  return loop.run();
}
