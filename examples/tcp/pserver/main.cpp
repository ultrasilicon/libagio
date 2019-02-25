#include <iostream>
#include <PTcpServer.h>

#include <parsly/parse_engine.h>

using namespace std;
using namespace Parsley;

void receive_cb(string& data, TcpSocket* sock)
{
  cout << sock->peerAddress()->toIPString() << ": " << data << '\n';
}

int main()
{
  Loop loop;

  TcpServer *server = new TcpServer(&loop);
  server->bind("0.0.0.0", 63773);
  server->listen();

  ParseEngine *p = new ParseEngine();

  connect(&server->onReadyRead, &receive_cb);

  return loop.run();
}
