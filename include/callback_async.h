#ifndef AGIO_CALLBACK_ASYNC_H
#define AGIO_CALLBACK_ASYNC_H

#include "service.h"
#include "callback.h"
#include <tuple>

A_NS_BEGIN

template<typename Ret, typename... Args>
class CallbackAsync;
template<typename Ret, typename... Args>
class CallbackAsync<Ret(Args...)>;


template<typename Ret, typename... Args>
class CallbackAsync<Ret(Args...)>
  : public AgioService<uv_async_t, CallbackAsync<Ret(Args...)>>
{
  using AgioServiceT = AgioService<uv_async_t, CallbackAsync<Ret(Args...)>>;
  using ArgsTupleT = std::tuple<CallbackAsync, Args...>;

  static void executeCb(uv_async_t* handle)
  {
    CallbackAsync* ev = AgioServiceT::getAgioService(handle);
    std::apply(ev->onCalled, *static_cast<ArgsTupleT*>(ev->serviceData.data));
  }

public:
  Callback<Ret(CallbackAsync*, Args...)> onCalled;

  CallbackAsync(Loop* l)
    : AgioServiceT(l, this)
  {
    static_assert (std::is_same<Ret, void>::value, "CallbackAsync<Ret(Args...)> currently only supports void return type.");
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);
  }

  Ret operator()(Args... args)
  {
    ArgsTupleT* args_tuple_ = new ArgsTupleT*(this, args...);
    this->serviceData()->data = args_tuple_;
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
