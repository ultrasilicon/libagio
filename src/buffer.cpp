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

void Buffer::copy(char* dest, const unsigned int& len) const
{
  memcpy(dest, obj_->base, len);
}

void Buffer::copy(char* dest, char* fromPos, char* toPos) const
{
  memcpy(dest, fromPos, static_cast<unsigned int>(toPos - fromPos));
}

void Buffer::copy(char* dest, char* fromPos, const unsigned int& len) const
{
  memcpy(dest, fromPos, len);
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

uv_buf_t* Buffer::uvBuffer() const
{
  return obj_;
}

std::string Buffer::toString() const
{
  return std::string(obj_->base, obj_->len);
}
