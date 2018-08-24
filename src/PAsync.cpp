#include "PAsync.h"

using namespace Parsley;

void AsyncUtils::executeCb(uv_async_t *r)
{
  getInstance(r)->onCalled.call();
}


Async::Async(Loop *l)
  : AsyncUtils(l)
{
  uv_async_init(loop->uvHandle(), uv_handle, executeCb);
  regInstance(uv_handle, this);
}

int Async::send()
{
  return uv_async_send(uv_handle);
}
