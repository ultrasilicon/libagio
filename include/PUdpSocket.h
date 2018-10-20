#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "PAbstractSocket.h"


PARSLEY_NAMESPACE_BEGIN
class UdpSocketUtils;
class UdpSocket;


class UdpSocketUtils
    : public PObject<uv_udp_t, UdpSocket>
{
public:
  UdpSocketUtils(Loop *l) : PObject(l){}
protected:
  static void receiveCb(uv_udp_t* handle, ssize_t nread, const uv_buf_t *buf, const sockaddr *addr, unsigned flags);
  static void writtenCb(uv_udp_send_t* req, int status);

};


class UdpSocket
    : public AbstractSocket
    , private UdpSocketUtils
{
public:
  UdpSocket(Loop *l);
  UdpSocket(const char *ip, const int &port, Loop *l);

  void bind(const char *ip, const int &port);
  void start();
  void stop();
  void write(const char *ip, const int &port, Buffer &buf);
  void setBroadcatEnabled(const bool &enabled = true);

};

PARSLEY_NAMESPACE_END
#endif // UDPSOCKET_H
