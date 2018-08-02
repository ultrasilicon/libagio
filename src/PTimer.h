#ifndef TIMER_H
#define TIMER_H

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
public:
  using TimeoutCb = std::function<void (Timer*)>;

  Timer(Loop *l);
  Timer(const uint64_t &timeout, const uint64_t &repeat, Loop *l);

  void bindCb(const TimeoutCb &cb);
  bool callTimeout();
  bool start();
  int start(const uint64_t &timeout, const uint64_t &repeat);
  void stop();

private:
  uint64_t delay = 0;
  uint64_t interval = 0;

  TimeoutCb timeout_cb;


};


PARSLEY_NAMESPACE_END
#endif // TIMER_H
