#include "tcp_socket.h"

#include <string.h>

using namespace Agio;


void TcpSocket::writeCb(uv_write_t* handle, int status)
{
  if(status)
      fprintf(stderr, "Write error %s\n", uv_strerror(status));
  freeWriteReq(handle);
}

void TcpSocket::connectCb(uv_connect_t* handle, int status)
{
  getPHandle(reinterpret_cast<uv_tcp_t*>(handle->handle))->onConnected();
}

void TcpSocket::receiveCb(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
{
  if(nread > 0)
    {
      TcpSocket* s = getPHandle(reinterpret_cast<uv_tcp_t*>(handle));
      s->onReadyRead(new Buffer(buf->base, buf->len), s);
      return;
    }
  if(nread < 0)
    {
      if (nread != UV_EOF)
        {
          fprintf(stderr, "%s", uv_err_name(static_cast<int>(nread)));
        }
      uv_close(reinterpret_cast<uv_handle_t*>(handle), nullptr);
    }
  free(buf->base);
}


void TcpSocket::freeWriteReq(uv_write_t* handle)
{
  auto* req = reinterpret_cast<write_req_t*>(handle);
  free(req);
}




TcpSocket::TcpSocket(Loop* l)
  : AgioService(l, this)
{
  uv_tcp_init(loop_->cObject(), obj_);
}

TcpSocket::~TcpSocket()
{
  //! This might cause delay on quit, be careful!
  close();
  delete peer_address_;
}

int TcpSocket::start()
{
  return uv_read_start(reinterpret_cast<uv_stream_t*>(obj_), allocCb, receiveCb);
}

void TcpSocket::stop()
{
  uv_read_stop(reinterpret_cast<uv_stream_t*>(obj_));
}

void TcpSocket::close()
{
  stop();
  uv_close(reinterpret_cast<uv_handle_t*> (obj_), nullptr);
}

int TcpSocket::connect(const char* ip, const int &port)
{
  auto* addr = CXX_MALLOC(sockaddr_in);
  uv_ip4_addr(ip, port, addr);
  auto* connect = CXX_MALLOC(uv_connect_t);

//  if (m_uv_obj->type != UV_TCP)
//    std::cout << "handle type UV_EINVAL"<< std::endl;
//  if (((sockaddr*)addr)->sa_family != AF_INET && ((sockaddr*)addr)->sa_family != AF_INET6)
//    std::cout << "addr type UV_EINVAL"<< std::endl;

  return uv_tcp_connect(connect, obj_, reinterpret_cast<sockaddr*>(addr), &connectCb);
}

int TcpSocket::write(char* data)
{
  auto* req = CXX_MALLOC(write_req_t);
  req->buf = uv_buf_init(data, static_cast<unsigned int>(strlen(data)));
  return uv_write(reinterpret_cast<uv_write_t*>(req)
           , reinterpret_cast<uv_stream_t*>(obj_)
           , &req->buf
           , 1
           , writeCb);
}

int TcpSocket::write(const std::string& data)
{
  auto* req = CXX_MALLOC(write_req_t);
  req->buf = uv_buf_init((char*) data.c_str(), data.size());
  return uv_write(reinterpret_cast<uv_write_t*>(req)
           , reinterpret_cast<uv_stream_t*>(obj_)
           , &req->buf
           , 1
           , writeCb);
}

int TcpSocket::write(Buffer* data)
{
  auto* req = CXX_MALLOC(write_req_t);
  req->buf = *data->cObject(); //! TODO: verify copied
  return uv_write(reinterpret_cast<uv_write_t*>(req)
           , reinterpret_cast<uv_stream_t*>(obj_)
           , &req->buf
           , 1
           , writeCb);
}

void TcpSocket::setKeepAlive(const bool& enabled, const unsigned int& delay)
{
  uv_tcp_keepalive(obj_
                   , enabled ? 1 : 0
                   , delay);
}

const HostAddress* TcpSocket::peerAddress()
{
  if(!peer_address_ || peer_address_->version() == HostAddress::None)
    return retrievePeerAddress();
  return retrievePeerAddress();
}

const HostAddress* TcpSocket::retrievePeerAddress()
{
  sockaddr_storage addr;
  int addrLen;
  if(uv_tcp_getpeername(static_cast<uv_tcp_t*>(obj_), reinterpret_cast<sockaddr*>(&addr), &addrLen) != 0)
    return nullptr;
  peer_address_ = new HostAddress(addr);

  return peer_address_;
}



