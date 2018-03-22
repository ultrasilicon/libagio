#ifndef LOOP_H
#define LOOP_H

#include "Parsley.h"


PARSLEY_NAMESPACE_BEGIN
class LoopUtils;
class Loop;

class LoopUtils
    : InstanceMap<uv_loop_t, Loop>
{
public:
  static Loop *defaultLoop();

private:
  static Loop default_loop;
};

class Loop
    : public LoopUtils
{
public:
  Loop();
  Loop(uv_loop_t* l);

  int run(const uv_run_mode &mode);
  void close();
  int tryClose();
  uv_loop_t* uvHandle();

private:
  uv_loop_t* loop;
};






PARSLEY_NAMESPACE_END
#endif // LOOP_H
