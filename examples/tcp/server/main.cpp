#include <libagio/tcp_server.h>
#include <iostream>

using namespace std;
using namespace Agio;

static Loop loop;


void new_connection_cb(TcpServer* server)
{
  auto readyRead = [](Buffer* data, TcpSocket* sock) {
    cout << sock->peerAddress() << ": " << data->toString() << endl;
  };

  TcpSocket* sock = new TcpSocket(&loop);
  sock->onReadyRead = readyRead;
  server->accept(sock);
}

int main()
{
  TcpServer *server = new TcpServer(&loop);
  server->onNewConnection = &new_connection_cb;
  server->bind("0.0.0.0", 63773);
  server->listen();

  return loop.run();
}



