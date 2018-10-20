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

}

std::string Buffer::toString()
{
  return std::string(uv_handle->base, uv_handle->len);
}
