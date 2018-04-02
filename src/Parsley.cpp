#include "Parsley.h"

using namespace Parsley;


Loop LoopUtils::default_loop = Loop(uv_default_loop());

Loop *LoopUtils::defaultLoop()
{
  return &default_loop;
}

Loop::Loop()
{
  loop = (uv_loop_t*) malloc(sizeof(uv_loop_t));
  uv_loop_init(loop);
}

Loop::Loop(uv_loop_t *l)
{
  loop = l;
}

int Loop::run(const uv_run_mode &mode)
{
  return uv_run(loop, mode);
}

/*!
 * \brief Loop::close
 * WARNING: This is a synchronous function, which blocks to wait for uv_walk function to free all the handles in the loop.
 */
void Loop::close()
{
  if(this->tryClose() == UV_EBUSY)
    {
      uv_walk_cb uvWalkCb = [](uv_handle_t* handle, void* arg) {
        uv_close_cb uvCloseCb = [](uv_handle_t* handle) {
          if (handle != NULL)
            {
              free(handle);
            }
        };
        uv_close(handle, uvCloseCb);
      };

      uv_walk(uvHandle(), uvWalkCb, NULL);
      while(1)
        {
          if(uv_loop_alive(uvHandle()))
            continue;
          else
            break;
        }

      this->run(UV_RUN_DEFAULT);
      int result = this->tryClose();
      if (result)
        {
//          qDebug() << "failed to close libuv loop: " << uv_err_name(result);
        }
      else
        {
//          qDebug() << "libuv loop is closed successfully!\n";
        }
    }
  else
    {
//      qDebug() << "libuv loop is closed successfully!\n";
    }
}

int Loop::tryClose()
{
  return uv_loop_close(loop);
}


uv_loop_t *Loop::uvHandle()
{
  return loop;
}


