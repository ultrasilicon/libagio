#include "PAsync.h"

using namespace Parsley;

void AsyncUtils::executeCb(uv_async_t *r)
{
  getInstance(r)->callAsyncCb();
}


Async::Async(Loop *l)
  : AsyncUtils(l)
{
  addInstance(uv_handle, this);
}

int Async::send()
{
  return uv_async_send(uv_handle);
}

void Async::bindCb(const AsyncUtils::AsyncCb &cb)
{
  async_cb = cb;
  uv_async_init(loop->uvHandle(), uv_handle, executeCb);
}

bool Async::callAsyncCb()
{
  if(async_cb)
    {
      async_cb();
      return true;
    }
  return false;
}
