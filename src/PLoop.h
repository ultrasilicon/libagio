#ifndef LOOP_H
#define LOOP_H

#include "Log.h"

#ifdef Q_OS_WIN
#include "../libuv/win32/include/uv.h"
#else
#include <uv.h>
#endif


namespace Parsley {
  class LoopUtils;
  class Loop;

  class LoopUtils
  {
  public:
    static uv_loop_t* defaultLoop();
    static int close(uv_loop_t* loop);
    static int run(uv_loop_t*handle, uv_run_mode mode);

  };

  class Loop : public LoopUtils
  {


  };

}






#endif // LOOP_H
