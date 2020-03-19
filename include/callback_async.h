#ifndef AGIO_CALLBACK_ASYNC_H
#define AGIO_CALLBACK_ASYNC_H

#include "service.h"
#include "callback.h"
#include "promise.h"

#include <tuple>


A_NS_BEGIN

template<typename... Ts>
class Promise;
template<typename Ret, typename... Args>
struct CallbackAsyncData;
template<typename Ret, typename... Args>
struct CallbackAsyncData<Ret(Args...)>;
template<typename Ret, typename... Args>
class CallbackAsync;
template<typename Ret, typename... Args>
class CallbackAsync<Ret(Args...)>;




template<typename... Ts>
class Promise {
public:
  using T = typename std::tuple_element<0, std::tuple<Ts...> >::type;
  template<typename U>
  using IsVoid = typename std::enable_if<std::is_void<U>::value, void>::type;
  template<typename U>
  using IsNotVoid = typename std::enable_if<!std::is_void<U>::value, void>::type;

  template<typename T>
  struct CallbackSelector {
    typedef CallbackAsync<void(T)> type;
  };
  template<>
  struct CallbackSelector<void> {
    typedef CallbackAsync<void()> type;
  };

  using CallbackT = typename CallbackSelector<T>::type;

  Promise(Loop* l)
    : loop_(l)
  { }

  Promise(CallbackT& cb, Loop* l)
    : cb_(cb)
    , loop_(l)
  { }

  Promise(CallbackT&& cb, Loop* l)
    : loop_(l)
    , cb_(cb)
  { }

  //! TODO: fix all destructors
//  ~Promise() {
//    if(cb_)
//      delete cb_;
//    cb_ = nullptr;
//    if(next_promise_)
//      delete next_promise_;
//    next_promise_ = nullptr;
//  }

  template<typename U = T, IsNotVoid<U> = 0>
  void resolve(U& t) {

  }

//  template<typename U = T, IsVoid<U> = 0>
  void resolve() {
    if(cb_)
      (*cb_)();
    if(next_promise_)
      next_promise_->resolve();
  }

  template<typename Callable>
  Promise<T>& then(Callable&& cb) {
    cb_ = new CallbackT(cb, loop_);
//    if(next_promise_)
//      delete next_promise_;
    next_promise_ = new Promise<T>(loop_);
    return *next_promise_;
  }

  template<class Lambda>
  Promise<T>& finally(Lambda&& lambda) {
    return *this;
  }

  template<class Lambda>
  Promise<T>& err(Lambda&& lambda) {
    return *this;
  }

private:
  Loop* loop_;
  Promise<T>* next_promise_;
  CallbackT* cb_;
  CallbackT* final_cb_;
  CallbackT* error_cb_;
};



/*! @note The template parameters should be explicit,
 *  which means it should not be just the agio
 *  type. Because ServiceData uses the template
 *  parameters of the agio class type
 */
template<typename Ret, typename... Args>
struct CallbackAsyncData<Ret(Args...)>
    : ServiceData<CallbackAsync<Ret(Args...)>>
{
  using AgioT = CallbackAsync<Ret(Args...)>;
  using ArgsTupleT = std::tuple<Args...>;

  ArgsTupleT* args_tuple_;
  Promise<Ret>* promise_;

  ~CallbackAsyncData()
  {
//    if(args_tuple_)
//      delete args_tuple_;
//    args_tuple_ = nullptr;
    if(promise_)
      delete promise_;
    promise_= nullptr;
  }
};


template<typename Ret, typename... Args>
class CallbackAsync<Ret(Args...)>
  : public AgioService<uv_async_t, CallbackAsync<Ret(Args...)>, CallbackAsyncData<Ret(Args...)>>
  , public Callback<Ret(Args...)>
{
  using AgioServiceT = AgioService<uv_async_t, CallbackAsync<Ret(Args...)>, CallbackAsyncData<Ret(Args...)>>;
  using CallbackT = Callback<Ret(Args...)>;
  using ArgsTupleT = std::tuple<Args...>;

  //! TODO: implement for non void return type
  static void executeCb(uv_async_t* handle)
  {
    CallbackAsync* cb = AgioServiceT::getAgioService(handle);
    std::apply(cb->f_, *cb->serviceData()->args_tuple_);

    Promise<Ret>*& promise = cb->serviceData()->promise_;
    if(promise)
      {
        //! TODO: null pointer crash, fix resolve implmentation
//        promise->resolve();
        delete promise;
        promise = nullptr;
      }
  }

public:
//  CallbackAsync() = delete;

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

  Promise<Ret>& operator()(Args... args) noexcept
  {
    ArgsTupleT*& argsTuple = AgioServiceT::serviceData()->args_tuple_;
    if(!argsTuple || *argsTuple != ArgsTupleT(args...)) {
        if(argsTuple)
          delete argsTuple;
        argsTuple = new ArgsTupleT(args...);
      }

//    AgioServiceT::serviceData()->args_tuple_ = new ArgsTupleT(args...);

    uv_async_send(AgioServiceT::obj_);
    return *AgioServiceT::serviceData()->promise_;
  }

  template<class Lambda>
  CallbackAsync<Ret(Args...)>& operator=(Lambda&& lambda) noexcept
    {
      this->f_ = lambda;
      return *this;
    }

private:
  inline void init()
  {
    static_assert (std::is_same<Ret, void>::value, "CallbackAsync<Ret(Args...)> currently only supports void return type.");
    uv_async_init(AgioServiceT::loop_->cObject(), AgioServiceT::obj_, executeCb);

    Promise<Ret>*& promise = AgioServiceT::serviceData()->promise_;
    if(promise)
      delete promise;
    promise = new Promise<Ret>(this->loop_);
  }
};




A_NS_END
#endif // AGIO_CALLBACK_ASYNC_H
