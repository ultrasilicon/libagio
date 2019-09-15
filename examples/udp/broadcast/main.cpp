#include <iostream>

#include <libagio/udp_socket.h>
#include <libagio/timer.h>

using namespace std;
using namespace Agio;

static Loop* loop;
static UdpSocket* server;
static Timer* timer;

void send_cb(Timer*)
{
  server->write("255.255.255.255", 44444, "hello");
}

void receive_cb(string& data, IP& ip)
{
  cout << ip.toIPString() << ": " << data << endl;
}

int main()
{
  loop = new Loop();

  server = new UdpSocket("0.0.0.0", 44444, loop);
  on(&server->onReadyRead, &receive_cb);

  timer = new Timer(500, loop);
  on(&timer->onTimedOut, &send_cb);

  server->start();
  timer->start();

  return loop->run();
}
