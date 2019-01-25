#include <iostream>

#include <PUdpSocket.h>
#include <PTimer.h>
#include <PFunction.h>

using namespace std;
using namespace Parsley;

static Loop *loop;
static UdpSocket* server;
static Timer* timer;
static int counter = 0;

void send_cb(Timer *)
{
  if(counter == 100)
    {
      timer->stop();
      server->stop();
    }
  server->write("255.255.255.255", 66666, "hello: " + to_string(++ counter));
}

void receive_cb(string& data, IPAddress ip)
{
  cout << ip.toIPString() << ": " << data << '\n';
}

int main()
{
  loop = new Loop();

  server = new UdpSocket("0.0.0.0", 66666, loop);
  connect(&server->onReadyRead, &receive_cb);
  server->start();

  timer = new Timer(1, 5, loop);
  connect(&timer->onTimedOut, &send_cb);
  timer->start();

  return loop->run();
}
