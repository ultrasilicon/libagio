#include "PAbstractSocket.h"

#include <functional>
#include <cstdlib>

using namespace Parsley;

AbstractSocket::~AbstractSocket()
{
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

void AbstractSocket::allocCb(uv_handle_t *handle, size_t size, uv_buf_t *buf)
{
  P_USED(handle);
  buf->base = (char*) malloc(size);
  buf->len = size;
}




