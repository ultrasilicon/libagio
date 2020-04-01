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

/*!
 * \enum GCRule
 * \abstract Garbage collection rules
 */
enum GCRule {
  Manual = 0,
  Agio
};

template <typename CType, typename AType, GCRule>
class AgioObject;


/*!
 * \arg CType: type of the c obj
 * \arg AType: type of the agio obj
 */
template <typename CType, typename AType, GCRule gc_rule = Agio>
class AgioObject
{
public:
  AgioObject(const GCRule& rule = Agio)
    : obj_(new CType())
  { }

  AgioObject(CType* obj, const GCRule& rule = Agio)
    : obj_(obj)
  { }

  ~AgioObject()
  {
    if constexpr (gc_rule == Manual)
    {
      return;
    }

    if constexpr (gc_rule == Agio)
    {
      if(obj_)
        delete obj_;
      obj_ = nullptr;
    }
  }

  constexpr CType* cObject() const
  {
    return obj_;
  }

protected:
  CType* obj_;
};














A_NS_END
#endif // AGIO_H
