#ifndef ABSTRACTSOCKET_H
#define ABSTRACTSOCKET_H


#include "parsley.h"
#include "buffer.h"
#include "address.h"

P_NS_BEGIN

class AbstractSocket;

class AbstractSocket
{
public:
  using SocketDescriptor = int;

//  Callback<void, std::string&, AbstractSocket*> onReadyRead;
//  Callback<void, const SocketDescriptor&> onWritten;

  virtual ~AbstractSocket();
  virtual void bind(const char* ipAddr, const int &port);
  virtual int start();
  virtual void stop();

  static void allocCb(uv_handle_t *, size_t size, uv_buf_t *buf);

protected:
};


P_NS_END
#endif // ABSTRACTSOCKET_H