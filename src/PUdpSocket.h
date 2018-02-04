#ifndef UDPSOCKET_H
#define UDPSOCKET_H


#include "PAbstractSocket.h"

namespace Parsley {
  class UdpSocketUtils;
  class UdpSocket;


  class UdpSocketUtils
  {
  protected:
    static std::map<int, UdpSocket*> instance_hash;

//    static QHash<int, UdpSocket*> instance_hash;
    static bool registerInstance(UdpSocket *sock);

    static void receiveCb(uv_udp_t* handle, ssize_t nread, const uv_buf_t *buf, const sockaddr *addr, unsigned flags);
    static void writeCb(uv_udp_send_t* req, int status);

  };


  class UdpSocket
      : public AbstractSocket
      , protected UdpSocketUtils
  {
  public:
    SockReadyReadCb read_cb;

    UdpSocket(uv_loop_t *loop);
    UdpSocket(const char *ipAddr, const int &port, uv_loop_t *loop);

    void bind(const char *ipAddr, const int &port);
    void start();
    void stop();
    void write(const char *ipAddr, const int &port, const uv_buf_t *buf);
    void setBroadcatEnabled(const bool &enabled = true);

    uv_udp_t* getSocket() {return udp_socket;}


  protected:
    uv_udp_t* udp_socket;

  };
}

#endif // UDPSOCKET_H
