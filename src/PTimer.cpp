#include "PTimer.h"

using namespace Parsley;



Timer::Timer(uv_loop_t *loop)
{
  timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
  uv_timer_init(loop, timer);
}

void Timer::bindCb(const Timer::TimeOutCb &cb)
{

}

void Timer::start(const uint64_t &timeout, const uint64_t &repeat)
{
    uv_timer_start(timer, [](uv_timer_t *handle){
      //! TODO: add callback

    }, timeout, repeat);

    //! TODO: add instance reg
}
