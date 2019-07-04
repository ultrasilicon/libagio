#include "udp_socket.h"

#include <map>

using namespace Parsley;


void UdpSocket::receiveCb(uv_udp_t *handle, ssize_t nread, const uv_buf_t *buf, const sockaddr *addr, unsigned)
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
      getPHandle(handle)->onReadyRead(data, ip);
    }

  free(buf->base);
  return;
}

void UdpSocket::writtenCb(uv_udp_send_t *req, int status)
{
  int socketDescriptor = Utils::getFd((uv_handle_t*) req->handle);
  getPHandle(req->handle)->onWritten(socketDescriptor);
  free(req->bufs);
  free(req);
}


UdpSocket::UdpSocket(Loop *l)
  : PUvObject(l, this)
{
  uv_udp_init(l->uvHandle(), obj_);
}

UdpSocket::UdpSocket(const char *ip, const int &port, Loop *l)
  : PUvObject(l, this)
{
  uv_udp_init(l->uvHandle(), obj_);

  bind(ip, port);
  start();
  setBroadcatEnabled(true);
}

void UdpSocket::bind(const char *ip, const int &port)
{
  sockaddr_in addr;
  uv_ip4_addr(ip, port, &addr);
  uv_udp_bind(obj_, (const sockaddr*) &addr, UV_UDP_REUSEADDR);
}

int UdpSocket::start()
{
  return uv_udp_recv_start(obj_, allocCb, receiveCb);
}

void UdpSocket::stop()
{
  uv_udp_recv_stop(obj_);
}

void UdpSocket::close()
{
  stop();
  uv_close((uv_handle_t*) obj_, nullptr);
}

void UdpSocket::write(const char *ip, const int &port, const std::string &data)
{
  auto *req = CXX_MALLOC(uv_udp_send_t);
  uv_buf_t buf = uv_buf_init((char*) data.c_str(), data.size());
  struct sockaddr_in addr;
  uv_ip4_addr(ip, port, &addr);
  uv_udp_send(req, obj_, &buf, 1, (const struct sockaddr *)&addr, writtenCb);
}

void UdpSocket::setBroadcatEnabled(const bool &enabled)
{
  uv_udp_set_broadcast(obj_, enabled ? 1 : 0);
}

