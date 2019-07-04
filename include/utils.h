#ifndef UTILS_H
#define UTILS_H

#include "agio.h"


A_NS_BEGIN

class Utils
{
public:
  static int getFd(uv_handle_t* handle);
  static const char* errStr(int err);
};


A_NS_END
#endif // UTILS_H
