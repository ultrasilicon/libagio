#include <iostream>
#include <PTcpServer.h>

using namespace std;
using namespace Parsley;

void receive_cb(Buffer data, char* ip)
{
  cout << ip << ": " << data.base;
  free(data.base);
}

int main()
{
  Loop l;

  TcpServer *server = new TcpServer(&l);
  server->bind("127.0.0.1", 63773);
  server->listen();
  connect(&server->onReadyRead, &receive_cb);

  return l.run();
}
