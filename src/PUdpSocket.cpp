#include "PUdpSocket.h"

using namespace Parsley;

QHash<int, UdpSocket*> UdpSocketUtils::instance_hash;

bool UdpSocketUtils::registerInstance(UdpSocket *sock)
{
  instance_hash.insert(AbstractSocket::getSocketDescriptor((uv_handle_t*)sock->getSocket()), sock);
}

void
UdpSocketUtils::receiveCb(uv_udp_t *handle, ssize_t nread, const uv_buf_t *buf, const sockaddr *addr, unsigned flags)
{
  /// DOC: libuv 1.18.1-dev
  ///  - The receive callback will be called with nread == 0 and addr == NULL when there is nothing to read,
  ///  - and with nread == 0 and addr != NULL when an empty UDP packet is received.
  if(nread != 0)
    {
      if(addr != NULL)
        {
          char senderAddr[17] = { 0 };
          uv_ip4_name((const struct sockaddr_in*)addr, senderAddr, 16);
          Log::net(Log::Normal, "UvUdpSockUtils::read()", QString("UDP Packet Received From %1").arg(senderAddr));

          uv_buf_t buffer = uv_buf_init(buf->base, nread);
          qDebug()<<buffer.base;

          instance_hash.value(AbstractSocket::getSocketDescriptor((uv_handle_t*) handle))->callReadyRead(buffer.base, senderAddr);
          /// Do callback or what ever
        }
    }
  else
    {
      if(addr != NULL)
        {
          Log::net(Log::Normal, "UvUdpSockUtils::read()", "Empty UDP Packet Received...");
        }
    }

  free(buf->base);
  return;
}

void
UdpSocketUtils::writeCb(uv_udp_send_t *req, int status)
{
  free(req);
}






UdpSocket::UdpSocket(uv_loop_t *loop)
{
  uv_loop = loop;
  udp_socket = (uv_udp_t*) malloc(sizeof(uv_udp_t));
  uv_udp_init(uv_loop, udp_socket);
}

UdpSocket::UdpSocket(const char *ipAddr, const int &port, uv_loop_t *loop)
{
  uv_loop = loop;
  udp_socket = (uv_udp_t*) malloc(sizeof(uv_udp_t));
  uv_udp_init(uv_loop, udp_socket);

  this->bind(ipAddr, port);
  this->start();
  this->setBroadcatEnabled(true);
}

void
UdpSocket::bind(const char *ipAddr, const int &port)
{
  struct sockaddr_in *udpAddr = (sockaddr_in*)malloc(sizeof(sockaddr_in));
  uv_ip4_addr(ipAddr, port, udpAddr);
  uv_udp_bind(udp_socket, (const struct sockaddr*) udpAddr, UV_UDP_REUSEADDR);

  UdpSocketUtils::registerInstance(this);
}

void
UdpSocket::start()
{
  uv_udp_recv_start(udp_socket, allocBuffer, receiveCb);
}

void
UdpSocket::stop()
{
  uv_udp_recv_stop(udp_socket);
}

void
UdpSocket::write(const char *ipAddr, const int &port, const uv_buf_t *buf)
{
  uv_udp_send_t *req = (uv_udp_send_t*)malloc(sizeof(uv_udp_send_t));
  struct sockaddr_in addr;
  uv_ip4_addr(ipAddr, port, &addr);
  uv_udp_send(req, udp_socket, buf, 1, (const struct sockaddr *)&addr, writeCb);

  Log::net(Log::Normal, "UvServer::sendTextMessage()", "message sent");
}

void
UdpSocket::setBroadcatEnabled(const bool &enabled)
{
  uv_udp_set_broadcast(udp_socket, enabled ? 1 : 0);
}

