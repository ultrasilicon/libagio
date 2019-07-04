#include "agio.h"

using namespace Agio;


Loop Loop::default_loop = Loop(uv_default_loop());

Loop *Loop::defaultLoop()
{
  return &default_loop;
}

Loop::Loop()
  : loop_(new uv_loop_t())
{
  uv_loop_init(loop_);
}

Loop::Loop(uv_loop_t *l)
  : loop_(l)
{
}

Loop::~Loop()
{
  close();
}

int Loop::run(const uv_run_mode &mode)
{
  return uv_run(loop_, mode);
}

/*!
 * @brief Loop::close
 * @note WARNING: This is a synchronous function, which blocks to wait for uv_walk function to free all the handles in the loop.
 */
void Loop::close()
{
  if(tryClose() == UV_EBUSY)
    {
      uv_walk_cb uvWalkCb = [](uv_handle_t* handle, void*) {
        uv_close_cb uvCloseCb = [](uv_handle_t* handle) {
          if (handle)
            free(handle);
        };
        uv_close(handle, uvCloseCb);
      };

      uv_walk(uvHandle(), uvWalkCb, nullptr);
      while(1)
        {
          if(uv_loop_alive(uvHandle()))
            continue;
          else
            break;
        }

      run(UV_RUN_DEFAULT);
      int result = tryClose();
      if (result)
        {
//          std::cerr << "failed to close libuv loop: " << uv_err_name(result);
        }
    }
}

int Loop::tryClose()
{
  return uv_loop_close(loop_);
}


uv_loop_t *Loop::uvHandle()
{
  return loop_;
}
