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
  , public Callback<Ret(Args...)>
{
  using AgioServiceT = AgioService<uv_async_t, CallbackAsync<Ret(Args...)>>;
  using CallbackT = Callback<Ret(Args...)>;
  using ArgsTupleT = std::tuple<CallbackAsync, Args...>;

  static void executeCb(uv_async_t* handle)
  {
    CallbackAsync* ev = AgioServiceT::getAgioService(handle);
    std::apply(ev->f_, *static_cast<ArgsTupleT*>(ev->serviceData.data));
  }

public:
  CallbackAsync(Loop* l)
    : AgioServiceT(l, this)
    , CallbackT()
  {
    static_assert (std::is_same<Ret, void>::value, "CallbackAsync<Ret(Args...)> currently only supports void return type.");
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);
  }

  template<class T>
  CallbackAsync(T *obj , Ret (T::*func)(ArgsTupleT*, Args...), Loop* l)
    : AgioServiceT(l, this)
    , CallbackT(Functor<T, Ret, ArgsTupleT*, Args...>(obj, func))
  {
    static_assert (std::is_same<Ret, void>::value, "CallbackAsync<Ret(Args...)> currently only supports void return type.");
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);
  }

  CallbackAsync(Function<Ret(ArgsTupleT*, Args...)>& func, Loop* l)
    : AgioServiceT(l, this)
    , CallbackT(func)
  {
    static_assert (std::is_same<Ret, void>::value, "CallbackAsync<Ret(Args...)> currently only supports void return type.");
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);
  }

  CallbackAsync(Callback<Ret, ArgsTupleT*, Args...>* handler, Loop* l)
    : AgioServiceT(l, this)
    , CallbackT(handler)
  {
    static_assert (std::is_same<Ret, void>::value, "CallbackAsync<Ret(Args...)> currently only supports void return type.");
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);
  }

  template<class Lambda>
  CallbackAsync(Lambda&& handler, Loop* l)
    : AgioServiceT(l, this)
    , CallbackT(handler)
  {
    static_assert (std::is_same<Ret, void>::value, "CallbackAsync<Ret(Args...)> currently only supports void return type.");
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);
  }

  Ret operator()(Args... args) const noexcept
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
