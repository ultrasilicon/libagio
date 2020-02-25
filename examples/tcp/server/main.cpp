#include <libagio/tcp_server.h>
#include <vector>
#include <iostream>

using namespace std;
using namespace Agio;

static Loop loop;


void new_connection_cb(TcpServer* server)
{
  TcpSocket* sock = new TcpSocket(&loop);
  on(&sock->onReadyRead, [](Buffer* data, TcpSocket* sock){
      cout << sock->peerAddress() << ": " << data->toString() << endl;
    });

  server->accept(sock);
}

int main()
{
  TcpServer *server = new TcpServer(&loop);
  server->bind("0.0.0.0", 63773);
  server->listen();

  on(&server->onNewConnection, &new_connection_cb);

  return loop.run();
}
