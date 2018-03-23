#ifndef TIMER_H
#define TIMER_H

#include <functional>

#include "Parsley.h"
#include "PLoop.h"

//#include "Log.h"


PARSLEY_NAMESPACE_BEGIN
class TimerUtils;
class Timer;

class TimerUtils
    : public PObject<uv_timer_t, Timer>
{
public:
  static void timeoutCb(uv_timer_t *handle);

};

class Timer
    : public TimerUtils
{
public:
  typedef std::function<void (Timer*)> TimeoutCb;

  Timer(Loop *l);
  Timer(const uint64_t &timeout, const uint64_t &repeat, Loop *l);

  void bindCb(const TimeoutCb &cb);
  bool callTimeout();
  bool start();
  void start(const uint64_t &timeout, const uint64_t &repeat);
  void stop();

private:
  uv_timer_t *timer;
  uint64_t delay = 0;
  uint64_t interval = 0;

  TimeoutCb timeout_cb;


};

//  uv_timer_t *heart_beat_timer2 = (uv_timer_t*)malloc(sizeof(uv_timer_t));
//  uv_timer_init(loop, heart_beat_timer2);
//  uv_timer_start(heart_beat_timer2, [](uv_timer_t *handle){
//    qDebug()<<counter << "p/s";
//    counter = 0;
//  }, 10, 1000);



PARSLEY_NAMESPACE_END
#endif // TIMER_H
