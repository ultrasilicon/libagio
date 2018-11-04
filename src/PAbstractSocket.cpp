#include "PAbstractSocket.h"

#include <functional>
#include <stdlib.h>

using namespace Parsley;

AbstractSocket::~AbstractSocket()
{
}

void AbstractSocket::write(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf)
{
  P_USED(handle);
  P_USED(nread);
  P_USED(buf);
}

void AbstractSocket::bind(const char *ipAddr, const int &port)
{
  P_USED(ipAddr);
  P_USED(port);
}

void AbstractSocket::start()
{
}

void AbstractSocket::stop()
{
}

void AbstractSocket::allocCb(uv_handle_t *handle, size_t suggestedSize, uv_buf_t *buf)
{
  P_USED(handle);
  buf->base = (char*) malloc(suggestedSize);
  buf->len = suggestedSize;
}

int AbstractSocket::getFd(uv_handle_t* handle)
{
  int fd;
#ifndef Q_OS_WIN
  uv_fileno(handle, &fd);
#else
  uv_fileno(handle, (uv_os_fd_t*)(&fd));
#endif
  return fd;
}



