#ifndef PASYNC_H
#define PASYNC_H

#include "parsley.h"

P_NS_BEGIN
class AsyncEventUtils;
class AsyncEvent;

class AsyncEventUtils
  : public PUvObject<uv_async_t, AsyncEvent>
{
public:
  AsyncEventUtils(Loop *l);

protected:
  static void executeCb(uv_async_t *r);
};

class AsyncEvent
  : public AsyncEventUtils
{
public:
  CallbackHandler<void()> onCalled;

  AsyncEvent(Loop *l);
  int send();

private:
};


P_NS_END
#endif // PASYNC_H
