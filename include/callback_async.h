#ifndef AGIO_CALLBACK_ASYNC_H
#define AGIO_CALLBACK_ASYNC_H

#include "service.h"
#include "callback.h"
#include <tuple>
#include <functional>


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
  using ArgsTupleT = std::tuple<Args...>;

  static void executeCb(uv_async_t* handle)
  {
    CallbackAsync* ev = AgioServiceT::getAgioService(handle);
    std::apply(ev->f_, *static_cast<ArgsTupleT*>(ev->serviceData()->data));
  }

public:
  CallbackAsync(Loop* l)
    : AgioServiceT(l, this)
  {
    static_assert (std::is_same<Ret, void>::value, "CallbackAsync<Ret(Args...)> currently only supports void return type.");
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);
  }

  template<class T>
  CallbackAsync(T *obj , Ret (T::*func)(Args...), Loop* l)
    : AgioServiceT(l, this)
    , CallbackT(Functor<T, Ret, Args...>(obj, func))
  {
    static_assert (std::is_same<Ret, void>::value, "CallbackAsync<Ret(Args...)> currently only supports void return type.");
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);
  }

  CallbackAsync(Function<Ret(Args...)>& func, Loop* l)
    : AgioServiceT(l, this)
    , CallbackT(func)
  {
    static_assert (std::is_same<Ret, void>::value, "CallbackAsync<Ret(Args...)> currently only supports void return type.");
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);
  }

  CallbackAsync(Callback<Ret, Args...>* cb, Loop* l)
    : AgioServiceT(l, this)
    , CallbackT(cb)
  {
    static_assert (std::is_same<Ret, void>::value, "CallbackAsync<Ret(Args...)> currently only supports void return type.");
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);
  }

  template<class Lambda>
  CallbackAsync(Lambda&& lambda, Loop* l)
    : AgioServiceT(l, this)
    , CallbackT(lambda)
  {
    static_assert (std::is_same<Ret, void>::value, "CallbackAsync<Ret(Args...)> currently only supports void return type.");
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);
  }

  Ret operator()(Args&... args) noexcept
  {
    if(!getArgsTuple() || *getArgsTuple() != ArgsTupleT(args...)) {
        ArgsTupleT* argsTuple = new ArgsTupleT(args...);
        AgioServiceT::serviceData()->data = argsTuple;
      }
    uv_async_send(AgioServiceT::obj_);
  }

private:
  constexpr ArgsTupleT* getArgsTuple() {
    return static_cast<ArgsTupleT*>(AgioServiceT::serviceData()->data);
  }


};




A_NS_END
#endif // AGIO_CALLBACK_ASYNC_H
