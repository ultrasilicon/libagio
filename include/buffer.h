#ifndef AGIO_BUFFER_H
#define AGIO_BUFFER_H

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
#endif // AGIO_BUFFER_H
