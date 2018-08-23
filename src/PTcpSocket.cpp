#include "PTcpSocket.h"


using namespace Parsley;

TcpSocket::TcpSocket(Loop *l)
  : TcpSocketUtils(l)
{
  loop = l;
  uv_tcp_init(loop->uvHandle(), tcp_socket);
  addInstance(uv_handle, this);
}

uv_tcp_t* TcpSocket::getSocket()
{
  return tcp_socket;
}

void TcpSocket::start()
{
  uv_read_start((uv_stream_t*) tcp_socket, allocCb, read);
}

void TcpSocket::close()
{
  uv_close((uv_handle_t*) tcp_socket, nullptr);
}

void TcpSocket::connect(const char *addr, const int &port)
{
//  struct sockaddr_in dest = uv_ip4_addr(addr, port);
//  uv_connect_t *connect = (uv_connect_t*) malloc(sizeof(uv_connect_t));
//  uv_tcp_connect(connect, tcp_socket, dest, on_connect);
}

void
TcpSocket::read(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf)
{
  SocketDescriptor socketDiscriptor = getFd((uv_handle_t*) handle);

  if(nread > 0)
    {
      uv_buf_t buffer = uv_buf_init(buf->base, nread);
//      decodeTcp(QString::fromUtf8(buffer.base, buffer.len), hiveClient);

      return;
    }
  if(nread < 0)
    {
      if (nread != UV_EOF)
        {
//          Log::net(Log::Critical, "TcpSocket::tcpRead()", "TCP Read error: " + QString(uv_err_name(nread)));
        }
//      Log::net(Log::Normal, "TcpSocket::tcpRead()", "Disconnected from discriptor: " + QString::number(socketDiscriptor));
      uv_close((uv_handle_t*) handle, nullptr);
    }

  free(buf->base);
}

void
TcpSocket::write(const uv_buf_t *data)
{
  write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
  req->buf = uv_buf_init(data->base, data->len);
  uv_write((uv_write_t*) req
           , (uv_stream_t*)tcp_socket
           , &req->buf
           , 1
           , writeCb);
}

void TcpSocket::setKeepAlive(const bool &enabled, const int &delay)
{
  uv_tcp_keepalive(tcp_socket
                   , enabled ? 1 : 0
                   , delay);
}

void
TcpSocket::writeCb(uv_write_t *handle, int status)
{
  if(status)
    {
      fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }

  freeWriteReq(handle);
}

void
TcpSocket::freeWriteReq(uv_write_t *handle)
{
  write_req_t *req = (write_req_t*) handle;
  free(req->buf.base);
  free(req);
}


