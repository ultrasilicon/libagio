#include "PUdpSocket.h"

#include <map>

using namespace Parsley;


UdpSocketUtils::UdpSocketUtils(Loop *l)
  : PUvObject(l)
{
}

void
UdpSocketUtils::receiveCb(uv_udp_t *handle, ssize_t nread, const uv_buf_t *buf, const sockaddr *addr, unsigned)
{
  /*! DOC: libuv 1.18.1-dev
   *  - The receive callback will be called with nread == 0 and addr == NULL when there is nothing to read,
   *  - and with nread == 0 and addr != NULL when an empty UDP packet is received.
   */
  if(nread != 0 && addr)
    {
//      char senderAddr[17] = { 0 };
//      uv_ip4_name((const struct sockaddr_in*)addr, senderAddr, 16);
//      std::string ip(senderAddr);
      std::string data(buf->base, nread);
      IPAddress ip((sockaddr_storage&) *addr);
      getInstance(handle)->onReadyRead.call(data, ip);
    }

  free(buf->base);
  return;
}

void
UdpSocketUtils::writtenCb(uv_udp_send_t *req, int status)
{
  int socketDescriptor = Utils::getFd((uv_handle_t*) req->handle);
  getInstance(req->handle)->onWritten.call(socketDescriptor);
  free(req->bufs);
  free(req);
}




UdpSocket::UdpSocket(Loop *l)
  : UdpSocketUtils(l)
{
  uv_udp_init(l->uvHandle(), m_uv_obj);
  regInstance(m_uv_obj, this);
}

UdpSocket::UdpSocket(const char *ip, const int &port, Loop *l)
  : UdpSocketUtils(l)
{
  uv_udp_init(l->uvHandle(), m_uv_obj);
  regInstance(m_uv_obj, this);

  bind(ip, port);
  start();
  setBroadcatEnabled(true);
}

void
UdpSocket::bind(const char *ip, const int &port)
{
  sockaddr_in addr;
  uv_ip4_addr(ip, port, &addr);
  uv_udp_bind(m_uv_obj, (const sockaddr*) &addr, UV_UDP_REUSEADDR);
}

int UdpSocket::start()
{
  return uv_udp_recv_start(m_uv_obj, allocCb, receiveCb);
}

void
UdpSocket::stop()
{
  uv_udp_recv_stop(m_uv_obj);
}

void
UdpSocket::write(const char *ip, const int &port, const std::string &data)
{
  auto *req = CXX_MALLOC(uv_udp_send_t);
  uv_buf_t buf = uv_buf_init((char*) data.c_str(), data.size());
  struct sockaddr_in addr;
  uv_ip4_addr(ip, port, &addr);
  uv_udp_send(req, m_uv_obj, &buf, 1, (const struct sockaddr *)&addr, writtenCb);
}

void
UdpSocket::setBroadcatEnabled(const bool &enabled)
{
  uv_udp_set_broadcast(m_uv_obj, enabled ? 1 : 0);
}

