#ifndef AGIO_H
#define AGIO_H

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

#define A_NS_BEGIN namespace Agio {
#define A_NS_END }
#define CXX_MALLOC(type) static_cast<type*>(malloc(sizeof(type)))
#define CXX_MALLOC_CSTR(size) static_cast<char*>(malloc(size * sizeof(char)))
#define A_USED(var) (void)var

A_NS_BEGIN

enum Mode {
  Async = 0,
  Sync = 1
};

class Loop;
template <typename CType, typename PType>
class AgioObject;
template <typename PType>
struct UvObjectData;
template <typename UvT, typename PType>
class UvObject;



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
 * \arg PType: typename of the agio obj
 */
template <typename CType, typename PType>
class AgioObject
{
public:
  AgioObject()
    : obj_(new CType())
  { }

  ~AgioObject()
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
struct UvObjectData
{
  PType* pHandle;
  // ...
};




template <typename UvType, typename PType>
class UvObject
    : public AgioObject<UvType, PType>
{
public:
  static PType* getPHandle(UvType* handle)
  {
    return static_cast<UvObjectData<PType>*>(handle->data)->pHandle;
  }

  UvObject(Loop* l, PType* pHandle)
    : AgioObject<UvType, PType>()
    , loop_(l)
    , data_(new UvObjectData<PType>{pHandle})
  {
    AgioObject<UvType, PType>::obj_->data = data_;
  }

  ~UvObject()
  {
    delete data_;
  }

  Loop* getLoop()
  {
    return loop_;
  }

protected:
  Loop* loop_;
  UvObjectData<PType>* data_;
};










A_NS_END
#endif // AGIO_H
