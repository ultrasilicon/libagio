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

Buffer::Buffer(const unsigned int& len)
{
  char* data = static_cast<char*>(malloc(len));
  *obj_ = uv_buf_init(data, len);
}

Buffer::~Buffer()
{
  if(obj_->base)
    free(obj_->base);
}

char& Buffer::operator[](const std::size_t &idx)
{
  return obj_->base[idx];
}

const char& Buffer::operator[](const std::size_t &idx) const
{
  return obj_->base[idx];
}

char* Buffer::front() const
{
  return obj_->base;
}

char* Buffer::back() const
{
  return &obj_->base[obj_->len];
}

char *Buffer::data() const
{
  return obj_->base;
}

size_t Buffer::length() const
{
  return obj_->len;
}

size_t Buffer::size() const
{
  return length();
}

std::string Buffer::toString() const
{
  return std::string(obj_->base, obj_->len);
}

uv_buf_t* Buffer::uvBuffer()
{
  return obj_;
}
