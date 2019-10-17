#ifndef PFUNCTION_H
#define PFUNCTION_H

#include <functional>

namespace Agio {

template<class T, typename Ret, typename... Args>
struct Function;
template<class T, typename Ret, typename... Args>
struct Functor;
template<typename Ret, typename... Args>
struct CallbackHandler;
template<typename Ret, typename... Args>
struct CallbackHandler<Ret(Args...)>;

//template <class T, typename std::enable_if<std::is_member_function_pointer<T>::value, T>::type* = nullptr>
//void do_stuff(T& t) {
//  std::cout << "do_stuff integral\n";
//    // an implementation for integral types (int, char, unsigned, etc.)
//}

//template <class T, >
//void do_stuff(T& t) {
//    // an implementation for class types
//}


template<class T, typename Ret, typename... Args>
struct Function {
  using F = Ret(T::*)(Args...);
  T* o_;
  F f_;

  Function(T* obj, F func)
    : o_(obj)
    , f_(func)
  {}

  Ret call(Args... args)
  {
    return (o_->*f_)(args...);
  }
};

template<class T, typename Ret, typename... Args>
static Function<T, Ret, Args...> function_wrap(T *t, Ret(T::*f)(Args...))
{
    Function<T, Ret, Args...> fp(t, f);
    return fp;
}

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

  Ret operator()(Args... args) noexcept
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
};

template<typename Ret1, typename... Args1, class T, typename Ret2, typename... Args2>
void on(CallbackHandler<Ret1(Args1...)> *handler, T *obj , Ret2 (T::*func)(Args2...))
{
//  printf("mem fun");
  static_assert (std::is_same<Ret1(Args1...), Ret2(Args2...)>::value, "binding callback functions of unmatched types.");
  handler->connect(obj, func);
}

template<typename Ret1, typename... Args1, typename Ret2, typename... Args2>
void on(CallbackHandler<Ret1(Args1...)> *handler, Ret2 (*func)(Args2...))
{
//  printf("static fun");
  static_assert (std::is_same<Ret1(Args1...), Ret2(Args2...)>::value, "binding callback functions of unmatched types.");
  handler->connect(func);
}

template<typename Ret1, typename... Args1, typename Ret2, typename... Args2>
void on(CallbackHandler<Ret1(Args1...)> *handler1, CallbackHandler<Ret2(Args2...)> *handler2)
{
  printf("cb fun");
  static_assert (std::is_same<Ret1(Args1...), Ret2(Args2...)>::value, "binding callback functions of unmatched types.");
  handler1->connect(handler2);
}




}

#endif // PFUNCTION_H
