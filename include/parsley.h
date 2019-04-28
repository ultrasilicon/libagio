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
  Async = 0,
  Sync = 1
};

class Loop;

template <typename CType, typename PType>
class PObject;
template <typename UvT, typename PType>
class PUvObject;



class Loop
{
public:
  static Loop *defaultLoop();
  static Loop default_loop;

  Loop();
  Loop(uv_loop_t* l);
  ~Loop();

  int run(const uv_run_mode &mode = UV_RUN_DEFAULT);
  void close();
  int tryClose();
  uv_loop_t* uvHandle();

private:
  uv_loop_t* loop_;
};



/*!
 * \arg CType: typename of the c obj
 * \arg PType: typename of the parsley obj
 */
template <typename CType, typename PType>
class PObject
{
public:
  PObject();
  ~PObject();

  static void regInstance(CType *cHandle, PType *pHandle);
  static void removeInstance(CType *cHandle);
  static PType *getInstance(CType *cHandle);
  CType *getHandle();

protected:
  CType *obj_;

private:
  static std::unordered_map<CType*, PType*> instances_;
};

template <typename CType, typename PType>
std::unordered_map<CType*, PType*> PObject<CType, PType>::instances_;

template<typename CType, typename PType>
PObject<CType, PType>::PObject()
  : obj_(new CType())
{
}

template<typename CType, typename PType>
PObject<CType, PType>::~PObject()
{
  if(obj_)
    removeInstance(obj_);
}

template<typename CType, typename PType>
void PObject<CType, PType>::regInstance(CType* cHandle, PType* pHandle)
{
  if(!instances_.count(cHandle))
    instances_.insert({ cHandle, pHandle });
}

template<typename CType, typename PType>
void PObject<CType, PType>::removeInstance(CType* cHandle)
{
  instances_.erase(instances_.find(cHandle));
}

template<typename CType, typename PType>
PType* PObject<CType, PType>::getInstance(CType* cHandle)
{
  return instances_.at(cHandle);
}

template<typename CType, typename PType>
CType* PObject<CType, PType>::getHandle()
{
  return obj_;
}

template <typename PType>
struct PUvObjectData
{
  PType *pHandle;
  // ...
};

template <typename UvType, typename PType>
class PUvObject
    : public PObject<UvType, PType>
{
public:
  PUvObject(Loop *l);

  Loop* getLoop();

protected:
  Loop* loop_;
};

template<typename UvType, typename PType>
PUvObject<UvType, PType>::PUvObject(Loop *l)
  : PObject<UvType, PType>()
  , loop_(l)
{
//  PObject<UvType, PType>::obj_->data = PUvObjectData<PType>{};
}

template<typename UvType, typename PType>
Loop *PUvObject<UvType, PType>::getLoop()
{
  return loop_;
}


P_NS_END
#endif // PARSLEY_H
