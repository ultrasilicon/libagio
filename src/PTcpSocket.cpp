#include "PTcpSocket.h"


using namespace Parsley;


void TcpSocketUtils::writeCb(uv_write_t *handle, int status)
{
  if(status)
    {
      fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
  freeWriteReq(handle);
}

void TcpSocketUtils::connectCb(uv_connect_s *handle, int status)
{

}

void TcpSocketUtils::receiveCb(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf)
{
  if(nread > 0)
    {
//      Buffer *buffer = new Buffer(buf->base, nread, Loop::defaultLoop());

      TcpSocket *s = getInstance((uv_tcp_t*)handle);
      std::string data(buf->base, nread);
      s->onReadyRead.call(data, s->getPeerAddress());
      return;
    }
  if(nread < 0)
    {
      if (nread != UV_EOF)
        {
          fprintf(stderr, "%s", uv_err_name((int)nread));
        }
//      Log::net(Log::Normal, "TcpSocket::tcpRead()", "Disconnected from discriptor: " + QString::number(socketDiscriptor));
      uv_close((uv_handle_t*) handle, nullptr);
    }

  free(buf->base);
}


void TcpSocketUtils::freeWriteReq(uv_write_t *handle)
{
  write_req_t *req = (write_req_t*) handle;
  free(req->buf.base);
  free(req);
}




TcpSocket::TcpSocket(Loop *l)
  : TcpSocketUtils(l)
{
  regInstance(uv_handle, this);
  uv_tcp_init(loop->uvHandle(), uv_handle);
}

TcpSocket::~TcpSocket()
{
  //! This might cause delay on quit, be careful!
  close();
}

void TcpSocket::start()
{
  uv_read_start((uv_stream_t*) uv_handle, allocCb, receiveCb);
}

void TcpSocket::close()
{
  uv_close((uv_handle_t*) uv_handle, nullptr);
}

void TcpSocket::connect(const char *ip, const int &port)
{
  sockaddr_in addr;
  uv_ip4_addr(ip, port, &addr);
  uv_connect_t *connect = CXX_MALLOC(uv_connect_t);
  uv_tcp_connect(connect, uv_handle, (sockaddr*)&addr, connectCb);
}


void
TcpSocket::write(const uv_buf_t *data)
{
  write_req_t *req = CXX_MALLOC(write_req_t);
  req->buf = uv_buf_init(data->base, data->len);
  uv_write((uv_write_t*) req
           , (uv_stream_t*)uv_handle
           , &req->buf
           , 1
           , writeCb);
}

void TcpSocket::setKeepAlive(const bool &enabled, const int &delay)
{
  uv_tcp_keepalive(uv_handle
                   , enabled ? 1 : 0
                   , delay);
}

std::string& TcpSocket::getPeerAddress()
{
  if(peer_address == "")
    {
      sockaddr_in addr;
      int addrLen;
      uv_tcp_getpeername((uv_tcp_t*)this->uv_handle, (sockaddr*)&addr, &addrLen);
      peer_address = std::string(inet_ntoa(addr.sin_addr));
    }
  return peer_address;
}



