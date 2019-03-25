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
  uv_async_init(m_loop->uvHandle(), m_uv_obj, executeCb);
  regInstance(m_uv_obj, this);
}

int AsyncEvent::send()
{
  return uv_async_send(m_uv_obj);
}
