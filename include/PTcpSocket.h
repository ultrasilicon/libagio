#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "PAbstractSocket.h"
#include "PAddress.h"

P_NS_BEGIN
class TcpSocketUtils;
class TcpSocket;

class TcpSocketUtils
    : public PUvObject<uv_tcp_t, TcpSocket>
{
public:
  typedef struct {
    uv_write_t req;
    uv_buf_t buf;
  } write_req_t;

  Callback<void, const int&> onConnected;

  TcpSocketUtils(Loop *l);

protected:
  static void receiveCb(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf);
  static void writeCb(uv_write_t *handle, int status);
  static void connectCb(uv_connect_s *handle, int status);
  static void freeWriteReq(uv_write_t *handle);
};


class TcpSocket
    : public AbstractSocket
    , public TcpSocketUtils
{
public:
  Callback<void, std::string&, TcpSocket*> onReadyRead;
  Callback<void, const SocketDescriptor&> onWritten;

  TcpSocket(Loop *l);
  ~TcpSocket();
  void start();
  void close();
  void connect(const char* addr, const int &port);
  void write(const std::string &data);
  void setKeepAlive(const bool &enabled, const int &delay);
  const IPAddress *peerAddress();
  const IPAddress *retrievePeerAddress();

protected:
  IPAddress *m_peer_address = nullptr;

};


P_NS_END
#endif // TCPSOCKET_H
