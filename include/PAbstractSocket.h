#ifndef ABSTRACTSOCKET_H
#define ABSTRACTSOCKET_H


#include "Parsley.h"
#include "PBuffer.h"
#include "PAddress.h"

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
  virtual void start();
  virtual void stop();
  virtual void write(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf);
  static int getFd(uv_handle_t *handle);
  static void allocCb(uv_handle_t *handle, size_t size, uv_buf_t *buf);

protected:
};


P_NS_END
#endif // ABSTRACTSOCKET_H
