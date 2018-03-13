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
  static Loop *defaultLoop();
  static int close(uv_loop_t* loop);
  static int run(uv_loop_t*handle, uv_run_mode mode);

private:
  static Loop default_loop;

};

class Loop
    : public LoopUtils
{
public:
  Loop(uv_loop_t* l);

  int run(const uv_run_mode &mode);
  int close();
  uv_loop_t* uvHandle();


private:
  uv_loop_t* loop;
};






PARSLEY_NAMESPACE_END
#endif // LOOP_H
