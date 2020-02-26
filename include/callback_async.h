#ifndef AGIO_CALLBACK_ASYNC_H
#define AGIO_CALLBACK_ASYNC_H

#include "service.h"
#include "callback.h"

A_NS_BEGIN

class AsyncEvent
  : public AgioService<uv_async_t, AsyncEvent>
{
  static void executeCb(uv_async_t* handle)
  {
    AsyncEvent* ev = getAgioService(handle);
    ev->onCalled(ev);
  }

public:
  Callback<void(AsyncEvent*)> onCalled;

  AsyncEvent(Loop* l)
    : AgioService(l, this)
  {
    uv_async_init(loop_->cObject(), obj_, executeCb);
  }

  void operator()()
  {
    send();
  }

  int send()
  {
    return uv_async_send(obj_);
  }

private:
};




A_NS_END
#endif // AGIO_CALLBACK_ASYNC_H
