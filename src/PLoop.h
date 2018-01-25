#ifndef ABSTRACTSOCKET_H
#define ABSTRACTSOCKET_H

#include "Log.h"

#ifdef Q_OS_WIN
#include "../libuv/win32/include/uv.h"
#else
#include <uv.h>
#endif


namespace Parsley {
  class Loop;

  class Loop
  {
  public:

  };

}






#endif // ABSTRACTSOCKET_H
