#include "buffer.h"

using namespace Parsley;



Parsley::Buffer::Buffer(const std::string &data)
  : PObject()
{
  *obj_ = uv_buf_init((char*)data.c_str(), data.size());
  regInstance(obj_, this);
}

Buffer::Buffer(char *data, const int len)
  : PObject()
{
  *obj_ = uv_buf_init(data, len);
  regInstance(obj_, this);
}

Buffer::~Buffer()
{
  if(obj_->base)
    free(obj_->base);
}

char *Buffer::data()
{
  return obj_->base;
}

int Buffer::length()
{
  return obj_->len;
}

std::string Buffer::toString()
{
  return std::string(obj_->base, obj_->len);
}
