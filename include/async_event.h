#ifndef PASYNC_H
#define PASYNC_H

#include "parsley.h"

P_NS_BEGIN

class AsyncEvent
  : public PUvObject<uv_async_t, AsyncEvent>
{
  static void executeCb(uv_async_t *r);

public:
  CallbackHandler<void()> onCalled;

  AsyncEvent(Loop *l);
  int send();

private:
};


P_NS_END
#endif // PASYNC_H
