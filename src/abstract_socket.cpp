#include "abstract_socket.h"

#include <functional>
#include <cstdlib>

using namespace Agio;

AbstractSocket::~AbstractSocket()
{
}

void AbstractSocket::allocCb(uv_handle_t *, size_t size, uv_buf_t *buf)
{
  buf->base = (char*) malloc(size);
  buf->len = size;
}




