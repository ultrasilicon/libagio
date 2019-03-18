#ifndef PARSLEY_H
#define PARSLEY_H

#if defined(_WIN32)
#include "../libuv/win32/include/uv.h"
#else
#include <uv.h>
#endif

#include "function.h"

#include <stdlib.h>
#include <unordered_map>
#include <string>
#include <iostream>

#define P_NS_BEGIN namespace Parsley {
#define P_NS_END }
#define CXX_MALLOC(type) static_cast<type*>(malloc(sizeof(type)))
#define CXX_MALLOC_CSTR(size) static_cast<char*>(malloc(size * sizeof(char)))
#define P_USED(var) (void)var

P_NS_BEGIN

enum Mode {
  AsyncMode = 0,
  SyncMode = 1
};

class LoopUtils;
class Loop;

template <typename UvHandle, typename PHandle>
class PUvObject;

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

//template <typename UvHandle, typename PHandle>
//class PObject
//{
//public:
//  PObject();
//  ~PObject();

//  static void regInstance(UvHandle *uvHandle, PHandle *pHandle);
//  static void removeInstance(UvHandle *uvHandle);
//  static PHandle *getInstance(UvHandle *uvHandle);
//  UvHandle *getUvHandle();
//protected:
//  UvHandle *m_c_obj;

//private:
//  static std::unordered_map<UvHandle*, PHandle*> m_instances;
//};

template <typename UvHandle, typename PHandle>
class PUvObject
{
public:
  PUvObject(Loop *l);
  ~PUvObject();

  static void regInstance(UvHandle *uvHandle, PHandle *pHandle);
  static void removeInstance(UvHandle *uvHandle);
  static PHandle *getInstance(UvHandle *uvHandle);
  UvHandle *getUvHandle();
  Loop *getLoop();

protected:
  Loop *m_loop;
  UvHandle *m_uv_obj;

private:
  static std::unordered_map<UvHandle*, PHandle*> m_instances;

};

template <typename UvHandle, typename PHandle>
std::unordered_map<UvHandle*, PHandle*> PUvObject<UvHandle, PHandle>::m_instances;

template<typename UvHandle, typename PHandle>
PUvObject<UvHandle, PHandle>::PUvObject(Loop *l)
  : m_loop(l)
  , m_uv_obj(new UvHandle())
{
}

template<typename UvHandle, typename PHandle>
PUvObject<UvHandle, PHandle>::~PUvObject()
{
  if(m_uv_obj)
    removeInstance(m_uv_obj);
}

template<typename UvHandle, typename PHandle>
void PUvObject<UvHandle, PHandle>::regInstance(UvHandle *uvHandle, PHandle *pHandle)
{
  if(!m_instances.count(uvHandle))
    m_instances.insert({ uvHandle, pHandle });
}

template<typename UvHandle, typename PHandle>
void PUvObject<UvHandle, PHandle>::removeInstance(UvHandle *uvHandle)
{
  m_instances.erase(m_instances.find(uvHandle));
}

template<typename UvHandle, typename PHandle>
PHandle *PUvObject<UvHandle, PHandle>::getInstance(UvHandle *uvHandle)
{
  return m_instances[uvHandle];
}

template<typename UvHandle, typename PHandle>
UvHandle *PUvObject<UvHandle, PHandle>::getUvHandle()
{
  return m_uv_obj;
}

template<typename UvHandle, typename PHandle>
Loop *PUvObject<UvHandle, PHandle>::getLoop()
{
  return m_loop;
}


P_NS_END
#endif // PARSLEY_H
