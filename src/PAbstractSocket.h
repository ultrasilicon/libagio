#ifndef ABSTRACTSOCKET_H
#define ABSTRACTSOCKET_H

#include "Log.h"

#ifdef Q_OS_WIN
#include "../libuv/win32/include/uv.h"
#else
#include <uv.h>
#endif

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
  typedef std::function<void (char*, char*)> SockReadyReadCb;
  typedef std::function<void (const SocketDescriptor&)> SockWrittenCb;

  void bindCb(const SockDestroyedCb &cb);
  void bindCb(const SockReadyReadCb &cb);

  bool callDestroyed(const int &sockDescriptor);
  bool callReadyRead(char* data, char* ip);
  bool callWritten(const SocketDescriptor &sd);


  virtual void bind(const char* ipAddr, const int &port);
  virtual void start();
  virtual void stop();
  virtual void write(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf);

  static int getFd(uv_handle_t *handle);

protected:
  int port;
  uv_loop_t* uv_loop;

  SockDestroyedCb destroyed_cb;
  SockReadyReadCb ready_read_cb;
  SockWrittenCb written_cb;


  static void allocBuffer(uv_handle_t *handle, size_t suggestedSize, uv_buf_t *buf);
};





#endif // ABSTRACTSOCKET_H
