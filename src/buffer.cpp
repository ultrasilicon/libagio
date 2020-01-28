#include "buffer.h"

using namespace Agio;



Buffer::Buffer(const std::string& data)
  : Buffer((char*)data.c_str(), data.size())
{
}

Buffer::Buffer(const unsigned int& len)
  : Buffer(CXX_MALLOC_CSTR(len), len)
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

constexpr char& Buffer::operator[](const std::size_t &idx)
{
  return obj_->base[idx];
}

constexpr const char& Buffer::operator[](const std::size_t &idx) const
{
  return obj_->base[idx];
}

constexpr char* Buffer::front() const
{
  return obj_->base;
}

constexpr char* Buffer::back() const
{
  return &obj_->base[obj_->len];
}

constexpr char *Buffer::data() const
{
  return obj_->base;
}

constexpr size_t Buffer::length() const
{
  return obj_->len;
}

constexpr size_t Buffer::size() const
{
  return length();
}

constexpr uv_buf_t* Buffer::uvBuffer() const
{
  return obj_;
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

std::string Buffer::toString() const
{
  return std::string(obj_->base, obj_->len);
}
