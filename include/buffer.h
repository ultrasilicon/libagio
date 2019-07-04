#ifndef PBUFFER_H
#define PBUFFER_H

#include "parsley.h"

#include <string>

P_NS_BEGIN

class Buffer;

class Buffer
    : public PObject<uv_buf_t, Buffer>
{
public:
  Buffer(const std::string &data);
  Buffer(char* data, const size_t len);
  ~Buffer();

  char* data() const;
  int length() const;
  std::string toString() const;
};





P_NS_END
#endif // PBUFFER_H
