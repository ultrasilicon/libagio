#ifndef PASYNC_H
#define PASYNC_H

#include "service.h"

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
#endif // PASYNC_H
