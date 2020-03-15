#ifndef AGIO_CALLBACK_ASYNC_H
#define AGIO_CALLBACK_ASYNC_H

#include "service.h"
#include "callback.h"
#include "promise.h"

#include <tuple>


A_NS_BEGIN

template<typename Ret, typename... Args>
struct CallbackAsyncData;
template<typename Ret, typename... Args>
struct CallbackAsyncData<Ret(Args...)>;
template<typename Ret, typename... Args>
class CallbackAsync;
template<typename Ret, typename... Args>
class CallbackAsync<Ret(Args...)>;



/*! @note The template parameters should be explicit,
 *  which means it should not be just the agio
 * type. Because ServiceData uses the template
 * parameters of the agio class type
 */
template<typename Ret, typename... Args>
struct CallbackAsyncData<Ret(Args...)>
    : ServiceData<CallbackAsync<Ret(Args...)>>
{
  using AgioT = CallbackAsync<Ret(Args...)>;
  using ArgsTupleT = std::tuple<Args...>;

  ArgsTupleT* args_tuple_;
  Promise<Ret>* promise_;
};


template<typename Ret, typename... Args>
class CallbackAsync<Ret(Args...)>
  : public AgioService<uv_async_t, CallbackAsync<Ret(Args...)>, CallbackAsyncData<Ret(Args...)>>
  , public Callback<Ret(Args...)>
{
  using AgioServiceT = AgioService<uv_async_t, CallbackAsync<Ret(Args...)>, CallbackAsyncData<Ret(Args...)>>;
  using CallbackT = Callback<Ret(Args...)>;
  using ArgsTupleT = std::tuple<Args...>;

  static void executeCb(uv_async_t* handle)
  {
    CallbackAsync* ev = AgioServiceT::getAgioService(handle);
    std::apply(ev->f_, *ev->serviceData()->args_tuple_);

  }

public:
  CallbackAsync(Loop* l)
    : AgioServiceT(l, this)
  {
    init();
  }

  template<class T>
  CallbackAsync(T *obj , Ret (T::*func)(Args...), Loop* l)
    : AgioServiceT(l, this)
    , CallbackT(Functor<T, Ret, Args...>(obj, func))
  {
    init();
  }

  CallbackAsync(Function<Ret(Args...)>& func, Loop* l)
    : AgioServiceT(l, this)
    , CallbackT(func)
  {
    init();
  }

  CallbackAsync(Callback<Ret, Args...>* cb, Loop* l)
    : AgioServiceT(l, this)
    , CallbackT(cb)
  {
    init();
  }

  template<class Lambda>
  CallbackAsync(Lambda&& lambda, Loop* l)
    : AgioServiceT(l, this)
    , CallbackT(lambda)
  {
    init();
  }

  Promise<Ret>* operator()(Args... args) noexcept
  {
    ArgsTupleT*& argsTuple = AgioServiceT::serviceData()->args_tuple_;
    if(!argsTuple || *argsTuple != ArgsTupleT(args...)) {
        if(argsTuple)
          delete argsTuple;
        argsTuple = new ArgsTupleT(args...);
      }

    Promise<Ret>*& promise = AgioServiceT::serviceData()->promise_;
    if(promise)
      delete promise;
    promise = new Promise<Ret>();

    uv_async_send(AgioServiceT::obj_);
    return promise;
  }

//! TODO: not working
//  template<class Lambda>
//  CallbackAsync<Ret(Args...)>& operator=(Lambda&& lambda) noexcept
//    {
//      if(getArgsTuple())
//          delete getArgsTuple();
//      this->f_ = lambda;
//    }

private:
  inline void init() {
    static_assert (std::is_same<Ret, void>::value, "CallbackAsync<Ret(Args...)> currently only supports void return type.");
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);
  }
};




A_NS_END
#endif // AGIO_CALLBACK_ASYNC_H
