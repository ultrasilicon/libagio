#ifndef PBUFFER_H
#define PBUFFER_H

#include "Parsley.h"

#include <string>

PARSLEY_NAMESPACE_BEGIN

class BufferT;

class BufferT
    : public PObject<uv_buf_t, BufferT>
{
public:
  BufferT(char* data, const int len, Loop *l);
  ~BufferT();

  std::string toString();
};


PARSLEY_NAMESPACE_END
#endif // PBUFFER_H
