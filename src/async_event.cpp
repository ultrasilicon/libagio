#include "async_event.h"

using namespace Parsley;

AsyncEventUtils::AsyncEventUtils(Loop *l)
  : PUvObject(l)
{
}

void AsyncEventUtils::executeCb(uv_async_t *r)
{
  getInstance(r)->onCalled.call();
}


AsyncEvent::AsyncEvent(Loop *l)
  : AsyncEventUtils(l)
{
  uv_async_init(loop_->uvHandle(), obj_, executeCb);
  regInstance(obj_, this);
}

int AsyncEvent::send()
{
  return uv_async_send(obj_);
}
