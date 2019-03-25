#ifndef STREAM_H
#define STREAM_H


#include "parsley.h"
#include "buffer.h"

P_NS_BEGIN
class Stream;


class Stream
{
public:
  Stream();
  virtual ~Stream();

  virtual int write(char* data);
  virtual int write(char* data, const size_t &len);


private:

};


P_NS_END

#endif // STREAM_H
