#ifndef AGIO_CALLBACK_ASYNC_H
#define AGIO_CALLBACK_ASYNC_H

#include "async_event.h"

A_NS_BEGIN

template<typename Ret, typename... Args>
class CallbackAsync;
template<typename Ret, typename... Args>
class CallbackAsync<Ret(Args...)>;


template<typename Ret, typename... Args>
class CallbackAsync<Ret(Args...)>
  : public AgioService<uv_async_t, CallbackAsync<Ret(Args...)>>
  , public Callback<Ret(Args...)>
{
  using AgioServiceT = AgioService<uv_async_t, CallbackAsync<Ret(Args...)>>;

  static void executeCb(uv_async_t* handle)
  {
    AsyncEvent* ev = AgioServiceT::getAgioService(handle);
    ev->onCalled(ev);
  }

public:
  Callback<void(AsyncEvent*)> onCalled;

  CallbackAsync(Loop* l)
    : AgioServiceT(l, this)
  {
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);
  }

  void operator()()
  {
    send();
  }

  int send()
  {
    return uv_async_send(AgioServiceT::obj_);
  }

private:
};




A_NS_END
#endif // AGIO_CALLBACK_ASYNC_H
