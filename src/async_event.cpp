#include "async_event.h"

using namespace Agio;

void AsyncEvent::executeCb(uv_async_t* handle)
{
  AsyncEvent* ev = getPHandle(handle);
  ev->onCalled(ev);
}

AsyncEvent::AsyncEvent(Loop* l)
  : AgioService(l, this)
{
  uv_async_init(loop_->cObject(), obj_, executeCb);
}

void AsyncEvent::operator()()
{
  send();
}

int AsyncEvent::send()
{
  return uv_async_send(obj_);
}

