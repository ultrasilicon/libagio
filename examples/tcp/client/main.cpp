#include <libagio/tcp_socket.h>
#include <libagio/timer.h>
#include <iostream>

using namespace std;
using namespace Agio;


int main()
{
  Loop loop;

  TcpSocket *client = new TcpSocket(&loop);
  on(&client->onConnected, [](){
      cout << "connected." << endl;
    });

  Timer *timer = new Timer(2000, 500, &loop);
  on(&timer->onTimedOut, [&](){
      if(client->write("hello") == 0)
        cout << "written" << endl;
    });

  client->connect("127.0.0.1", 63773);
  client->start();
  timer->start();

  return loop.run();
}
