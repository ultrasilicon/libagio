#ifndef TIMER_H
#define TIMER_H

#include "service.h"
#include "callback.h"

A_NS_BEGIN

class Timer
    : public AgioService<uv_timer_t, Timer>
{
  static void timeoutCb(uv_timer_t* handle);

public:
  Callback<void(Timer*)> onTimedOut;

  Timer(Loop* l);
  Timer(const uint64_t& repeat, Loop* l);
  Timer(const uint64_t& timeout, const uint64_t& repeat, Loop* l);

  void stop() const;
  int start() const;
  int start(const uint64_t& repeat);
  int start(const uint64_t& timeout, const uint64_t& repeat);


private:
  uint64_t timeout_ = 0;
  uint64_t repeat_ = 0;
};


A_NS_END
#endif // TIMER_H
