#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "abstract_socket.h"
#include "utils.h"

P_NS_BEGIN

class UdpSocket
    : public AbstractSocket
    , public PUvObject<uv_udp_t, UdpSocket>
{
  static void receiveCb(uv_udp_t* handle, ssize_t nread, const uv_buf_t *buf, const sockaddr *addr, unsigned);
  static void writtenCb(uv_udp_send_t* req, int status);

public:
  CallbackHandler<void(std::string&, IPAddress&)> onReadyRead;
  CallbackHandler<void(const SocketDescriptor&)> onWritten;

  UdpSocket(Loop *l);
  UdpSocket(const char *ip, const int &port, Loop *l);

  void bind(const char *ip, const int &port);
  int start();
  void stop();
  void close();
  void write(const char *ip, const int &port, const std::string &data);
  void setBroadcatEnabled(const bool &enabled = true);

private:
};

P_NS_END
#endif // UDPSOCKET_H
