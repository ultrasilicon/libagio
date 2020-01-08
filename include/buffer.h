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
  Buffer(const std::string& data);
  Buffer(char* data, const unsigned int& len);
  Buffer(const unsigned int& len);
  ~Buffer();

  char& operator[](const std::size_t &idx);
  const char& operator[](const std::size_t &idx) const;

  char* front() const;
  char* back() const;
  char* data() const;
  size_t length() const;
  size_t size() const;
  std::string toString() const;
  uv_buf_t* uvBuffer();
};





A_NS_END
#endif // AGIO_BUFFER_H
