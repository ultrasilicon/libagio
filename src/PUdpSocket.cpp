#include "PUdpSocket.h"

#include <map>

using namespace Parsley;


void
UdpSocketUtils::receiveCb(uv_udp_t *handle, ssize_t nread, const Buffer *buf, const sockaddr *addr, unsigned flags)
{
  /*! DOC: libuv 1.18.1-dev
   *  - The receive callback will be called with nread == 0 and addr == NULL when there is nothing to read,
   *  - and with nread == 0 and addr != NULL when an empty UDP packet is received.
   */
  if(nread != 0)
    {
      if(addr != NULL)
        {
          char senderAddr[17] = { 0 };
          uv_ip4_name((const struct sockaddr_in*)addr, senderAddr, 16);
//          Log::net(Log::Normal, "Parsley::UdpSocketUtils::read()", QString("UDP Packet Received From %1").arg(senderAddr));
          Buffer buffer = uv_buf_init(buf->base, nread);

//          QByteArray a = QByteArray(buffer.base, buffer.len);
//          qDebug()<<a;
          getInstance(handle)->callReadyRead(buffer, senderAddr);
        }
    }
  else
    {
      if(addr != NULL)
        {
//          Log::net(Log::Normal, "Parsley::UdpSocketUtils::read()", "Empty UDP Packet Received...");
        }
    }

  free(buf->base);
  return;
}

void
UdpSocketUtils::writeCb(uv_udp_send_t *req, int status)
{
  int socketDescriptor = AbstractSocket::getFd((uv_handle_t*) req->handle);
  getInstance(req->handle)->callWritten(socketDescriptor);

  free(req);

}




UdpSocket::UdpSocket(Loop *l)
  : UdpSocketUtils(l)
{
  loop = l;
  uv_udp_init(l->uvHandle(), uv_handle);
  addInstance(uv_handle, this);
}

UdpSocket::UdpSocket(const char *ipAddr, const int &port, Loop *l)
  : UdpSocketUtils(l)
{
  loop = l;
  uv_udp_init(l->uvHandle(), uv_handle);
  addInstance(uv_handle, this);

  bind(ipAddr, port);
  start();
  setBroadcatEnabled(true);
}

void
UdpSocket::bind(const char *ipAddr, const int &port)
{
  struct sockaddr_in *udpAddr = (sockaddr_in*)malloc(sizeof(sockaddr_in));
  uv_ip4_addr(ipAddr, port, udpAddr);
  uv_udp_bind(uv_handle, (const struct sockaddr*) udpAddr, UV_UDP_REUSEADDR);

}

void
UdpSocket::start()
{
  uv_udp_recv_start(uv_handle, allocBuffer, receiveCb);
}

void
UdpSocket::stop()
{
  uv_udp_recv_stop(uv_handle);
}

void
UdpSocket::write(const char *ipAddr, const int &port, const Buffer *buf)
{
  uv_udp_send_t *req = (uv_udp_send_t*)malloc(sizeof(uv_udp_send_t));
  struct sockaddr_in addr;
  uv_ip4_addr(ipAddr, port, &addr);
  uv_udp_send(req, uv_handle, buf, 1, (const struct sockaddr *)&addr, writeCb);

//  Log::net(Log::Normal, "Parsley::UdpSocket::sendTextMessage()", "message sent");
}

void
UdpSocket::setBroadcatEnabled(const bool &enabled)
{
  uv_udp_set_broadcast(uv_handle, enabled ? 1 : 0);
}

