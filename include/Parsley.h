#ifndef PARSLEY_H
#define PARSLEY_H

#if defined(_WIN32)
#include "../libuv/win32/include/uv.h"
#else
#include <uv.h>
#endif

#include "PFunction.h"

#include <stdlib.h>
#include <map>


#define PARSLEY_NAMESPACE_BEGIN namespace Parsley {
#define PARSLEY_NAMESPACE_END }

PARSLEY_NAMESPACE_BEGIN

enum Mode {
  AsyncMode = 0,
  SyncMode = 1
};

class LoopUtils;
class Loop;

template <typename UvHandle, typename PHandle>
class PObject;

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
  ~Loop();

  int run(const uv_run_mode &mode = UV_RUN_DEFAULT);
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
  if(uv_handle)
    {
      removeInstance(uv_handle);
      free(uv_handle);
    }
}

template<typename UvHandle, typename PHandle>
void PObject<UvHandle, PHandle>::regInstance(UvHandle *uvHandle, PHandle *pHandle)
{
  if(!instance_map.count(uvHandle))
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


PARSLEY_NAMESPACE_END
#endif // PARSLEY_H
