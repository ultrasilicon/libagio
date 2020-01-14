#ifndef AGIO_BUFFER_H
#define AGIO_BUFFER_H

#include "agio.h"
#include <string>

A_NS_BEGIN

class Buffer;

/*!
 * \brief The Buffer class
 * \note Buffer maintains a iovec-like buffer.
 * \note Ending character '\0' is regarded as regular character
 */
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

  /*!
   * \brief copy
   * \param dest
   * \param len
   * \note Copy first len bytes to dest.
   */
  void copy(char* dest, const unsigned int& len) const;

  /*!
   * \brief copy
   * \param dest
   * \param len
   * \note Copy bytes from fromPos until toPos. toPos is included.
   */
  void copy(char* dest, char* fromPos, char* toPos) const;

  /*!
   * \brief copy
   * \param dest
   * \param len
   * \note Copy first len bytes from fromPos. fromPos is included.
   */
  void copy(char* dest, char* fromPos, const unsigned int& len) const ;

  char* front() const;
  char* back() const;
  char* data() const;
  size_t length() const;
  size_t size() const;
  uv_buf_t* uvBuffer() const;
  std::string toString() const;
};





A_NS_END
#endif // AGIO_BUFFER_H
