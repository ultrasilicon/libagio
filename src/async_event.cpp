#include "async_event.h"

using namespace Parsley;

void AsyncEvent::executeCb(uv_async_t *handle)
{
  auto newf = getPHandle(handle);
  newf->onCalled();
  auto oldf = &getInstance(handle)->data_->pHandle;
//  getPHandle(handle)->onCalled();
//  getInstance(handle)->onCalled();
}

AsyncEvent::AsyncEvent(Loop *l)
  : PUvObject(l, this)
{
  uv_async_init(loop_->uvHandle(), obj_, executeCb);
  regInstance(obj_, this);
}

int AsyncEvent::send()
{
  return uv_async_send(obj_);
}

void AsyncEvent::print()
{
  std::cout << "hello!!!!!!!!!!!!!!!!" << std::endl;
}
