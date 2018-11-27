#include "PBuffer.h"

using namespace Parsley;

Buffer::Buffer(char *data, const int len, Loop *l)
  : PObject(l)
{
  *m_uv_handle = uv_buf_init(data, len);
  regInstance(m_uv_handle, this);
}

Buffer::~Buffer()
{
  free(m_uv_handle->base);
}

char *Buffer::data()
{
  return m_uv_handle->base;
}

int Buffer::length()
{
  return m_uv_handle->len;
}

std::string Buffer::toString()
{
  return std::string(m_uv_handle->base, m_uv_handle->len);
}
