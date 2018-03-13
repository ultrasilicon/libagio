#include "PAbstractSocket.h"

#include <functional>

using namespace Parsley;

void AbstractSocket::bindCb(const SockDestroyedCb &cb)
{
  destroyed_cb = cb;
}

void AbstractSocket::bindCb(const SockReadyReadCb &cb)
{
  ready_read_cb = cb;
}

bool AbstractSocket::callDestroyed(const SocketDescriptor &sd)
{
  if(destroyed_cb)
    {
      destroyed_cb(sd);
      return true;
    }
  return false;
}

bool AbstractSocket::callReadyRead(const Buffer &data, char *ip)
{
  if(ready_read_cb)
    {
      ready_read_cb(data, ip);
      return true;
    }
  return false;
}

bool AbstractSocket::callWritten(const AbstractSocket::SocketDescriptor &sd)
{
  if(written_cb)
    {
      written_cb(sd);
      return true;
    }
  return false;
}

void AbstractSocket::write(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf)
{

}

void AbstractSocket::bind(const char *ipAddr, const int &port)
{

}

void AbstractSocket::start()
{

}

void AbstractSocket::stop()
{

}

void AbstractSocket::allocBuffer(uv_handle_t *handle, size_t suggestedSize, uv_buf_t *buf)
{
  buf->base = (char*) malloc(suggestedSize);
  buf->len = suggestedSize;
}

int AbstractSocket::getFd(uv_handle_t* handle)
{
  int fd;
#ifdef Q_OS_WIN
  uv_fileno(handle, (uv_os_fd_t*)(&fd));
#else
  uv_fileno(handle, &fd);
#endif
  return fd;
}

//template void UvAbstractSock::bindCb<UvAbstractSock::SockReadyReadCb>(UvAbstractSock::Callback, UvAbstractSock::SockReadyReadCb);

//template<typename T>
//void UvAbstractSock::bindCb(Callback cbType, T cb)
//{
//  switch (cbType) {
//    case Callback::Read:
//      ready_read_cb = cb;
//      break;
//    default:
//      break;
//    }
//}










