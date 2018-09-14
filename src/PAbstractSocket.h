#ifndef ABSTRACTSOCKET_H
#define ABSTRACTSOCKET_H


#include "Parsley.h"


PARSLEY_NAMESPACE_BEGIN
class AbstractSocket;

class AbstractSocket
{
public:
  using SocketDescriptor = int;

  virtual ~AbstractSocket();

  Callback<void, Buffer, char*> onReadyRead;
  Callback<void, const SocketDescriptor&> onWritten;

  virtual void bind(const char* ipAddr, const int &port);
  virtual void start();
  virtual void stop();
  virtual void write(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf);
  static int getFd(uv_handle_t *handle);
  static void allocCb(uv_handle_t *handle, size_t suggestedSize, uv_buf_t *buf);

protected:
};


PARSLEY_NAMESPACE_END
#endif // ABSTRACTSOCKET_H
