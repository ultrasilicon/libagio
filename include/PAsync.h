#ifndef PASYNC_H
#define PASYNC_H

#include "Parsley.h"

P_NS_BEGIN
class AsyncUtils;
class Async;

class AsyncUtils
  : public PObject<uv_async_t, Async>
{
public:
  AsyncUtils(Loop *l);

protected:
  static void executeCb(uv_async_t *r);
};

class Async
  : AsyncUtils
{
public:
  Callback<void> onCalled;

  Async(Loop *l);
  int send();

private:
};


P_NS_END
#endif // PASYNC_H
