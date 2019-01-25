#include "PTimer.h"

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

Timer::Timer(const uint64_t &timeout, const uint64_t &repeat, Loop *l)
  : TimerUtils(l)
  , m_delay(timeout)
  , m_interval(repeat)
{
  uv_timer_init(l->uvHandle(), m_uv_obj);
  regInstance(m_uv_obj, this);
}

bool Timer::start()
{
  if(m_delay && m_interval)
    {
      uv_timer_start(m_uv_obj
                     , timeoutCb
                     , m_delay
                     , m_interval);
      return true;
    }
  return false;
}

int Timer::start(const uint64_t &timeout, const uint64_t &repeat) //! TODO: duplicated?
{
  m_delay = timeout;
  m_interval = repeat;
  return uv_timer_start(m_uv_obj
                        , timeoutCb
                        , timeout
                        , repeat);
}

void Timer::stop()
{
  uv_timer_stop(m_uv_obj);
}

