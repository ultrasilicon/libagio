#include "loop.h"

#include <stdlib.h>

using namespace Agio;

Loop Loop::default_loop = Loop(uv_default_loop());

Loop* Loop::defaultLoop()
{
  return &default_loop;
}

Loop::Loop()
  : AgioObject(GCRule::Uv)
{
  uv_loop_init(obj_);
}

Loop::Loop(uv_loop_t* l)
  : AgioObject(l, GCRule::Uv)
{
}

Loop::~Loop()
{
  close();
}

int Loop::run(const uv_run_mode& mode)
{
  return uv_run(obj_, mode);
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
            {
              free(handle);
              handle = nullptr;
            }
        };
        uv_close(handle, uvCloseCb);
      };

      uv_walk(obj_, uvWalkCb, nullptr);
      while(1)
        {
          if(uv_loop_alive(obj_))
            continue;
          else
            break;
        }

      run(UV_RUN_DEFAULT);
      int result = tryClose();
      if (result)
        {
          fprintf(stderr, "Failed to close libuv loop: %s\n", uv_err_name(result));
        }
    }
}

int Loop::tryClose()
{
  return uv_loop_close(obj_);
}




