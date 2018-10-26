#ifndef PBUFFER_H
#define PBUFFER_H

#include "Parsley.h"

#include <string>

PARSLEY_NAMESPACE_BEGIN

class Buffer;

class Buffer
    : public PObject<uv_buf_t, Buffer>
{
public:
  Buffer(char* data, const int len, Loop *l);
  ~Buffer();

  char* data();
  int length();

  std::string toString();
};


PARSLEY_NAMESPACE_END
#endif // PBUFFER_H
