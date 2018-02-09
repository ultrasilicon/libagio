#ifndef LOOP_H
#define LOOP_H

#include "Log.h"
#include "Parsley.h"


PARSLEY_NAMESPACE_BEGIN
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






PARSLEY_NAMESPACE_END
#endif // LOOP_H
