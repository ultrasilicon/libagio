#include <iostream>
#include <PTcpServer.h>

using namespace std;
using namespace Parsley;

void receive_cb(string &data, string &ip)
{
  cout << ip << ": " << data << '\n';
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
