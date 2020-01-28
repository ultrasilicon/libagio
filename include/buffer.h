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
  Buffer(const unsigned int& len);
  Buffer(char* data, const unsigned int& len);
  ~Buffer();

  constexpr char& operator[](const std::size_t &idx);
  constexpr const char& operator[](const std::size_t &idx) const;
  constexpr char* front() const;
  constexpr char* back() const;
  constexpr char* data() const;
  constexpr size_t length() const;
  constexpr size_t size() const;
  constexpr uv_buf_t* uvBuffer() const;

  std::string toString() const;
  //! \brief copy Copy first len bytes to dest.
  void copy(char* dest, const unsigned int& len) const;
  //! \brief copy Copy bytes from fromPos until toPos. toPos is included.
  void copy(char* dest, char* fromPos, char* toPos) const;
  //!\brief copy  Copy first len bytes from fromPos.
  void copy(char* dest, char* fromPos, const unsigned int& len) const ;
};





A_NS_END
#endif // AGIO_BUFFER_H
