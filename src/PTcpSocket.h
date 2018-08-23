#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "PAbstractSocket.h"


PARSLEY_NAMESPACE_BEGIN
class TcpSocketUtils;
class TcpSocket;

class TcpSocketUtils
    : public PObject<uv_tcp_t, TcpSocket>
{
public:
  TcpSocketUtils(Loop *l) : PObject(l){}

  //protected:

};

class TcpSocket
    : protected AbstractSocket
    , protected TcpSocketUtils
{
  typedef struct {
    uv_write_t req;
    uv_buf_t buf;
  } write_req_t;


public:
  TcpSocket(Loop *l);

  void start();
  void close();
  void connect(const char* addr, const int &port);
  void write(const uv_buf_t *data);

  void setKeepAlive(const bool &enabled, const int &delay);

  uv_tcp_t* getSocket();



protected:
  uv_tcp_t* tcp_socket;

  static void read(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf);
  static void writeCb(uv_write_t *handle, int status);
  static void freeWriteReq(uv_write_t *handle);
};


PARSLEY_NAMESPACE_END
#endif // TCPSOCKET_H
