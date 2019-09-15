#include "timer.h"

using namespace Agio;


void Timer::timeoutCb(uv_timer_t *handle)
{
  Timer *t = getPHandle(handle);
  t->onTimedOut(t);
}



Timer::Timer(Loop *l)
  : Timer(0, 0, l)
{
}

Timer::Timer(const uint64_t &repeat, Loop *l)
  : Timer(0, repeat, l)
{
}

Timer::Timer(const uint64_t &timeout, const uint64_t &repeat, Loop *l)
  : AgioService(l, this)
  , timeout_(timeout)
  , repeat_(repeat)
{
  uv_timer_init(l->cObject(), obj_);
}

void Timer::stop() const
{
  uv_timer_stop(obj_);
}

int Timer::start() const
{
  return uv_timer_start(obj_
                        , timeoutCb
                        , timeout_
                        , repeat_);
}

int Timer::start(const uint64_t &repeat)
{
  repeat_ = repeat;
  return start();
}

int Timer::start(const uint64_t &timeout, const uint64_t &repeat)
{
  timeout_ = timeout;
  repeat_ = repeat;
  return start();
}

