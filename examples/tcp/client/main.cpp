#include <libagio/tcp_socket.h>
#include <libagio/timer.h>

#include <iostream>

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
  client->connect("127.0.0.1", 63773);
  on(&client->onConnected, &connected_cb);

  Timer *timer = new Timer(2000, 500, &loop);
  on(&timer->onTimedOut, &timeout_cb);

  client->start();
  timer->start();

  return loop.run();
}
