#ifndef PASYNC_H
#define PASYNC_H

#include "service.h"

A_NS_BEGIN

class AsyncEvent
  : public AgioService<uv_async_t, AsyncEvent>
{
  static void executeCb(uv_async_t *handle);

public:
  CallbackHandler<void()> onCalled;

  AsyncEvent(Loop *l);
  int send();

private:
};


A_NS_END
#endif // PASYNC_H
