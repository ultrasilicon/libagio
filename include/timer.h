#ifndef TIMER_H
#define TIMER_H

/*
 * Maintainance:
 * 1. Remove any duplicated initializations or allocations
 * 2. Check API usage, correct wrong usages.
 * 3. Correct naming convention.
 */

#include <functional>

#include "parsley.h"

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
  CallbackHandler<void, Timer*> onTimedOut;

  Timer(Loop *l);
  Timer(const uint64_t &repeat, Loop *l);
  Timer(const uint64_t &timeout, const uint64_t &repeat, Loop *l);
  int start();
  int start(const uint64_t &repeat);
  int start(const uint64_t &timeout, const uint64_t &repeat);
  void stop();


private:
  uint64_t m_timeout = 0;
  uint64_t m_repeat = 0;
};


P_NS_END
#endif // TIMER_H
