#include <iostream>
#include <../../../src/PTcpServer.h>

using namespace std;
using namespace Parsley;

int main()
{
  Loop l;

  TcpServer *server = new TcpServer(&l);
  server->bind("127.0.0.1", 63773);
  server->listen();

  return l.run();
}
