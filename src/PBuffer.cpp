#include "PBuffer.h"

using namespace Parsley;



Parsley::Buffer::Buffer(const std::string &data, Loop *l)
  : PUvObject(l)
{
  *m_uv_obj = uv_buf_init((char*)data.c_str(), data.size());
  regInstance(m_uv_obj, this);
}

Buffer::Buffer(char *data, const int len, Loop *l)
  : PUvObject(l)
{
  *m_uv_obj = uv_buf_init(data, len);
  regInstance(m_uv_obj, this);
}

Buffer::~Buffer()
{
  free(m_uv_obj->base);
}

char *Buffer::data()
{
  return m_uv_obj->base;
}

int Buffer::length()
{
  return m_uv_obj->len;
}

std::string Buffer::toString()
{
  return std::string(m_uv_obj->base, m_uv_obj->len);
}
