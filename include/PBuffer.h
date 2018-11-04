#ifndef PBUFFER_H
#define PBUFFER_H

#include "Parsley.h"

#include <string>

P_NS_BEGIN

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


P_NS_END
#endif // PBUFFER_H
