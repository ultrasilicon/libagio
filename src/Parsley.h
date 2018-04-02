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
  Async = 0,
  Sync = 1
};
using Buffer = uv_buf_t;

class LoopUtils;
class Loop;
template <typename UvHandle, typename PHandle>
class PObject;


class LoopUtils
//    : public PObject<uv_loop_t, Loop>
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
  static void regInstance(UvHandle *uvHandle, PHandle *pHandle);
  static PHandle *getInstance(UvHandle *uvHandle);

  UvHandle *getUvHandle();
  UvHandle *getLoop();

protected:
  UvHandle *uv_handle;
  Loop *loop;

private:
  static std::map<UvHandle*, PHandle*> instance_map;

};

template <typename UvHandle, typename PHandle>
std::map<UvHandle*, PHandle*> PObject<UvHandle, PHandle>::instance_map;

template<typename UvHandle, typename PHandle>
void PObject<UvHandle, PHandle>::regInstance(UvHandle *uvHandle, PHandle *pHandle)
{
  instance_map.insert({ uvHandle, pHandle });
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
UvHandle *PObject<UvHandle, PHandle>::getLoop()
{
  return loop;
}








PARSLEY_NAMESPACE_END
#endif // PARSLEY_H
