#include <iostream>
#include <PTcpServer.h>
#include <PAddress.h>
#include <PFunction.h>

using namespace std;
using namespace Parsley;

void receive_cb(string& data, TcpSocket* s)
{
  cout << s->peerAddress()->toIPString() << ": " << data;
  cout << '\n';

}

int main()
{
  Loop l;

  TcpServer *server = new TcpServer(&l);
  server->bind("0.0.0.0", 63773);
  server->listen();
  connect(&server->onReadyRead, &receive_cb);

  return l.run();
}
