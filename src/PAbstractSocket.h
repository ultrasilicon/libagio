#ifndef ABSTRACTSOCKET_H
#define ABSTRACTSOCKET_H


#include "Parsley.h"


PARSLEY_NAMESPACE_BEGIN
class AbstractSocket;

class AbstractSocket
{
public:
  enum CallbackType {
    Destryed = 0,
    ReadyRead = 1,
    Written = 2
  };
  using SocketDescriptor = int;

  using SockDestroyedCb = std::function<void (const int&)>;
  using SockReadyReadCb = std::function<void (Buffer, char*)>;
  using SockWrittenCb = std::function<void (const SocketDescriptor&)>;

  template<typename T>
  void bindCb(const CallbackType &t, const T &cb);
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


  static void allocCb(uv_handle_t *handle, size_t suggestedSize, uv_buf_t *buf);
};

template<typename T>
void AbstractSocket::bindCb(const CallbackType &t, const T &cb)
{
  switch (t) {
    case Destryed:
      destroyed_cb = (SockDestroyedCb)cb;
      break;
    case ReadyRead:
      ready_read_cb = (SockReadyReadCb)cb;
      break;
    case Written:
      written_cb = (SockWrittenCb)cb;
      break;
    }
}



PARSLEY_NAMESPACE_END
#endif // ABSTRACTSOCKET_H
