#ifndef UTILS_H
#define UTILS_H

#include "parsley.h"


P_NS_BEGIN

class Utils
{
public:
  static int getFd(uv_handle_t* handle);
  static const char* errStr(int err);
};


P_NS_END
#endif // UTILS_H
