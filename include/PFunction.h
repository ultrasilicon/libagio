#ifndef PFUNCTION_H
#define PFUNCTION_H

#include <functional>

namespace Parsley {

template<class T, typename Ret, typename... Args>
struct Function;
template<class T, typename Ret, typename... Args>
struct Functor;
template<typename Ret, typename... Args>
struct Callback;

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
  T* o;
  F f;

  Function(T* obj, F func)
    : o(obj)
    , f(func)
  {}

  Ret call(Args... args)
  {
    return (o->*f)(args...);
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
    T *o;
    F f;

    Functor(T *obj, F func)
        : o(obj)
        , f(func)
    {}

    Ret operator()(Args... args)
    {
        return (o->*f)(args...);
    }
};

template<class T, typename Ret, typename... Args>
Functor<T, Ret, Args...> functor_wrap(T *obj , Ret (T::*func)(Args...))
{
    return Functor<T, Ret, Args...>(obj , func);
}


template<typename Ret, typename... Args>
struct Callback {
  using Func = std::function<Ret(Args...)>;
  Func f;

  Ret call(Args... args) noexcept
  {
    if(f)
      return f(args ...);
    return Ret();
  }

  //! Member funtion
  template<class T>
  void connect(T *obj , Ret (T::*func)(Args...))
  {
    f = Functor<T, Ret, Args...>(obj, func);
  }

  //! Static funtion
  void connect(Ret (*func)(Args...))
  {
    f = func;
  }

  //! Callback<>
  void connect(Callback<Ret, Args...> *cb)
  {
    f = cb->f;
  }
};

template<typename Ret1, typename... Args1, class T, typename Ret2, typename... Args2>
void connect(Callback<Ret1, Args1...> *cb, T *obj , Ret2 (T::*func)(Args2...))
{
  cb->connect(obj, func);
}

template<typename Ret1, typename... Args1, typename Ret2, typename... Args2>
void connect(Callback<Ret1, Args1...> *cb, Ret2 (*func)(Args2...))
{
  cb->connect(func);
}

template<typename Ret1, typename... Args1, typename Ret2, typename... Args2>
void connect(Callback<Ret1, Args1...> *cb1, Callback<Ret2, Args2...> *cb2)
{
  cb1->connect(cb2);
}




}

#endif // PFUNCTION_H
