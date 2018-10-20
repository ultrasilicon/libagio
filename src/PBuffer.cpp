#include "PBuffer.h"

using namespace Parsley;

BufferT::BufferT(char *data, const int len, Loop *l)
  : PObject(l)
{
  uv_buf_init(data, len);
}

BufferT::~BufferT()
{

}

std::string BufferT::toString()
{
  return std::string(uv_handle->base, uv_handle->len);
}
