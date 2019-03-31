#include <iostream>
#include <string>
#include <vector>
#include <libparsley/tcp_server.h>

using namespace std;
using namespace Parsley;

static Loop loop;
static vector<TcpSocket*> clients;

void receive_cb(string& data, TcpSocket* sock)
{
  cout << sock->peerAddress()->toIPString() << ": " << data << endl;
}

void new_connection_cb(TcpServer* s)
{
  TcpSocket* sock = new TcpSocket(&loop);
  clients.push_back(sock);
  connect(&sock->onReadyRead, &receive_cb);
  s->accept(sock);
}

int main()
{
  TcpServer *server = new TcpServer(&loop);
  server->bind("0.0.0.0", 63773);
  server->listen();
  connect(&server->onNewConnection, &new_connection_cb);

  return loop.run();
}
