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
template <typename PType>
struct PUvObjectData;
template <typename UvT, typename PType>
class PUvObject;



class Loop
{
public:
  static Loop* defaultLoop();
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
  PObject()
    : obj_(new CType())
  { }

  ~PObject()
  {
    if(obj_)
      delete obj_;
  }

  CType* getHandle()
  {
    return obj_;
  }

protected:
  CType* obj_;
};




template <typename PType>
struct PUvObjectData
{
  PType* pHandle;
  // ...
};




template <typename UvType, typename PType>
class PUvObject
    : public PObject<UvType, PType>
{
public:
  static PType* getPHandle(UvType* handle)
  {
    return static_cast<PUvObjectData<PType>*>(handle->data)->pHandle;
  }


  PUvObject(Loop* l, PType* pHandle)
    : PObject<UvType, PType>()
    , loop_(l)
    , data_(new PUvObjectData<PType>{pHandle})
  {
    PObject<UvType, PType>::obj_->data = data_;
  }

  ~PUvObject()
  {
    delete data_;
  }


  Loop* getLoop()
  {
    return loop_;
  }

protected:
  Loop* loop_;
  PUvObjectData<PType>* data_;
};










P_NS_END
#endif // PARSLEY_H
