#ifndef ABSTRACTSOCKET_H
#define ABSTRACTSOCKET_H


#include "Parsley.h"


PARSLEY_NAMESPACE_BEGIN
class AbstractSocket;

class AbstractSocket
{
public:
  enum Callback {
    Destryed = 0,
    Read = 1,
    Written = 2
  };

  typedef int SocketDescriptor;
  typedef std::function<void (const int&)> SockDestroyedCb;
  typedef std::function<void (Buffer, char*)> SockReadyReadCb;
  typedef std::function<void (const SocketDescriptor&)> SockWrittenCb;

  void bindCb(const SockDestroyedCb &cb);
  void bindCb(const SockReadyReadCb &cb);

  bool callDestroyed(const SocketDescriptor &sd);
  bool callReadyRead(const Buffer &data, char* ip);
  bool callWritten(const SocketDescriptor &sd);


  virtual void bind(const char* ipAddr, const int &port);
  virtual void start();
  virtual void stop();
  virtual void write(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf);

  static int getFd(uv_handle_t *handle);

protected:
  int port;

  SockDestroyedCb destroyed_cb;
  SockReadyReadCb ready_read_cb;
  SockWrittenCb written_cb;


  static void allocBuffer(uv_handle_t *handle, size_t suggestedSize, uv_buf_t *buf);
};




PARSLEY_NAMESPACE_END
#endif // ABSTRACTSOCKET_H
