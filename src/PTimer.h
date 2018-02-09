#ifndef TIMER_H
#define TIMER_H

#include <functional>

#include "Log.h"

#ifdef Q_OS_WIN
#include "../libuv/win32/include/uv.h"
#else
#include <uv.h>
#endif


namespace Parsley {
  class TimerUtils;
  class Timer;

  class TimerUtils
  {
  public:

  };

  class Timer : public TimerUtils
  {
  public:
    typedef std::function<void (Timer*)> TimeOutCb;

    Timer(uv_loop_t *loop);

    void bindCb(const TimeOutCb &cb);
    void start(const uint64_t &timeout, const uint64_t &repeat);

  private:
    uv_timer_t *timer;


  };

//  uv_timer_t *heart_beat_timer2 = (uv_timer_t*)malloc(sizeof(uv_timer_t));
//  uv_timer_init(loop, heart_beat_timer2);
//  uv_timer_start(heart_beat_timer2, [](uv_timer_t *handle){
//    qDebug()<<counter << "p/s";
//    counter = 0;
//  }, 10, 1000);


}






#endif // TIMER_H
