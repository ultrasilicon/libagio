#include <iostream>

#include <libParsley/PUdpSocket.h>
#include <libParsley/PTimer.h>

using namespace std;
using namespace Parsley;

static Loop* loop;
static UdpSocket* server;
static Timer* timer;

void send_cb(Timer*)
{
  server->write("255.255.255.255", 44444, "hello");
}

void receive_cb(string& data, IPAddress ip)
{
  cout << ip.toIPString() << ": " << data << '\n';
}

int main()
{
  loop = new Loop();

  server = new UdpSocket("0.0.0.0", 44444, loop);
  connect(&server->onReadyRead, &receive_cb);

  timer = new Timer(500, loop);
  connect(&timer->onTimedOut, &send_cb);

  server->start();
  timer->start();

  return loop->run();
}
