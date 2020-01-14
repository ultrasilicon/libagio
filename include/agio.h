#ifndef AGIO_H
#define AGIO_H

#if defined(_WIN32)
#include "../libuv/win32/include/uv.h"
#else
#include <uv.h>
#endif

#include "function.h"

#define A_NS_BEGIN namespace Agio {
#define A_NS_END }
#define CXX_MALLOC(type) static_cast<type*>(malloc(sizeof(type)))
#define CXX_MALLOC_CSTR(size) static_cast<char*>(malloc(size * sizeof(char)))
#define A_USED(var) (void)var

A_NS_BEGIN

template <typename CType, typename PType>
class AgioObject;

/*!
 * \arg CType: type of the c obj
 * \arg PType: type of the agio obj
 */
template <typename CType, typename PType>
class AgioObject
{
public:
  AgioObject()
    : obj_(new CType())
  { }

  AgioObject(CType* obj)
    : obj_(obj)
  { }

  ~AgioObject()
  {
    // TODO: find out which handles libuv frees
//    if(obj_)
//      delete obj_;
  }

  CType* cObject()
  {
    return obj_;
  }

protected:
  CType* obj_;
};














A_NS_END
#endif // AGIO_H
