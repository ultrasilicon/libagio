#include "PBuffer.h"

using namespace Parsley;

Buffer::Buffer(char *data, const int len, Loop *l)
  : PObject(l)
{
  *uv_handle = uv_buf_init(data, len);
  regInstance(uv_handle, this);
}

Buffer::~Buffer()
{
  free(uv_handle->base);
}

char *Buffer::data()
{
  return uv_handle->base;
}

int Buffer::length()
{
  return uv_handle->len;
}

std::string Buffer::toString()
{
  return std::string(uv_handle->base, uv_handle->len);
}
