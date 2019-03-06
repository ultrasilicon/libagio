#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "PAbstractSocket.h"


P_NS_BEGIN
class UdpSocketUtils;
class UdpSocket;


class UdpSocketUtils
    : public PUvObject<uv_udp_t, UdpSocket>
{
public:
  UdpSocketUtils(Loop *l);
protected:
  static void receiveCb(uv_udp_t* handle, ssize_t nread, const uv_buf_t *buf, const sockaddr *addr, unsigned);
  static void writtenCb(uv_udp_send_t* req, int status);

};


class UdpSocket
    : public AbstractSocket
    , private UdpSocketUtils
{
public:
  Callback<void, std::string&, IPAddress&> onReadyRead;
  Callback<void, const SocketDescriptor&> onWritten;

  UdpSocket(Loop *l);
  UdpSocket(const char *ip, const int &port, Loop *l);

  void bind(const char *ip, const int &port);
  void start();
  void stop();
  void write(const char *ip, const int &port, const std::string &data);
  void setBroadcatEnabled(const bool &enabled = true);

};

P_NS_END
#endif // UDPSOCKET_H
