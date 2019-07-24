#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "tcp_socket.h"
#include "utils.h"

A_NS_BEGIN
class TcpServer;


class TcpServer
    : public AgioService<uv_tcp_t, TcpServer>
{
  static void newConnectionCb(uv_stream_t *handle, int status);

public:
  CallbackHandler<void(TcpServer*)> onNewConnection;

  TcpServer(Loop *l);
  TcpServer(char *ip, const int &port, Loop *l);
  TcpServer(char *ip, const int &port, const int &backLog, Loop *l);
  int bind();
  int bind(char *ip, const int &port);
  int listen();
  int listen(const int &backLog);
  int accept(TcpSocket *client);
  void close();

private:
  char *ip_;
  int port_;
  int back_log_;
};

A_NS_END
#endif // TCPSERVER_H
