#include "async_event.h"

using namespace Parsley;

void AsyncEvent::executeCb(uv_async_t *r)
{
  getInstance(r)->onCalled();
}

AsyncEvent::AsyncEvent(Loop *l)
  : PUvObject(l)
{
  uv_async_init(loop_->uvHandle(), obj_, executeCb);
  regInstance(obj_, this);
}

int AsyncEvent::send()
{
  return uv_async_send(obj_);
}
