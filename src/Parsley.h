#ifndef PARSLEY_H
#define PARSLEY_H

#if defined(_WIN32)
#include "../libuv/win32/include/uv.h"
#else
#include <uv.h>
#endif

#include <stdlib.h>
#include <functional>
#include <map>


#define PARSLEY_NAMESPACE_BEGIN namespace Parsley {
#define PARSLEY_NAMESPACE_END }

PARSLEY_NAMESPACE_BEGIN

enum Mode {
  AsyncMode = 0,
  SyncMode = 1
};

using Buffer = uv_buf_t;

class LoopUtils;
class Loop;

template <typename UvHandle, typename PHandle>
class PObject;
template<class T, typename Ret, typename... Args>
struct Function;
template<class T, typename Ret, typename... Args>
struct Functor;


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

  template<class T>
  void bind(T *obj , Ret (T::*func)(Args...))
  {
    f = Functor<T, Ret, Args...>(obj, func);
  }

  Ret call(Args... args) noexcept
  {
    if(f)
      {
          return f(args ...);
      }
    return Ret();
  }
};

class LoopUtils
{
public:
  static Loop *defaultLoop();

private:
  static Loop default_loop;
};

class Loop
    : public LoopUtils
{
public:
  Loop();
  Loop(uv_loop_t* l);

  int run(const uv_run_mode &mode);
  void close();
  int tryClose();
  uv_loop_t* uvHandle();

private:
  uv_loop_t* loop;
};


template <typename UvHandle, typename PHandle>
class PObject
{
public:
  PObject(Loop *l);
  ~PObject();
  static void regInstance(UvHandle *uvHandle, PHandle *pHandle);
  static void removeInstance(UvHandle *uvHandle);
  static PHandle *getInstance(UvHandle *uvHandle);

  UvHandle *getUvHandle();
  Loop *getLoop();

protected:
  Loop *loop;
  UvHandle *uv_handle;

private:
  static std::map<UvHandle*, PHandle*> instance_map;

};

//template<typename UvHandle, typename PHandle>
template<typename Ret1, typename... Args1, class T, typename Ret2, typename... Args2>
void bind(Callback<Ret1, Args1...> *cb, T *obj , Ret2 (T::*func)(Args2...))
{
  cb->bind(obj, func);
}

template <typename UvHandle, typename PHandle>
std::map<UvHandle*, PHandle*> PObject<UvHandle, PHandle>::instance_map;

template<typename UvHandle, typename PHandle>
PObject<UvHandle, PHandle>::PObject(Loop *l)
  : loop(l)
  , uv_handle((UvHandle*) malloc(sizeof(UvHandle)))
{
}

template<typename UvHandle, typename PHandle>
PObject<UvHandle, PHandle>::~PObject()
{
  removeInstance(uv_handle);
//  free(uv_handle); //! Do or Don't?
}

template<typename UvHandle, typename PHandle>
void PObject<UvHandle, PHandle>::regInstance(UvHandle *uvHandle, PHandle *pHandle)
{
  instance_map.insert({ uvHandle, pHandle });
}

template<typename UvHandle, typename PHandle>
void PObject<UvHandle, PHandle>::removeInstance(UvHandle *uvHandle)
{
  instance_map.erase(instance_map.find(uvHandle));
}

template<typename UvHandle, typename PHandle>
PHandle *PObject<UvHandle, PHandle>::getInstance(UvHandle *uvHandle)
{
  return instance_map[uvHandle];
}

template<typename UvHandle, typename PHandle>
UvHandle *PObject<UvHandle, PHandle>::getUvHandle()
{
  return uv_handle;
}

template<typename UvHandle, typename PHandle>
Loop *PObject<UvHandle, PHandle>::getLoop()
{
  return loop;
}

//template<typename UvHandle, typename PHandle>
//template <typename T>
//bool PObject<UvHandle, PHandle>::tryCall(PHandle *pObj, const T &funct)
//{
//  if (pObj->funct) {
//      return false;
//    }
//  pObj->funct();
//  return true;
//}








PARSLEY_NAMESPACE_END
#endif // PARSLEY_H
