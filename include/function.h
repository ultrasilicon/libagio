#ifndef PFUNCTION_H
#define PFUNCTION_H

#include <functional>
#include <utility>

namespace Agio {


template<class T, typename Ret, typename... Args>
struct Functor;
template<typename Ret, typename... Args>
struct CallbackHandler;
template<typename Ret, typename... Args>
struct CallbackHandler<Ret(Args...)>;




template<typename Ret, typename... Args>
struct function_ptr;
template<typename Ret, typename... Args>
struct function_ptr<Ret(Args...)> {
  using type = void (*)(void *, Args... args);
};
template<typename T>
using Function =  typename function_ptr<T>::type;


template<class T, typename Ret, typename... Args>
struct Functor {
    using F = Ret (T::*)(Args...);
    T *o_;
    F f_;

    Functor(T *obj, F func)
        : o_(obj)
        , f_(func)
    {}

    Ret operator()(Args... args)
    {
        return (o_->*f_)(args...);
    }
};

template<class T, typename Ret, typename... Args>
Functor<T, Ret, Args...> functor_wrap(T *obj , Ret (T::*func)(Args...))
{
    return Functor<T, Ret, Args...>(obj , func);
}


template<typename Ret, typename... Args>
struct CallbackHandler<Ret(Args...)> {
  std::function<Ret(Args...)> f_;

  CallbackHandler()
  {}

  template<class T>
  CallbackHandler(T *obj , Ret (T::*func)(Args...))
    : f_(Functor<T, Ret, Args...>(obj, func))
  {}

  CallbackHandler(Function<Ret(Args...)> func)
    : f_(func)
  {}

  CallbackHandler(CallbackHandler<Ret, Args...> *handler)
    : f_(handler)
  {}

  template<class Lambda>
  CallbackHandler(Lambda&& handler)
    : f_(handler)
  {}

  Ret operator()(Args... args) const noexcept
  {
    if(f_)
      return f_(args ...);
    return Ret();
  }

  //! Member funtion
  template<class T>
  void connect(T *obj , Ret (T::*func)(Args...))
  {
    f_ = Functor<T, Ret, Args...>(obj, func);
  }

  //! Static funtion
  void connect(Ret (*func)(Args...))
  {
    f_ = func;
  }

  //! Callback<>
  void connect(CallbackHandler<Ret, Args...> *handler)
  {
    f_ = handler->f_;
  }

  //! Lambda
  template<typename Lambda>
  void connect(Lambda&& handler)
  {
    f_ = handler;
  }
};

template<typename Ret1, typename... Args1, class T, typename Ret2, typename... Args2>
void on(CallbackHandler<Ret1(Args1...)> *handler, T *obj , Ret2 (T::*func)(Args2...))
{
  static_assert (std::is_same<Ret1(Args1...), Ret2(Args2...)>::value, "binding callback functions of unmatched types.");
  handler->connect(obj, func);
}

template<typename Ret1, typename... Args1, typename Ret2, typename... Args2>
void on(CallbackHandler<Ret1(Args1...)> *handler, Ret2 (*func)(Args2...))
{
  static_assert (std::is_same<Ret1(Args1...), Ret2(Args2...)>::value, "binding callback functions of unmatched types.");
  handler->connect(func);
}

template<typename Ret1, typename... Args1, typename Ret2, typename... Args2>
void on(CallbackHandler<Ret1(Args1...)> *handler1, CallbackHandler<Ret2(Args2...)> *handler2)
{
  static_assert (std::is_same<Ret1(Args1...), Ret2(Args2...)>::value, "binding callback functions of unmatched types.");
  handler1->connect(handler2);
}

template<typename Ret, typename... Args, typename Lambda>
void on(CallbackHandler<Ret(Args...)> *handler, Lambda&& lambda)
{
  static_assert (std::is_same<Ret(Args...), Lambda>::value, "binding callback functions of unmatched types.");
  handler->connect(lambda);
}




}

#endif // PFUNCTION_H
