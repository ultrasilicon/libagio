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
  static void tcpNewConnectionCb(uv_stream_t *handle, int status);
};

class TcpServer
    : protected TcpServerUtils
{
  friend TcpServerUtils;
public:
  TcpServer(Loop *l);
  TcpServer(const char* ip, const int &port, const int &backLog, Loop *l);

  int bind(const char *ip, const int &port);
  int listen();
  int stop();

private:
  std::unordered_set<TcpSocket*> client_set;

  bool accept();
  void onReadyRead(Buffer buf, char* ip);
};

PARSLEY_NAMESPACE_END
#endif // TCPSERVER_H
