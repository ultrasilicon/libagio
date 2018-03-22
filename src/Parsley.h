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

using Buffer = uv_buf_t;

template <typename UvHandle, typename PHandle>
class InstanceMap
{
public:
  void regInstance(UvHandle *uvHandle, PHandle *pHandle);
  static PHandle *getInstance(UvHandle *uvHandle);

private:
  static std::map<UvHandle*, PHandle*> instance_map;



};

template <typename UvHandle, typename PHandle>
std::map<UvHandle*, PHandle*> InstanceMap<UvHandle, PHandle>::instance_map;

template<typename UvHandle, typename PHandle>
void InstanceMap<UvHandle, PHandle>::regInstance(UvHandle *uvHandle, PHandle *pHandle)
{
  instance_map.insert({ uvHandle, pHandle });
}

template<typename UvHandle, typename PHandle>
PHandle *InstanceMap<UvHandle, PHandle>::getInstance(UvHandle *uvHandle)
{
  return instance_map[uvHandle];
}




PARSLEY_NAMESPACE_END
#endif // PARSLEY_H
