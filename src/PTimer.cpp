#include "PTimer.h"

using namespace Parsley;

void TimerUtils::timeoutCb(uv_timer_t *handle)
{
  getInstance(handle)->callTimeout();
}




Timer::Timer(uv_loop_t *loop)
{
  timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
  uv_timer_init(loop, timer);

  regInstance(timer, this);
}

void Timer::bindCb(const TimeoutCb &cb)
{
  timeout_cb = cb;
}

bool Timer::callTimeout()
{
  if(timeout_cb)
    {
      timeout_cb(this);
      return true;
    }
  return false;
}

void Timer::start(const uint64_t &timeout, const uint64_t &repeat)
{
    uv_timer_start(timer, timeoutCb, timeout, repeat);

    //! TODO: add instance reg
}

