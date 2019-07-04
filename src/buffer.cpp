#include "buffer.h"

using namespace Agio;



Buffer::Buffer(const std::string &data)
  : AgioObject()
{
  *obj_ = uv_buf_init((char*)data.c_str(), data.size());
}

Buffer::Buffer(char *data, const size_t len)
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

int Buffer::length() const
{
  return obj_->len;
}

std::string Buffer::toString() const
{
  return std::string(obj_->base, obj_->len);
}
