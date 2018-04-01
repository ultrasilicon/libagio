#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "PAbstractSocket.h"
#include "PLoop.h"


PARSLEY_NAMESPACE_BEGIN
class UdpSocketUtils;
class UdpSocket;


class UdpSocketUtils
    : public PObject<uv_udp_t, UdpSocket>
{
protected:
  static void receiveCb(uv_udp_t* handle, ssize_t nread, const Buffer *buf, const sockaddr *addr, unsigned flags);
  static void writeCb(uv_udp_send_t* req, int status);

};


class UdpSocket
    : public AbstractSocket
    , protected UdpSocketUtils
{
public:
  SockReadyReadCb read_cb;

  UdpSocket(Loop *l);
  UdpSocket(const char *ipAddr, const int &port, Loop *l);

  void bind(const char *ipAddr, const int &port);
  void start();
  void stop();
  void write(const char *ipAddr, const int &port, const Buffer *buf);
  void setBroadcatEnabled(const bool &enabled = true);

};

PARSLEY_NAMESPACE_END
#endif // UDPSOCKET_H
