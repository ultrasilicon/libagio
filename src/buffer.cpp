#include "buffer.h"

using namespace Agio;



Buffer::Buffer(const std::string& data)
  : Buffer((char*)data.c_str(), data.size())
{
}

Buffer::Buffer(char* data, const unsigned int& len)
  : AgioObject()
{
  *obj_ = uv_buf_init(data, len);
}

Buffer::~Buffer()
{
  if(obj_->base)
    free(obj_->base);
}

char *Buffer::data() const
{
  return obj_->base;
}

size_t Buffer::length() const
{
  return obj_->len;
}

std::string Buffer::toString() const
{
  return std::string(obj_->base, obj_->len);
}

uv_buf_t* Buffer::uvBuffer()
{
  return obj_;
}
