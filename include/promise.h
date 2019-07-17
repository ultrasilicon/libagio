#ifndef AGIO_PROMISE_H
#define AGIO_PROMISE_H

#include "agio.h"
#include "async_event.h"

A_NS_BEGIN

template <typename Fun, typename ResolveFun>
struct Promise {
  Fun fun_;
  AsyncEvent* event_;

  Promise(Fun& fun)
    : fun_(fun)
    , event_(new AsyncEvent(Loop::defaultLoop()))
  {
    on(event_->onCalled, this, &Promise::exec);
  }

  void exec()
  {
    fun_();
  }

};


A_NS_END
#endif // AGIO_PROMISE_H
