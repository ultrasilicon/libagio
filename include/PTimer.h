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


P_NS_BEGIN
class TimerUtils;
class Timer;

class TimerUtils
    : public PUvObject<uv_timer_t, Timer>
{
public:
  TimerUtils(Loop *l);
protected:
  static void timeoutCb(uv_timer_t *handle);

};

class Timer
    : private TimerUtils
{
public:
  Callback<void, Timer*> onTimedOut;

  Timer(const uint64_t &timeout, const uint64_t &repeat, Loop *l);
  Timer(Loop *l);
  int start(const uint64_t &timeout, const uint64_t &repeat);
  bool start();
  void stop();


private:
  uint64_t m_delay = 0;
  uint64_t m_interval = 0;
};


P_NS_END
#endif // TIMER_H
