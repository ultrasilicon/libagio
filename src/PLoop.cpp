#include "PLoop.h"

using namespace Parsley;

Loop LoopUtils::default_loop = Loop(uv_default_loop());

Loop *LoopUtils::defaultLoop()
{
  return &default_loop;
}

int LoopUtils::close(uv_loop_t *loop)
{
  return uv_loop_close(loop);
}

int LoopUtils::run(uv_loop_t *handle, uv_run_mode mode)
{
  return uv_run(handle, mode);
}



Loop::Loop(uv_loop_t *l)
  : loop(l)
{
}

int Loop::run(const uv_run_mode &mode)
{
  return uv_run(loop, mode);
}

int Loop::close()
{
  return uv_loop_close(loop);
}

uv_loop_t *Loop::uvHandle()
{
  return loop;
}
