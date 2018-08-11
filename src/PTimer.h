#ifndef TIMER_H
#define TIMER_H

/*
 * Maintainance:
 * 1. Remove any duplicated initializations or allocations
 * 2. Check API usage, correct wrong usages.
 * 3. Correct naming convention.
 */

#include <functional>

#include "Parsley.h"

//#include "Log.h"


PARSLEY_NAMESPACE_BEGIN
class TimerUtils;
class Timer;

class TimerUtils
    : public PObject<uv_timer_t, Timer>
{
public:
  TimerUtils(Loop *l) : PObject(l){}
protected:
  static void timeoutCb(uv_timer_t *handle);

};

class Timer
    : public TimerUtils
{
  friend class TimerUtils;
public:
  enum CallbackType {
    Started = 1,
    Stopped = 2,
    Timeout = 3
  };
  using StartedCb = std::function<void (Timer*)>;
  using StoppedCb = std::function<void (Timer*)>;
  using TimeoutCb = std::function<void (Timer*)>;

  Timer(Loop *l);
  Timer(const uint64_t &timeout, const uint64_t &repeat, Loop *l);

  template<typename T>
  void bindCb(const CallbackType &t, const T &cb);
  void callTimeout();

  bool start();
  int start(const uint64_t &timeout, const uint64_t &repeat);
  void stop();

private:
  uint64_t delay = 0;
  uint64_t interval = 0;

  TimeoutCb timeout_cb;
};

template<typename T>
void Timer::bindCb(const CallbackType &t, const T &cb)
{
  switch (t) {
    case Started:
      timeout_cb = (StartedCb)cb;
      break;
    case Stopped:
      timeout_cb = (StoppedCb)cb;
      break;
    case Timeout:
      timeout_cb = (TimeoutCb)cb;
      break;
    }
}


PARSLEY_NAMESPACE_END
#endif // TIMER_H
