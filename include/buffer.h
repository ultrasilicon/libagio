#ifndef PBUFFER_H
#define PBUFFER_H

#include "agio.h"

#include <string>

A_NS_BEGIN

class Buffer;

class Buffer
    : public AgioObject<uv_buf_t, Buffer>
{
public:
  Buffer(const std::string &data);
  Buffer(char* data, const size_t len);
  ~Buffer();

  char* data() const;
  int length() const;
  std::string toString() const;
};





A_NS_END
#endif // PBUFFER_H
