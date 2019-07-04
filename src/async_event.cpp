#include "async_event.h"

using namespace Agio;

void AsyncEvent::executeCb(uv_async_t* handle)
{
  getPHandle(handle)->onCalled();
}

AsyncEvent::AsyncEvent(Loop *l)
  : UvObject(l, this)
{
  uv_async_init(loop_->uvHandle(), obj_, executeCb);
}

int AsyncEvent::send()
{
  return uv_async_send(obj_);
}
