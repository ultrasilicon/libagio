#include "PLoop.h"

using namespace Parsley;


uv_loop_t *LoopUtils::defaultLoop()
{
  return uv_default_loop();
}

int LoopUtils::close(uv_loop_t *loop)
{
  return uv_loop_close(loop);
}

int LoopUtils::run(uv_loop_t *handle, uv_run_mode mode)
{
  return uv_run(handle, mode);
}
