#include <iostream>

#include <PUdpSocket.h>
#include <PTimer.h>
#include <PFunction.h>

using namespace std;
using namespace Parsley;

static Loop *loop;
static UdpSocket* server;
static int counter = 0;

void send_cb(Timer *)
{
  Buffer* b = new Buffer("hello: " + to_string(++ counter), loop);
  server->write("255.255.255.255", 66666, b);
}

void receive_cb(string& data, IPAddress* ip)
{
  cout << ip->toIPString() << ": " << data << '\n';
}

int main()
{
  loop = new Loop();

  server = new UdpSocket("0.0.0.0", 66666, loop);
  connect(&server->onReadyRead, &receive_cb);
  server->start();

  Timer* timer = new Timer(1, 500, loop);
  connect(&timer->onTimedOut, &send_cb);
  timer->start();

  return loop->run();
}
