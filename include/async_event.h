#ifndef AGIO_ASYNC_EVENT_H
#define AGIO_ASYNC_EVENT_H

#include "service.h"
#include "callback.h"

A_NS_BEGIN

class AsyncEvent
  : public AgioService<uv_async_t, AsyncEvent>
{
  static void executeCb(uv_async_t* handle);

public:
  Callback<void(AsyncEvent*)> onCalled;

  AsyncEvent(Loop* l);
  void operator()();
  int send();

private:
};

A_NS_END
#endif // AGIO_ASYNC_EVENT_H
