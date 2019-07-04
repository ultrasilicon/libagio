#include "async_event.h"

using namespace Parsley;

void AsyncEvent::executeCb(uv_async_t* handle)
{
  getPHandle(handle)->onCalled();
}

AsyncEvent::AsyncEvent(Loop *l)
  : PUvObject(l, this)
{
  uv_async_init(loop_->uvHandle(), obj_, executeCb);
}

int AsyncEvent::send()
{
  return uv_async_send(obj_);
}

void AsyncEvent::print()
{
  std::cout << "TEST AsyncEvent in libparsley" << std::endl;
}
