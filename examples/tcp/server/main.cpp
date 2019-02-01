#include <iostream>
#include <PTcpServer.h>
#include <PTimer.h>

using namespace std;
using namespace Parsley;

static Timer *t;
static TcpServer *server;

void receive_cb(string& data, TcpSocket* sock)
{
  cout << sock->peerAddress()->toIPString() << ": " << data << '\n';
}

void timeout_cb(Timer *tm)
{
  server->stop();
  delete server;
  t->stop();
}

int main()
{
  Loop loop;

  server = new TcpServer(&loop);
  connect(&server->onReadyRead, &receive_cb);
  server->bind("0.0.0.0", 63773);
  server->listen();

  t = new Timer(5000, &loop);
  connect(&t->onTimedOut, &timeout_cb);
  t->start();

  return loop.run();
}
