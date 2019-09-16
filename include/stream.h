#ifndef STREAM_H
#define STREAM_H


#include "agio.h"
#include "buffer.h"

A_NS_BEGIN
class Stream;


class Stream
{
public:
  Stream();
  virtual ~Stream();

  virtual int write(char* data);
  virtual int write(char* data, const size_t &len);
  virtual int write(const std::string&);

private:

};


A_NS_END

#endif // STREAM_H
