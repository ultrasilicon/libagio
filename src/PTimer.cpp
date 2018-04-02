#include "PTimer.h"

using namespace Parsley;

void TimerUtils::timeoutCb(uv_timer_t *handle)
{
  getInstance(handle)->callTimeout();
}




Timer::Timer(Loop *l)
  : TimerUtils(l)
{
  timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
  uv_timer_init(l->uvHandle(), timer);

  regInstance(timer, this);
}

Timer::Timer(const uint64_t &timeout, const uint64_t &repeat, Loop *l)
  : TimerUtils(l)
  , delay(timeout)
  , interval(repeat)
{
  timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
  uv_timer_init(l->uvHandle(), timer);

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

bool Timer::start()
{
  if(delay && interval)
    {
      uv_timer_start(timer, timeoutCb, delay, interval);
      return true;
    }
  return false;
}

void Timer::start(const uint64_t &timeout, const uint64_t &repeat)
{
  delay = timeout;
  interval = repeat;
  uv_timer_start(timer, timeoutCb, timeout, repeat);
}

void Timer::stop()
{
  uv_timer_stop(timer);
}

