#include "timer.h"

using namespace Parsley;

TimerUtils::TimerUtils(Loop *l)
  : PUvObject(l)
{
}

void TimerUtils::timeoutCb(uv_timer_t *handle)
{
  Timer *t = getInstance(handle);
  t->onTimedOut.call(t);
}




Timer::Timer(Loop *l)
  : TimerUtils(l)
{
  uv_timer_init(l->uvHandle(), m_uv_obj);
  regInstance(m_uv_obj, this);
}

Timer::Timer(const uint64_t &repeat, Loop *l)
  : TimerUtils(l)
  , m_repeat(repeat)
{
  uv_timer_init(l->uvHandle(), m_uv_obj);
  regInstance(m_uv_obj, this);
}

Timer::Timer(const uint64_t &timeout, const uint64_t &repeat, Loop *l)
  : TimerUtils(l)
  , m_timeout(timeout)
  , m_repeat(repeat)
{
  uv_timer_init(l->uvHandle(), m_uv_obj);
  regInstance(m_uv_obj, this);
}

int Timer::start()
{
  return start(m_timeout, m_repeat);
}

int Timer::start(const uint64_t &repeat)
{
  m_repeat = repeat;
  return start(m_timeout, repeat);
}

int Timer::start(const uint64_t &timeout, const uint64_t &repeat)
{
  m_timeout = timeout;
  m_repeat = repeat;
  return uv_timer_start(m_uv_obj
                        , timeoutCb
                        , timeout
                        , repeat);
}

void Timer::stop()
{
  uv_timer_stop(m_uv_obj);
}

