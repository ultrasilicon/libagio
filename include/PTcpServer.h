#ifndef TCPSERVER_H
#define TCPSERVER_H

#define TCP_PORT 23232
#define TCP_BACKLOG 128

#include "PTcpSocket.h"

#include <unordered_set>
#include <map>


P_NS_BEGIN
class TcpServerUtils;
class TcpServer;

class TcpServerUtils
    : public PObject<uv_tcp_t, TcpServer>
{
public:
  explicit TcpServerUtils(Loop *l);

protected:
  static void newConnectionCb(uv_stream_t *handle, int status);
};

class TcpServer
    : private TcpServerUtils
{
  friend TcpServerUtils;
public:
  Callback<void, std::string&, std::string&> onReadyRead;

  TcpServer(Loop *l);
  TcpServer(char *m_ip, const int &m_port, Loop *l);
  TcpServer(char *m_ip, const int &m_port, const int &backLog, Loop *l);
  int bind();
  int bind(char *m_ip, const int &m_port);
  int listen();
  int listen(const int &backLog);
  int stop();

private:
  char *m_ip;
  int m_port;
  int m_back_log;
  std::unordered_set<TcpSocket*> m_client_set;

  void accept();
};

P_NS_END
#endif // TCPSERVER_H
