#ifndef AGIO_H
#define AGIO_H

#if defined(_WIN32)
#include "../libuv/win32/include/uv.h"
#else
#include <uv.h>
#endif


#define A_NS_BEGIN namespace Agio {
#define A_NS_END }
#define CXX_MALLOC(type) static_cast<type*>(malloc(sizeof(type)))
#define CXX_MALLOC_CSTR(size) static_cast<char*>(malloc(size * sizeof(char)))
#define A_USED(var) (void)var

A_NS_BEGIN

template <typename CType, typename AType>
class AgioObject;

/*!
 * \arg CType: type of the c obj
 * \arg AType: type of the agio obj
 */
template <typename CType, typename AType>
class AgioObject
{
public:
  enum GCRule {
    Uv = 0,
    Agio
  };

  AgioObject(const GCRule& rule = Agio)
    : obj_(new CType())
    , gc_rule_(rule)
  { }

  AgioObject(CType* obj, const GCRule& rule = Agio)
    : obj_(obj)
    , gc_rule_(rule)
  { }

  ~AgioObject()
  {
    if(gc_rule_ == Uv)
      return;
    if(obj_)
      delete obj_;
    obj_ = nullptr;
  }

  constexpr CType* cObject() const
  {
    return obj_;
  }

protected:
  CType* obj_;
  GCRule gc_rule_;
};














A_NS_END
#endif // AGIO_H
