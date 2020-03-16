#ifndef AGIO_PROMISE_H
#define AGIO_PROMISE_H

#include "agio.h"
#include "async_event.h"

A_NS_BEGIN

template<typename... Ts>
class Promise;




template<typename... Ts>
class Promise {
public:
  using T = typename std::tuple_element<0, std::tuple<Ts...> >::type;

  template<typename U = T, class std::enable_if <!std::is_void<U>::value, int >::type = 0>
  void resolve(U& t) {

  }

  template<typename U = T, class std::enable_if <std::is_void<U>::value, int >::type = 0>
  void resolve() {

  }

  Promise<T>& then() {

  }

  Promise<T>& finally() {

  }

  Promise<T>& err() {

  }

private:
  Promise<T>* next_promise_;

};


A_NS_END
#endif // AGIO_PROMISE_H
