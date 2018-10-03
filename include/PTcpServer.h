#ifndef TCPSERVER_H
#define TCPSERVER_H

#define TCP_PORT 23232
#define TCP_BACKLOG 128

#include "PTcpSocket.h"

#include <unordered_set>
#include <map>


PARSLEY_NAMESPACE_BEGIN
class TcpServerUtils;
class TcpServer;

class TcpServerUtils
    : public PObject<uv_tcp_t, TcpServer>
{
public:
  explicit TcpServerUtils(Loop *l) : PObject(l) {}

protected:
  static void newConnectionCb(uv_stream_t *handle, int status);
};

class TcpServer
    : protected TcpServerUtils
{
  friend TcpServerUtils;
public:
  TcpServer(Loop *l);
  TcpServer(char *ip, const int &port, Loop *l);
  TcpServer(char *ip, const int &port, const int &backLog, Loop *l);

  int bind();
  int bind(char *ip, const int &port);
  int listen();
  int listen(const int &backLog);
  int stop();

  Callback<void, Buffer, char*> onReadyRead;

private:
  std::unordered_set<TcpSocket*> client_set;

//  void onPacketReady(Buffer buf, char* ip);

  char *ip_;
  int port_;
  int back_log_;
  void accept();
};

PARSLEY_NAMESPACE_END
#endif // TCPSERVER_H
