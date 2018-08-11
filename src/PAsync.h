#ifndef PASYNC_H
#define PASYNC_H

#include "Parsley.h"

PARSLEY_NAMESPACE_BEGIN
class AsyncUtils;
class Async;

class AsyncUtils
  : public PObject<uv_async_t, Async>
{
public:
  using AsyncCb = std::function<void (void)>;

  AsyncUtils(Loop *l) : PObject(l){}

protected:
  static void executeCb(uv_async_t *r);
};

class Async
  : AsyncUtils
{
public:
  Async(Loop *l);
  int send();
  void bindCb(const AsyncCb &cb);

  void callAsyncCb();

private:
  AsyncCb async_cb;

};


PARSLEY_NAMESPACE_END
#endif // PASYNC_H
